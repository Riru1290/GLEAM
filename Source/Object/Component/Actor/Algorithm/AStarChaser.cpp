#ifdef _DEBUG
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#include "../../../../../External/imgui/imgui.h"
#include "../../../../../External/imgui/imgui_impl_win32.h"
#include "../../../../../External/imgui/imgui_impl_dx12.h"
#endif
#include "../../../../Algorithm/NavMesh.h"
#include "../../Common/Transform.h"
#include "AStarChaser.h"

namespace
{
    struct Node
    {
        const NavMeshPolygon* polygon;
        Position3D pos;         // ノード間距離を算出する際の座標
        float distanceCost;	    // 実際の距離コスト
        float heuristicCost;	// ヒューリスティックコスト
        float totalCost;	    // トータルコスト

        int cameFromID;         // 親ノードのポリゴンID
    };

}

bool isStop = false;
AStarChaser::AStarChaser()
{
    speed_ = 200.0f;
}

Vector3 AStarChaser::GetToTargetDir() const
{
    if (!target_.has_value()) return Vector3();

    auto ret = (target_->get().pos - transform_->get().pos).Normalized();

    return ret;
}

void AStarChaser::UpdateComponent()
{
    Moving();
}

void AStarChaser::ReleaseComponent()
{
}

void AStarChaser::Moving()
{
    if (transform_.has_value() && target_.has_value() && 
        transform_->get().IsActive() && target_->get().IsActive())
    {
        Position3D& selfPos = transform_->get().pos;
        const Position3D& targetPos = target_->get().pos;

        if (limitDistance_ >= (selfPos - targetPos).Length())
        {
            isGoal_ = true;
            IdleAction_();
            return;
        }

        vector<const NavMeshPolygon*> polygons;
        auto route = FindPathAStar(selfPos, targetPos, polygons);

        if (route.size() <= 0)return;

#ifdef _DEBUG
        int state = isStop;
        ImGui::RadioButton("ASter:Start", &state, 0);
        ImGui::SameLine();
        ImGui::RadioButton("ASter:Stop", &state, 1);

        isStop = state;
#endif
        float movePow = MainTimer.GetMultDelta(speed_);


        for (const auto routePos : route)
        {

            Vector3 moveDir = (routePos - selfPos).Normalized();

            float distance = (routePos - selfPos).Length();

            if (distance <= movePow)
            {
                selfPos = routePos;
                if (moveDir == Vector3())continue;
                moveDir_ = moveDir;

                isGoal_ = true;
            }
            else
            {
                moveDir_ = moveDir;

                selfPos += moveDir_ * movePow;

                MoveAction_();

                isGoal_ = false;

                break;
            }
        }
    }
    else
    {
        IdleAction_();
    }
}


