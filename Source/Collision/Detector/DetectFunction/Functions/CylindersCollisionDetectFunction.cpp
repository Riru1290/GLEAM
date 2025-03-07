#include "../../../../Object/Component/Collider/Colliders/CylinderCollider.h"
#include "../../../CollisionUtility.h"
#include "CylindersCollisionDetectFunction.h"

bool CylindersCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // コライダー情報取得
    auto [cylinderColA, cylinderColB] = ConvertCollider<CylinderCollider, CylinderCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::CYLINDER, COLLISION_SHAPE_TYPE::CYLINDER);

    auto& cylinderA = cylinderColA->GetCylinderInfo();
    auto& cylinderB = cylinderColB->GetCylinderInfo();


    // シリンダー（回転を考慮しない）同士の衝突判定

    // Y軸のみ線分衝突判定

    float cTopA = cylinderA.GetTopPosition().y;
    float cBottomA = cylinderA.GetBottomPosition().y;

    float cTopB = cylinderB.GetTopPosition().y;
    float cBottomB = cylinderB.GetBottomPosition().y;

    bool isHitLines = (cTopA >= cTopB && cTopB >= cBottomB) || (cTopA >= cBottomB && cBottomB >= cBottomB);

    if (!isHitLines)
    {
        return false;
    }

    // XZ軸2次元円衝突判定

    Position3D aPos = cylinderA.GetTopPosition();
    Position3D bPos = cylinderB.GetTopPosition();

    aPos.y = 0.0f;
    bPos.y = 0.0f;

    float distance = (aPos - bPos).Length();

    return (distance <= cylinderA.radius + cylinderB.radius);
}
