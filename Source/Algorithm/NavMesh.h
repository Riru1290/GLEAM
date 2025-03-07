#pragma once
#ifdef _DEBUG
#include "../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../Resource/Model/Model.h"
#include "../Common/Mesh/Meshes/MeshDefault3D.h"
#include "../Common/Mesh/Mesh.h"

/// @brief �_���O�p�`���ɓ����Ă��邩
/// @param p �_���W
/// @param a �O�p�`�\���_A
/// @param b �O�p�`�\���_B
/// @param c �O�p�`�\���_C
/// @return �_���O�p�`���ɓ����Ă��邩
static inline bool IsPointInTriangle(const Position3D& p, const Position3D& a, const Position3D& b, const Position3D& c) {
    // �x�N�g�����v�Z
    auto v0 = c - a;
    auto v1 = b - a;
    auto v2 = p - a;

    // ���ς��v�Z
    float dot00 = Dot(v0,v0);
    float dot01 = Dot(v0,v1);
    float dot02 = Dot(v0,v2);
    float dot11 = Dot(v1,v1);
    float dot12 = Dot(v1,v2);

    // �o���Z���g���b�N���W���v�Z
    float denom = dot00 * dot11 - dot01 * dot01;
    if (denom == 0.0f) return false; // �O�p�`���މ����Ă���ꍇ�� false

    float invDenom = 1.0f / denom;
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // u, v �� 0�`1 �͈̔͂ɂ���A���� u + v <= 1 �Ȃ�O�p�`��
    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

/// @brief �i�r���b�V�����\������|���S��
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

    /// @brief ������
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
            return minmax(v1, v2); // �����𖳎����邽��
            };

        nonAdjacentEdges.clear();
        nonAdjacentEdges.insert(makeEdgeKey(vertices[0], vertices[1]));
        nonAdjacentEdges.insert(makeEdgeKey(vertices[1], vertices[2]));
        nonAdjacentEdges.insert(makeEdgeKey(vertices[2], vertices[0]));
    }

    int id;                                             /// @brief �|���S��ID
    vector<Position3D> vertices;                        /// @brief �|���S�����_
    vector<int> neighbors;                              /// @brief �אڃ|���S��ID
    set<pair<Position3D, Position3D>> nonAdjacentEdges; /// @brief �אڂ��Ă��Ȃ���
    set<pair<Position3D, Position3D>> adjacentEdges;    /// @brief �אڂ��Ă����

    /// @brief �|���S�����S�擾
    /// @return �|���S�����S
    const Position3D& GetCenter() const {
        return center;
    }

    /// @brief �_���|���S�����Ɋ܂܂�Ă��邩
    /// @param point �_
    /// @return �_���|���S�����Ɋ܂܂�Ă��邩
    bool ContainsPoint(const Position3D& point) const {
        // �|���S�����O�p�`�ɕ������āA�_���ǂ̎O�p�`�̒��ɂ��邩���m�F����
        for (size_t i = 1; i < vertices.size() - 1; ++i) {
            if (IsPointInTriangle(point, vertices[0], vertices[i], vertices[i + 1])) {
                return true;
            }
        }
        return false;
    }

private:
    Position3D center;  /// @brief ���S
};

/// @brief �i�r���b�V��
struct NavMesh
{
    [[nodiscard]] NavMesh() :
        polygons({})
    {
    }

    vector<NavMeshPolygon> polygons;    /// @brief �i�r���b�V�����\������|���S��

    /// @brief �w����W��������|���S���܂��͍ł��߂��|���S�����擾
    /// @param point �w����W
    /// @return �i�r���b�V���|���S��
    const NavMeshPolygon* GetPolygonContainingPoint(const Position3D& point) const {
        const NavMeshPolygon* closestPolygon = nullptr;
        float closestDistance = (numeric_limits<float>::max)();

        for (const auto& poly : polygons) {

            Position3D nea = ClosestPointOnPolygon(point, poly.vertices);
            float distance = (point - nea).Length();

            // �ł��߂��|���S�����X�V
            if (distance < closestDistance) {
                closestDistance = distance;
                closestPolygon = &poly;
            }
        }
        return closestPolygon; // �ł��߂��|���S����Ԃ�
    }

};

