#include "../../../../Object/Component/Collider/Colliders/LineCollider.h"
#include "../../../../Object/Component/Collider/Colliders/BoxCollider.h"
#include "../../../CollisionUtility.h"
#include "LineBoxCollisionDetectFunction.h"

bool LineBoxCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // �R���C�_�[���擾
    auto [lineCol, boxCol] = ConvertCollider<LineCollider, BoxCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::LINE, COLLISION_SHAPE_TYPE::BOX);

    auto& line = lineCol->GetLineInfo();
    auto& box = boxCol->GetBoxInfo();

    const auto& [lineMax, lineMin] = line.GetAABB();
    const auto& [boxMax, boxMin] = box.GetAABB();

    //// AABB���m�̌�������
    //if (lineMax.x < boxMin.x || lineMin.x > boxMax.x ||
    //    lineMax.y < boxMin.y || lineMin.y > boxMax.y ||
    //    lineMax.z < boxMin.z || lineMin.z > boxMax.z)
    //{
    //    return false; // AABB���������Ȃ��ꍇ�̓X�L�b�v
    //}

    // �Փˎ��f�[�^
    COL_HIT_DATA_POLYGON data;

    bool hasCollision = false;
    float closestDistance = FLT_MAX;

    vector<POLYGON_COL_INFO> polygons = GeneratePolygonsFromBox(box);

    for (const auto& polygon : polygons)
    {
        //// �|���S����AABB���擾
        //const auto& [polyMax, polyMin] = polygon.GetAABB();

        //// AABB���m�̌�������
        //if (lineMax.x < polyMin.x || lineMin.x > polyMax.x ||
        //    lineMax.y < polyMin.y || lineMin.y > polyMax.y ||
        //    lineMax.z < polyMin.z || lineMin.z > polyMax.z)
        //{
        //    continue; // AABB���������Ȃ��ꍇ�̓X�L�b�v
        //}

        Position3D hitPos;
        if (CheckLinePolygonCollision(line, polygon, hitPos))
        {
            float distance = (hitPos - line.posA).Length();
            if (distance < closestDistance)
            {
                data.neaPolygon = polygon;
                data.neaHitPos = hitPos;
                closestDistance = distance;
            }
            hasCollision = true;
        }
    }

    hitData.hitData = data;

    return hasCollision;
}
