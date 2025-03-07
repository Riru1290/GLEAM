#pragma once
#include "CollisionHitData.h"

/// @brief 点に最も近い線分上の座標を返す
/// @param point 点座標
/// @param segStart 線分を構成する座標A
/// @param segEnd 線分を構成する座標B
/// @return 点に最も近い線分上の座標
static inline Vector3 ClosestPointOnSegment(const Position3D& point, const Position3D& segStart, const Position3D& segEnd)
{
    Vector3 seg = segEnd - segStart;
    float segLengthSquared = seg.LengthSquared();

    // 線分が点の場合
    if (segLengthSquared == 0.0f) {
        return segStart;
    }

    // 点を線分に射影
    float t = Dot(point - segStart, seg) / segLengthSquared;

    // t をクランプして線分内に収める
    t = clamp(t, 0.0f, 1.0f);

    return segStart + seg * t;
}

static inline bool IsPointInsidePolygon(const Position3D& point, const POLYGON_COL_INFO& polygon)
{
    for (int i = 0; i < 3; ++i) {
        Vector3 edge = polygon.tPos[(i + 1) % 3] - polygon.tPos[i];
        Vector3 toPoint = point - polygon.tPos[i];
        Vector3 cross = Cross(edge, toPoint);

        // 法線との内積が負なら外側にある
        if (Dot(cross, polygon.normal) < 0) {
            return false;
        }
    }
    return true;
}

static inline Vector3 ClosestPointOnSegmentXZ(const Position3D& point, const Position3D& segStart, const Position3D& segEnd)
{
    Vector3 seg = segEnd - segStart;
    seg.y = 0; // XZ平面に投影
    float segLengthSquared = seg.LengthSquared();

    // 線分が点の場合
    if (segLengthSquared == 0.0f) {
        return segStart;
    }

    // 点を線分に射影
    Vector3 toPoint = point - segStart;
    toPoint.y = 0; // XZ平面に投影

    float t = Dot(toPoint, seg) / segLengthSquared;

    // t をクランプして線分内に収める
    t = clamp(t, 0.0f, 1.0f);

    Vector3 closestPoint = segStart + seg * t;
    closestPoint.y = point.y; // 高さを保持
    return closestPoint;
}

static inline bool CheckCylinderPolygonCollision(
    const CYLINDER_COL_INFO& cylinder,
    const POLYGON_COL_INFO& polygon,
    Position3D& hitPoint)
{
    // シリンダーの上下端点と軸
    Position3D top = cylinder.GetTopPosition();
    Position3D bottom = cylinder.GetBottomPosition();
    Vector3 cylinderAxis = top - bottom;

    // ポリゴンの法線
    Vector3 polygonNormal = polygon.normal;

    // 1. 法線がXZ平面に平行かチェック
    if (fabsf(polygonNormal.y) < 1e-6f) {
        // XZ平面に平行な場合、シリンダーの中心軸とXZ平面での交差を確認

        // シリンダーのXZ成分の円とポリゴンのエッジの最近点判定
        for (int i = 0; i < 3; ++i) {
            Position3D edgeStart = polygon.tPos[i];
            Position3D edgeEnd = polygon.tPos[(i + 1) % 3];

            Vector3 closestPoint = ClosestPointOnSegmentXZ(
                bottom, edgeStart, edgeEnd);

            if ((closestPoint - bottom).Length() <= cylinder.radius) {
                hitPoint = closestPoint;
                return true;
            }
        }
        return false; // 衝突なし
    }

    // 2. シリンダー軸とポリゴン平面の交差点を計算
    float topDist = Dot((top - polygon.tPos[0]), polygonNormal);
    float bottomDist = Dot((bottom - polygon.tPos[0]), polygonNormal);

    // 両端点がポリゴン平面の同じ側にある場合、衝突していない
    if (topDist * bottomDist > 0) {
        return false;
    }

    // 平面上の交差点を計算
    float t = fabsf(topDist) / (fabsf(topDist) + fabsf(bottomDist));
    hitPoint = bottom + (cylinderAxis * t);

    // 3. 衝突点がポリゴン内部にあるか確認
    if (IsPointInsidePolygon(hitPoint, polygon)) {
        return true; // 衝突が確定
    }

    // 4. ポリゴンのエッジとの距離を計算し、シリンダー半径を考慮
    for (int i = 0; i < 3; ++i) {
        Position3D edgeStart = polygon.tPos[i];
        Position3D edgeEnd = polygon.tPos[(i + 1) % 3];

        // エッジ上の最近点を計算
        Vector3 closestPoint = ClosestPointOnSegment(hitPoint, edgeStart, edgeEnd);

        // 最近点と交点の距離を計算
        float distance = (hitPoint - closestPoint).Length();

        // 距離が半径以下なら衝突
        if (distance <= cylinder.radius) {
            hitPoint = closestPoint;
            return true;
        }
    }

    return false; // 衝突なし
}

