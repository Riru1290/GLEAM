#pragma once
#include "CollisionHitData.h"

/// @brief �_�ɍł��߂�������̍��W��Ԃ�
/// @param point �_���W
/// @param segStart �������\��������WA
/// @param segEnd �������\��������WB
/// @return �_�ɍł��߂�������̍��W
static inline Vector3 ClosestPointOnSegment(const Position3D& point, const Position3D& segStart, const Position3D& segEnd)
{
    Vector3 seg = segEnd - segStart;
    float segLengthSquared = seg.LengthSquared();

    // �������_�̏ꍇ
    if (segLengthSquared == 0.0f) {
        return segStart;
    }

    // �_������Ɏˉe
    float t = Dot(point - segStart, seg) / segLengthSquared;

    // t ���N�����v���Đ������Ɏ��߂�
    t = clamp(t, 0.0f, 1.0f);

    return segStart + seg * t;
}

static inline bool IsPointInsidePolygon(const Position3D& point, const POLYGON_COL_INFO& polygon)
{
    for (int i = 0; i < 3; ++i) {
        Vector3 edge = polygon.tPos[(i + 1) % 3] - polygon.tPos[i];
        Vector3 toPoint = point - polygon.tPos[i];
        Vector3 cross = Cross(edge, toPoint);

        // �@���Ƃ̓��ς����Ȃ�O���ɂ���
        if (Dot(cross, polygon.normal) < 0) {
            return false;
        }
    }
    return true;
}

static inline Vector3 ClosestPointOnSegmentXZ(const Position3D& point, const Position3D& segStart, const Position3D& segEnd)
{
    Vector3 seg = segEnd - segStart;
    seg.y = 0; // XZ���ʂɓ��e
    float segLengthSquared = seg.LengthSquared();

    // �������_�̏ꍇ
    if (segLengthSquared == 0.0f) {
        return segStart;
    }

    // �_������Ɏˉe
    Vector3 toPoint = point - segStart;
    toPoint.y = 0; // XZ���ʂɓ��e

    float t = Dot(toPoint, seg) / segLengthSquared;

    // t ���N�����v���Đ������Ɏ��߂�
    t = clamp(t, 0.0f, 1.0f);

    Vector3 closestPoint = segStart + seg * t;
    closestPoint.y = point.y; // ������ێ�
    return closestPoint;
}

static inline bool CheckCylinderPolygonCollision(
    const CYLINDER_COL_INFO& cylinder,
    const POLYGON_COL_INFO& polygon,
    Position3D& hitPoint)
{
    // �V�����_�[�̏㉺�[�_�Ǝ�
    Position3D top = cylinder.GetTopPosition();
    Position3D bottom = cylinder.GetBottomPosition();
    Vector3 cylinderAxis = top - bottom;

    // �|���S���̖@��
    Vector3 polygonNormal = polygon.normal;

    // 1. �@����XZ���ʂɕ��s���`�F�b�N
    if (fabsf(polygonNormal.y) < 1e-6f) {
        // XZ���ʂɕ��s�ȏꍇ�A�V�����_�[�̒��S����XZ���ʂł̌������m�F

        // �V�����_�[��XZ�����̉~�ƃ|���S���̃G�b�W�̍ŋߓ_����
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
        return false; // �Փ˂Ȃ�
    }

    // 2. �V�����_�[���ƃ|���S�����ʂ̌����_���v�Z
    float topDist = Dot((top - polygon.tPos[0]), polygonNormal);
    float bottomDist = Dot((bottom - polygon.tPos[0]), polygonNormal);

    // ���[�_���|���S�����ʂ̓������ɂ���ꍇ�A�Փ˂��Ă��Ȃ�
    if (topDist * bottomDist > 0) {
        return false;
    }

    // ���ʏ�̌����_���v�Z
    float t = fabsf(topDist) / (fabsf(topDist) + fabsf(bottomDist));
    hitPoint = bottom + (cylinderAxis * t);

    // 3. �Փ˓_���|���S�������ɂ��邩�m�F
    if (IsPointInsidePolygon(hitPoint, polygon)) {
        return true; // �Փ˂��m��
    }

    // 4. �|���S���̃G�b�W�Ƃ̋������v�Z���A�V�����_�[���a���l��
    for (int i = 0; i < 3; ++i) {
        Position3D edgeStart = polygon.tPos[i];
        Position3D edgeEnd = polygon.tPos[(i + 1) % 3];

        // �G�b�W��̍ŋߓ_���v�Z
        Vector3 closestPoint = ClosestPointOnSegment(hitPoint, edgeStart, edgeEnd);

        // �ŋߓ_�ƌ�_�̋������v�Z
        float distance = (hitPoint - closestPoint).Length();

        // ���������a�ȉ��Ȃ�Փ�
        if (distance <= cylinder.radius) {
            hitPoint = closestPoint;
            return true;
        }
    }

    return false; // �Փ˂Ȃ�
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

// �_�Ɛ����̋����v�Z
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

    // �����ƕ��ʂ̏Փ˔���

    Vector3 vecA = (line.posA - vertices[0]);
    Vector3 vecB = (line.posB - vertices[0]);

    if (Dot(vecA, normal) * Dot(vecB, normal) > 0.0f)
    {
        return false;
    }

    // top����|���S�����ʂ܂ł̋���
    Vector3 polToTop = (line.posA - polygon.tPos[0]);
    float ptLength = fabsf(Dot(normal, polToTop));

    // bottom����|���S�����ʂ܂ł̋���
    Vector3 polToBottom = (line.posB - polygon.tPos[0]);
    float pbLength = fabsf(Dot(normal, polToBottom));

    // �����䂩��top����Փ˓_�ւ̋����x�N�g�����v�Z
    float rate = ptLength / (ptLength + pbLength);
    Vector3 topToHitPos = (line.posB - line.posA) * rate;

    // �Փ˓_�v�Z
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

        if (Dot(c.Normalized(), normal) <= 0)  // �@���Ƃ̓��ς�p���Ĕ���
        {
            inPolygon = false;
            break;
        }
    }

    return inPolygon;
}
