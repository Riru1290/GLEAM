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
        Position3D pos;         // �m�[�h�ԋ������Z�o����ۂ̍��W
        float distanceCost;	    // ���ۂ̋����R�X�g
        float heuristicCost;	// �q���[���X�e�B�b�N�R�X�g
        float totalCost;	    // �g�[�^���R�X�g

        int cameFromID;         // �e�m�[�h�̃|���S��ID
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

    // �J�n�n�_�ƏI���n�_��������|���S�������
    const NavMeshPolygon* startPoly = navMesh_->get().GetPolygonContainingPoint(startPos);
    const NavMeshPolygon* goalPoly = navMesh_->get().GetPolygonContainingPoint(goalPos);

    if (!startPoly || !goalPoly) {
        return {}; // �����ȊJ�n�܂��͏I���|���S��
    }

    // �o�H
    vector<Position3D> route;
    set<int> path;

    if (startPoly->id == goalPoly->id) {
        route.emplace_back(ClosestPointOnPolygon(goalPos, goalPoly->vertices));
        return route;
    }

    // �I�[�v�����X�g(�T�����̃m�[�h)
    unordered_map<int, Node> openList;

    // �T�����I�������m�[�h
    unordered_map<int, Node> closeList;

    // �J�n�m�[�h��ݒ�
    Node startNode;
    startNode.polygon = startPoly;
    startNode.pos = ClosestPointOnPolygon(startPos, startPoly->vertices);
    startNode.pos += (startPoly->GetCenter() - startNode.pos).Normalized() * 0.1f;
    startNode.distanceCost = 0.0f;
    startNode.heuristicCost = 0.0f;
    startNode.totalCost = 0.0f;
    startNode.cameFromID = -1;
    openList[startPoly->id] = startNode;

    // �I�[�v�����X�g�̒�����ł��g�[�^���R�X�g���Ⴂ�m�[�h��ID��Ԃ��֐�
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

    // �I�[�v�����X�g����ɂȂ�܂ŌJ��Ԃ�
    while (!openList.empty())
    {
        // �I�[�v�����X�g�̒�����ł��g�[�^���R�X�g���Ⴂ�m�[�h��I��
        int currentID = GetSmallestCostNodeID();
        const Node* currentNode = &openList[currentID];


        // �N���[�Y���X�g�ɒǉ�
        closeList.emplace(currentID, *currentNode);

        // �������擾�����m�[�h���S�[���m�[�h�ƍ��v���Ă���΃S�[���Ƃ���
        if (currentID == goalPoly->id)
        {
            vector<const NavMeshPolygon*> routePolygons;

            Position3D goal = ClosestPointOnPolygon(goalPos, goalPoly->vertices);

            // ������ɏ��Ȃ����߂̃I�t�Z�b�g
            goal += (goalPoly->GetCenter() - goal).Normalized() * 0.1f;

            route.emplace_back(goal);

            // �e�m�[�h�����ǂ�
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

        // �אڃm�[�h������
        for (const auto& neiNodeID : currentNode->polygon->neighbors)
        {
            // �N���[�Y���X�g�ɒǉ����ꂽ�m�[�h�̏ꍇ�X�L�b�v
            bool isCloseID = false;
            for(const auto& [closeID,node] : closeList)
            {
                if (closeID == neiNodeID)
                {
                    isCloseID = true;
                }
            }
            if (isCloseID)continue;

            // �אڃm�[�h
            Node neiNode;
            neiNode.polygon = &navMesh_->get().polygons[neiNodeID];


            // �e�m�[�h���W����ł��߂��|���S����̍��W���m�[�h�̍��W�Ƃ���
            neiNode.pos = ClosestPointOnPolygon(currentNode->pos, neiNode.polygon->vertices);

            // ������ɏ��Ȃ����߂̃I�t�Z�b�g
            neiNode.pos += (neiNode.polygon->GetCenter() - neiNode.pos).Normalized() * 0.1f;

            // �����R�X�g�v�Z
            neiNode.distanceCost = currentNode->distanceCost +
                (neiNode.pos - currentNode->pos).Length();
            // �q���[���X�e�B�b�N�R�X�g�v�Z
            neiNode.heuristicCost = (goalPos - neiNode.pos).Length();

            // �g�[�^���R�X�g�v�Z
            neiNode.totalCost = neiNode.distanceCost + neiNode.heuristicCost;

            // �eID��ݒ�
            neiNode.cameFromID = currentID;

            // �I�[�v�����X�g�ɒǉ�����Ă���m�[�h�̏ꍇ�R�X�g���r����
            if (openList.contains(neiNodeID))
            {
                if (openList[neiNodeID].totalCost > neiNode.totalCost)
                {
                    openList[neiNodeID] = neiNode;
                }
            }
            else
            {
                // ���̂܂ܒǉ�
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
        return route; // ���͂������̏ꍇ�A���̂܂ܕԂ�
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

