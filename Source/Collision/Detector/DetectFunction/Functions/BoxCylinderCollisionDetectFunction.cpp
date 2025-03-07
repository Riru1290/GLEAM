#include "../../../../Object/Component/Collider/Colliders/BoxCollider.h"
#include "../../../../Object/Component/Collider/Colliders/CylinderCollider.h"
#include "../../../CollisionUtility.h"
#include "BoxCylinderCollisionDetectFunction.h"

bool BoxCylinderCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // �R���C�_�[���擾
    auto [boxCol, cylinderCol] = ConvertCollider<BoxCollider, CylinderCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::BOX, COLLISION_SHAPE_TYPE::CYLINDER);

    auto& box = boxCol->GetBoxInfo();
    auto& cylinder = cylinderCol->GetCylinderInfo();

    // �Փˎ��f�[�^
    COL_HIT_DATA_POLYGON data;

    // AABB�ɂ��L�攻��
    const auto& [cylinderMax, cylinderMin] = cylinder.GetAABB();
    const auto& [boxMax, boxMin] = box.GetAABB();

    if (boxMax.x < cylinderMin.x || boxMin.x > cylinderMax.x ||
        boxMax.y < cylinderMin.y || boxMin.y > cylinderMax.y ||
        boxMax.z < cylinderMin.z || boxMin.z > cylinderMax.z)
    {
        // AABB�͈͊O�ŏՓ˂Ȃ�
        return false;
    }

    // �ڍה���: �V�����_�[�̎������ƃ{�b�N�X��OBB�Ƃ̋���
    const Vector3 cylinderAxis = Vector3(0.0f, 1.0f, 0.0f); // �V�����_�[�̎�����
    const Vector3 cylinderTop = cylinder.GetTopPosition();
    const Vector3 cylinderBottom = cylinder.GetBottomPosition();

    // �{�b�N�X�̒��_���擾
    vector<Vector3> boxVertices = CalculateOBBVertices(box.pos, box.width, box.height, box.depth, box.qua);

    vector<POLYGON_COL_INFO> polygons = GeneratePolygonsFromBox(box);


    for (auto& polygon : polygons)
    {
        // �|���S����AABB���擾
        const auto& [polyMax, polyMin] = polygon.GetAABB();

        // AABB���m�̌�������
        if (cylinderMax.x < polyMin.x || cylinderMin.x > polyMax.x ||
            cylinderMax.y < polyMin.y || cylinderMin.y > polyMax.y ||
            cylinderMax.z < polyMin.z || cylinderMin.z > polyMax.z)
        {
            continue; // AABB���������Ȃ��ꍇ�̓X�L�b�v
        }

        Position3D hitPos;
        if (CheckCylinderPolygonCollision(cylinder, polygon, hitPos))
        {
            data.hitPolygonNum++;
            data.hitPolygons.emplace_back(polygon);
        }
    }

    hitData.hitData = data;

    return (data.hitPolygonNum > 0);
}
