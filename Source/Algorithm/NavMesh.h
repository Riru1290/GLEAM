#pragma once
#ifdef _DEBUG
#include "../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../Resource/Model/Model.h"
#include "../Common/Mesh/Meshes/MeshDefault3D.h"
#include "../Common/Mesh/Mesh.h"

/// @brief 点が三角形内に入っているか
/// @param p 点座標
/// @param a 三角形構成点A
/// @param b 三角形構成点B
/// @param c 三角形構成点C
/// @return 点が三角形内に入っているか
static inline bool IsPointInTriangle(const Position3D& p, const Position3D& a, const Position3D& b, const Position3D& c) {
    // ベクトルを計算
    auto v0 = c - a;
    auto v1 = b - a;
    auto v2 = p - a;

    // 内積を計算
    float dot00 = Dot(v0,v0);
    float dot01 = Dot(v0,v1);
    float dot02 = Dot(v0,v2);
    float dot11 = Dot(v1,v1);
    float dot12 = Dot(v1,v2);

    // バリセントリック座標を計算
    float denom = dot00 * dot11 - dot01 * dot01;
    if (denom == 0.0f) return false; // 三角形が退化している場合は false

    float invDenom = 1.0f / denom;
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // u, v が 0～1 の範囲にあり、かつ u + v <= 1 なら三角形内
    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

/// @brief ナビメッシュを構成するポリゴン
struct NavMeshPolygon
{
    [[nodiscard]] NavMeshPolygon() :
        id(0),
        vertices({}),
        neighbors({}),
        nonAdjacentEdges({}),
        adjacentEdges({}),
        center({})
    {

    }

    /// @brief 初期化
    void Init()
    {
        center = { 0.0f, 0.0f, 0.0f };
        for (const auto& vertex : vertices) {
            center.x += vertex.x;
            center.y += vertex.y;
            center.z += vertex.z;
        }
        center.x /= vertices.size();
        center.y /= vertices.size();
        center.z /= vertices.size();

        auto makeEdgeKey = [](const Position3D& v1, const Position3D& v2) {
            return minmax(v1, v2); // 順序を無視するため
            };

        nonAdjacentEdges.clear();
        nonAdjacentEdges.insert(makeEdgeKey(vertices[0], vertices[1]));
        nonAdjacentEdges.insert(makeEdgeKey(vertices[1], vertices[2]));
        nonAdjacentEdges.insert(makeEdgeKey(vertices[2], vertices[0]));
    }

    int id;                                             /// @brief ポリゴンID
    vector<Position3D> vertices;                        /// @brief ポリゴン頂点
    vector<int> neighbors;                              /// @brief 隣接ポリゴンID
    set<pair<Position3D, Position3D>> nonAdjacentEdges; /// @brief 隣接していない辺
    set<pair<Position3D, Position3D>> adjacentEdges;    /// @brief 隣接している辺

    /// @brief ポリゴン中心取得
    /// @return ポリゴン中心
    const Position3D& GetCenter() const {
        return center;
    }

    /// @brief 点がポリゴン内に含まれているか
    /// @param point 点
    /// @return 点がポリゴン内に含まれているか
    bool ContainsPoint(const Position3D& point) const {
        // ポリゴンを三角形に分割して、点がどの三角形の中にあるかを確認する
        for (size_t i = 1; i < vertices.size() - 1; ++i) {
            if (IsPointInTriangle(point, vertices[0], vertices[i], vertices[i + 1])) {
                return true;
            }
        }
        return false;
    }

private:
    Position3D center;  /// @brief 中心
};

/// @brief ナビメッシュ
struct NavMesh
{
    [[nodiscard]] NavMesh() :
        polygons({})
    {
    }

    vector<NavMeshPolygon> polygons;    /// @brief ナビメッシュを構成するポリゴン

    /// @brief 指定座標が属するポリゴンまたは最も近いポリゴンを取得
    /// @param point 指定座標
    /// @return ナビメッシュポリゴン
    const NavMeshPolygon* GetPolygonContainingPoint(const Position3D& point) const {
        const NavMeshPolygon* closestPolygon = nullptr;
        float closestDistance = (numeric_limits<float>::max)();

        for (const auto& poly : polygons) {

            Position3D nea = ClosestPointOnPolygon(point, poly.vertices);
            float distance = (point - nea).Length();

            // 最も近いポリゴンを更新
            if (distance < closestDistance) {
                closestDistance = distance;
                closestPolygon = &poly;
            }
        }
        return closestPolygon; // 最も近いポリゴンを返す
    }

};

/// @brief 点が端を除く線分状にあるかどうかを返す
/// @param point 点座標
/// @param segStart 線分を構成する点A
/// @param segEnd 線分を構成する点B
/// @return 点が端を除く線分状にあるかどうか
static inline bool IsPointOnSegmentExcludeSegmentPoint(const Position3D& point, const Position3D& segStart, const Position3D& segEnd) {
    Vector3 seg = segEnd - segStart;      // 線分のベクトル
    Vector3 toPoint = point - segStart;  // 始点から点へのベクトル

    // 点が直線上にあるか（外積がゼロであるかを確認）
    Vector3 crossProduct = Cross(seg, toPoint);
    if (crossProduct.LengthSquared() > 1e-6f) { // 許容誤差
        return false; // 直線上にない
    }

    // 点が線分の範囲内にあるか（射影値 t が 0 より大きく、1 未満であるかを確認）
    float dotProduct = Dot(toPoint, seg);
    if (dotProduct <= 0.0f || dotProduct >= seg.LengthSquared()) {
        return false; // 範囲外または端点
    }

    return true; // 線分上にある（端点を除く）
}

/// @brief 線分と線分が隣接しているか
/// @param a1 線分Aを構成する点A
/// @param a2 線分Aを構成する点B
/// @param b1 線分Bを構成する点A
/// @param b2 線分Bを構成する点B
/// @return 線分と線分が隣接しているか
static inline bool AreSegmentsCollinear(const Position3D& a1, const Position3D& a2,
    const Position3D& b1, const Position3D& b2) {
    Vector3 dirA = a2 - a1;
    Vector3 dirB = b2 - b1;

    // 平行判定 (外積がゼロベクトルであるか)
    Vector3 cross = Cross(dirA, dirB);
    float epsilon = 1e-6f * (dirA.Length() * dirB.Length());
    if (cross.LengthSquared() > epsilon) {
        return false; // 平行でない
    }

    // 重なり判定 (片方の端点がもう片方の線分上にあるか)
    return (a1 == b1 && a2 == b2) ||
            (IsPointOnSegmentExcludeSegmentPoint(b1, a1, a2) || IsPointOnSegmentExcludeSegmentPoint(b2, a1, a2) ||
             IsPointOnSegmentExcludeSegmentPoint(a1, b1, b2) || IsPointOnSegmentExcludeSegmentPoint(a2, b1, b2));
}



/// @brief ポリゴン同士が隣接しているか
/// @param poly1 ポリゴンA
/// @param poly2 ポリゴンB
/// @return ポリゴン同士が隣接しているか
static inline bool ArePolygonsAdjacent(NavMeshPolygon& poly1,NavMeshPolygon& poly2) {

    auto makeEdgeKey = [](const Position3D& v1, const Position3D& v2) {
        return minmax(v1, v2); // 順序を無視するため
        };

    // poly1 の辺
    array<pair<Position3D, Position3D>, 3> vec1 = {
        makeEdgeKey(poly1.vertices[0], poly1.vertices[1]),
        makeEdgeKey(poly1.vertices[1], poly1.vertices[2]),
        makeEdgeKey(poly1.vertices[2], poly1.vertices[0])
    };

    // poly2 の辺
    array<pair<Position3D, Position3D>, 3> vec2 = {
        makeEdgeKey(poly2.vertices[0], poly2.vertices[1]),
        makeEdgeKey(poly2.vertices[1], poly2.vertices[2]),
        makeEdgeKey(poly2.vertices[2], poly2.vertices[0])
    };

    // 隣接していたかどうか
    bool isAdjacent = false;

    for (const auto& v1 : vec1) {
        for (const auto& v2 : vec2) {

            if (AreSegmentsCollinear(v1.first, v1.second, v2.first, v2.second))
            {
                if (poly1.nonAdjacentEdges.contains(v1))
                {
                    poly1.nonAdjacentEdges.erase(v1);
                }
                isAdjacent = true;
            }
        }
    }

    return isAdjacent;
}

/// @brief モデルを元にナビメッシュを構成
/// @param model モデル
/// @return ナビメッシュ
static inline NavMesh MakeNavMesh(const Model& model) {
    NavMesh navMesh = {};

    const auto& meshes = model.GetMeshes();
    int polygonIdCounter = 0;

    for (const auto& mesh : meshes) {
        auto vertices = static_cast<MeshDefault3D*>(mesh.get())->GetVertices();
        const auto& indices = mesh->GetIndices();

        for (size_t i = 0; i < indices.size(); i += 3) {
            NavMeshPolygon polygon;
            polygon.id = polygonIdCounter++;
            polygon.vertices.push_back({ vertices[indices[i]].pos.x, vertices[indices[i]].pos.y, vertices[indices[i]].pos.z });
            polygon.vertices.push_back({ vertices[indices[i + 1]].pos.x, vertices[indices[i + 1]].pos.y, vertices[indices[i + 1]].pos.z });
            polygon.vertices.push_back({ vertices[indices[i + 2]].pos.x, vertices[indices[i + 2]].pos.y, vertices[indices[i + 2]].pos.z });

            polygon.Init();
            navMesh.polygons.push_back(move(polygon));
        }
    }

    for (auto& poly : navMesh.polygons) {
        for (auto& otherPoly : navMesh.polygons) {
            if (poly.id != otherPoly.id && ArePolygonsAdjacent(poly, otherPoly)) {
                poly.neighbors.push_back(otherPoly.id);
            }
        }
    }

    return navMesh;
}