static inline vector<Vector3> CalculateOBBVertices(const Position3D& pos, float width, float height, float depth, const Quaternion& rotation)
{
    vector<Vector3> vertices;
    Vector3 halfExtents(width / 2.0f, height / 2.0f, depth / 2.0f);
    array<Vector3, 8> localVertices = {
        Vector3(-halfExtents.x, -halfExtents.y, -halfExtents.z),
        Vector3(halfExtents.x, -halfExtents.y, -halfExtents.z),
        Vector3(-halfExtents.x,  halfExtents.y, -halfExtents.z),
        Vector3(halfExtents.x,  halfExtents.y, -halfExtents.z),
        Vector3(-halfExtents.x, -halfExtents.y,  halfExtents.z),
        Vector3(halfExtents.x, -halfExtents.y,  halfExtents.z),
        Vector3(-halfExtents.x,  halfExtents.y,  halfExtents.z),
        Vector3(halfExtents.x,  halfExtents.y,  halfExtents.z)
    };

    for (const auto& localVertex : localVertices)
    {
        vertices.push_back(pos + rotation.PosAxis(localVertex));
    }
    return vertices;
}

// 点と線分の距離計算
static inline float DistancePointToLine(const Vector3& point, const Vector3& lineStart, const Vector3& lineEnd)
{
    Vector3 lineDir = (lineEnd - lineStart).Normalized();
    Vector3 projectedPoint = lineStart + lineDir * (Dot((point - lineStart), lineDir));
    return (projectedPoint - point).Length();
}

static inline bool CheckLinePolygonCollision(
    const LINE_COL_INFO& line,
    const POLYGON_COL_INFO& polygon,
    Position3D& hitPos)
{
    const auto& vertices = polygon.tPos;
    const auto& normal = polygon.normal;

    // 線分と平面の衝突判定

    Vector3 vecA = (line.posA - vertices[0]);
    Vector3 vecB = (line.posB - vertices[0]);

    if (Dot(vecA, normal) * Dot(vecB, normal) > 0.0f)
    {
        return false;
    }

    // topからポリゴン平面までの距離
    Vector3 polToTop = (line.posA - polygon.tPos[0]);
    float ptLength = fabsf(Dot(normal, polToTop));

    // bottomからポリゴン平面までの距離
    Vector3 polToBottom = (line.posB - polygon.tPos[0]);
    float pbLength = fabsf(Dot(normal, polToBottom));

    // 内分比からtopから衝突点への距離ベクトルを計算
    float rate = ptLength / (ptLength + pbLength);
    Vector3 topToHitPos = (line.posB - line.posA) * rate;

    // 衝突点計算
    hitPos = line.posA + topToHitPos;

    Vector3 polygonVec[] =
    {
        (polygon.tPos[1] - polygon.tPos[0]),
        (polygon.tPos[2] - polygon.tPos[1]),
        (polygon.tPos[0] - polygon.tPos[2])
    };

    Vector3 PolygonTohitPosVec[] =
    {
        (hitPos - polygon.tPos[1]),
        (hitPos - polygon.tPos[2]),
        (hitPos - polygon.tPos[0])
    };

    bool inPolygon = true;

    for (int i = 0; i < 3; i++)
    {
        Vector3 c = Cross(polygonVec[i], PolygonTohitPosVec[i]);

        if (Dot(c.Normalized(), normal) <= 0)  // 法線との内積を用いて判定
        {
            inPolygon = false;
            break;
        }
    }

    return inPolygon;
}
