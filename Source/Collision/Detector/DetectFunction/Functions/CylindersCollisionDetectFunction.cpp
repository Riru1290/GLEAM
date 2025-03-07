#include "../../../../Object/Component/Collider/Colliders/CylinderCollider.h"
#include "../../../CollisionUtility.h"
#include "CylindersCollisionDetectFunction.h"

bool CylindersCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // �R���C�_�[���擾
    auto [cylinderColA, cylinderColB] = ConvertCollider<CylinderCollider, CylinderCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::CYLINDER, COLLISION_SHAPE_TYPE::CYLINDER);

    auto& cylinderA = cylinderColA->GetCylinderInfo();
    auto& cylinderB = cylinderColB->GetCylinderInfo();


    // �V�����_�[�i��]���l�����Ȃ��j���m�̏Փ˔���

    // Y���̂ݐ����Փ˔���

    float cTopA = cylinderA.GetTopPosition().y;
    float cBottomA = cylinderA.GetBottomPosition().y;

    float cTopB = cylinderB.GetTopPosition().y;
    float cBottomB = cylinderB.GetBottomPosition().y;

    bool isHitLines = (cTopA >= cTopB && cTopB >= cBottomB) || (cTopA >= cBottomB && cBottomB >= cBottomB);

    if (!isHitLines)
    {
        return false;
    }

    // XZ��2�����~�Փ˔���

    Position3D aPos = cylinderA.GetTopPosition();
    Position3D bPos = cylinderB.GetTopPosition();

    aPos.y = 0.0f;
    bPos.y = 0.0f;

    float distance = (aPos - bPos).Length();

    return (distance <= cylinderA.radius + cylinderB.radius);
}