vector<Position3D> AStarChaser::FindPathAStar(const Position3D& startPos, const Position3D& goalPos,vector<const NavMeshPolygon*>& polygons)
{
    if (!navMesh_.has_value()) return {};

    // 開始地点と終了地点が属するポリゴンを特定
    const NavMeshPolygon* startPoly = navMesh_->get().GetPolygonContainingPoint(startPos);
    const NavMeshPolygon* goalPoly = navMesh_->get().GetPolygonContainingPoint(goalPos);

    if (!startPoly || !goalPoly) {
        return {}; // 無効な開始または終了ポリゴン
    }

    // 経路
    vector<Position3D> route;
    set<int> path;

    if (startPoly->id == goalPoly->id) {
        route.emplace_back(ClosestPointOnPolygon(goalPos, goalPoly->vertices));
        return route;
    }

    // オープンリスト(探索中のノード)
    unordered_map<int, Node> openList;

    // 探索が終了したノード
    unordered_map<int, Node> closeList;

    // 開始ノードを設定
    Node startNode;
    startNode.polygon = startPoly;
    startNode.pos = ClosestPointOnPolygon(startPos, startPoly->vertices);
    startNode.pos += (startPoly->GetCenter() - startNode.pos).Normalized() * 0.1f;
    startNode.distanceCost = 0.0f;
    startNode.heuristicCost = 0.0f;
    startNode.totalCost = 0.0f;
    startNode.cameFromID = -1;
    openList[startPoly->id] = startNode;

    // オープンリストの中から最もトータルコストが低いノードのIDを返す関数
    auto GetSmallestCostNodeID =
        [&openList]
        ()->int
        {
            int retID = -1;
            float smallCost = D3D12_FLOAT32_MAX;

            for (const auto& [id, node] : openList)
            {
                if (smallCost > node.totalCost)
                {
                    smallCost = node.totalCost;
                    retID = id;
                }
            }
            return retID;
        };

    // オープンリストが空になるまで繰り返す
    while (!openList.empty())
    {
        // オープンリストの中から最もトータルコストが低いノードを選択
        int currentID = GetSmallestCostNodeID();
        const Node* currentNode = &openList[currentID];


        // クローズリストに追加
        closeList.emplace(currentID, *currentNode);

        // もしも取得したノードがゴールノードと合致していればゴールとする
        if (currentID == goalPoly->id)
        {
            vector<const NavMeshPolygon*> routePolygons;

            Position3D goal = ClosestPointOnPolygon(goalPos, goalPoly->vertices);

            // 線分上に乗らないためのオフセット
            goal += (goalPoly->GetCenter() - goal).Normalized() * 0.1f;

            route.emplace_back(goal);

            // 親ノードをたどる
            int traceID = currentID;
            while (traceID != -1) {
                const Node& traceNode = closeList[traceID];
                route.emplace_back(traceNode.pos);

                routePolygons.emplace_back(&navMesh_->get().polygons[traceID]);

                traceID = traceNode.cameFromID;
            }

            polygons = routePolygons;
            return GetOptimizedRoute(route, routePolygons);
        }

        // 隣接ノードを検索
        for (const auto& neiNodeID : currentNode->polygon->neighbors)
        {
            // クローズリストに追加されたノードの場合スキップ
            bool isCloseID = false;
            for(const auto& [closeID,node] : closeList)
            {
                if (closeID == neiNodeID)
                {
                    isCloseID = true;
                }
            }
            if (isCloseID)continue;

            // 隣接ノード
            Node neiNode;
            neiNode.polygon = &navMesh_->get().polygons[neiNodeID];


            // 親ノード座標から最も近いポリゴン状の座標をノードの座標とする
            neiNode.pos = ClosestPointOnPolygon(currentNode->pos, neiNode.polygon->vertices);

            // 線分上に乗らないためのオフセット
            neiNode.pos += (neiNode.polygon->GetCenter() - neiNode.pos).Normalized() * 0.1f;

            // 距離コスト計算
            neiNode.distanceCost = currentNode->distanceCost +
                (neiNode.pos - currentNode->pos).Length();
            // ヒューリスティックコスト計算
            neiNode.heuristicCost = (goalPos - neiNode.pos).Length();

            // トータルコスト計算
            neiNode.totalCost = neiNode.distanceCost + neiNode.heuristicCost;

            // 親IDを設定
            neiNode.cameFromID = currentID;

            // オープンリストに追加されているノードの場合コストを比較する
            if (openList.contains(neiNodeID))
            {
                if (openList[neiNodeID].totalCost > neiNode.totalCost)
                {
                    openList[neiNodeID] = neiNode;
                }
            }
            else
            {
                // そのまま追加
                openList[neiNodeID] = neiNode;
            }

        }

        openList.erase(currentID);
    }

    return {};
}

vector<Position3D> AStarChaser::GetOptimizedRoute(vector<Position3D>& route, vector<const NavMeshPolygon*>& polygons)
{
    if (route.empty() || polygons.empty()) {
        return route; // 入力が無効の場合、そのまま返す
    }

    vector<Position3D> optRoute;

//#ifdef _DEBUG
//    for (const auto& tPos : polygons[0]->vertices)
//    {
//        DebugRender.DrawSphere(tPos, 10.0f, 0xffffff, 255.0f, true);
//    }
//    for (const auto& [st, ed] : polygons[0]->nonAdjacentEdges)
//    {
//        DebugRender.DrawSphere(st, 15.0f, 0xffff00, 255.0f, true);
//        DebugRender.DrawSphere(ed, 15.0f, 0xffff00, 255.0f, true);
//    }
//#endif

    reverse(route.begin(), route.end());
    reverse(polygons.begin(), polygons.end());

    set<pair<Position3D, Position3D>> nonAdjacentEdges;
    for (const auto& polygon: polygons)
    {
        for (const auto& index : polygon->neighbors)
        {
            for (const auto& edge : navMesh_->get().polygons[index].nonAdjacentEdges) {
                nonAdjacentEdges.insert(edge);
            }
        }
    }

    set<Position3D> hitEdgePos;

    Position3D goalPos = route[static_cast<int>(route.size()) - 1];
    Position3D basePos = route[0];

#ifdef _DEBUG

    DebugRender.DrawSphere(goalPos, 10.0f, 0xff00ff, 255.0f, true);
    DebugRender.DrawSphere(basePos, 10.0f, 0xff00ff, 255.0f, true);

#endif

    bool isHit = false;

    for (int i = 1; i < static_cast<int>(route.size()); i++)
    {
        Position3D nextPos = route[i];

        for (const auto& [edgeSt, edgeEd] : nonAdjacentEdges)
        {
            if (DoLinesIntersect(basePos, nextPos, edgeSt, edgeEd))
            {
                isHit = true;

                if (i - 1 > 0)
                {
                    basePos = route[i - 1];
                    optRoute.emplace_back(route[i - 1]);
                }

                break;
            }
        }
    }

    optRoute.emplace_back(goalPos);

    return optRoute;
}