/// @brief �_���[������������ɂ��邩�ǂ�����Ԃ�
/// @param point �_���W
/// @param segStart �������\������_A
/// @param segEnd �������\������_B
/// @return �_���[������������ɂ��邩�ǂ���
static inline bool IsPointOnSegmentExcludeSegmentPoint(const Position3D& point, const Position3D& segStart, const Position3D& segEnd) {
    Vector3 seg = segEnd - segStart;      // �����̃x�N�g��
    Vector3 toPoint = point - segStart;  // �n�_����_�ւ̃x�N�g��

    // �_��������ɂ��邩�i�O�ς��[���ł��邩���m�F�j
    Vector3 crossProduct = Cross(seg, toPoint);
    if (crossProduct.LengthSquared() > 1e-6f) { // ���e�덷
        return false; // ������ɂȂ�
    }

    // �_�������͈͓̔��ɂ��邩�i�ˉe�l t �� 0 ���傫���A1 �����ł��邩���m�F�j
    float dotProduct = Dot(toPoint, seg);
    if (dotProduct <= 0.0f || dotProduct >= seg.LengthSquared()) {
        return false; // �͈͊O�܂��͒[�_
    }

    return true; // ������ɂ���i�[�_�������j
}

/// @brief �����Ɛ������אڂ��Ă��邩
/// @param a1 ����A���\������_A
/// @param a2 ����A���\������_B
/// @param b1 ����B���\������_A
/// @param b2 ����B���\������_B
/// @return �����Ɛ������אڂ��Ă��邩
static inline bool AreSegmentsCollinear(const Position3D& a1, const Position3D& a2,
    const Position3D& b1, const Position3D& b2) {
    Vector3 dirA = a2 - a1;
    Vector3 dirB = b2 - b1;

    // ���s���� (�O�ς��[���x�N�g���ł��邩)
    Vector3 cross = Cross(dirA, dirB);
    float epsilon = 1e-6f * (dirA.Length() * dirB.Length());
    if (cross.LengthSquared() > epsilon) {
        return false; // ���s�łȂ�
    }

    // �d�Ȃ蔻�� (�Е��̒[�_�������Е��̐�����ɂ��邩)
    return (a1 == b1 && a2 == b2) ||
            (IsPointOnSegmentExcludeSegmentPoint(b1, a1, a2) || IsPointOnSegmentExcludeSegmentPoint(b2, a1, a2) ||
             IsPointOnSegmentExcludeSegmentPoint(a1, b1, b2) || IsPointOnSegmentExcludeSegmentPoint(a2, b1, b2));
}



/// @brief �|���S�����m���אڂ��Ă��邩
/// @param poly1 �|���S��A
/// @param poly2 �|���S��B
/// @return �|���S�����m���אڂ��Ă��邩
static inline bool ArePolygonsAdjacent(NavMeshPolygon& poly1,NavMeshPolygon& poly2) {

    auto makeEdgeKey = [](const Position3D& v1, const Position3D& v2) {
        return minmax(v1, v2); // �����𖳎����邽��
        };

    // poly1 �̕�
    array<pair<Position3D, Position3D>, 3> vec1 = {
        makeEdgeKey(poly1.vertices[0], poly1.vertices[1]),
        makeEdgeKey(poly1.vertices[1], poly1.vertices[2]),
        makeEdgeKey(poly1.vertices[2], poly1.vertices[0])
    };

    // poly2 �̕�
    array<pair<Position3D, Position3D>, 3> vec2 = {
        makeEdgeKey(poly2.vertices[0], poly2.vertices[1]),
        makeEdgeKey(poly2.vertices[1], poly2.vertices[2]),
        makeEdgeKey(poly2.vertices[2], poly2.vertices[0])
    };

    // �אڂ��Ă������ǂ���
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

/// @brief ���f�������Ƀi�r���b�V�����\��
/// @param model ���f��
/// @return �i�r���b�V��
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


