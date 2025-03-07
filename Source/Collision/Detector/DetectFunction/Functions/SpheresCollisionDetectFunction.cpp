#include "../../../../Object/Component/Collider/Colliders/SphereCollider.h"
#include "../../../CollisionUtility.h"
#include "SpheresCollisionDetectFunction.h"

bool SpheresCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // コライダー情報取得
    auto [sphereColA, sphereColB] = ConvertCollider<SphereCollider, SphereCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::SPHERE, COLLISION_SHAPE_TYPE::SPHERE);

    auto& sphereA = sphereColA->GetSphereInfo();
    auto& sphereB = sphereColB->GetSphereInfo();

    float distance = (sphereA.pos - sphereB.pos).Length();

    return (distance <= sphereA.radius + sphereB.radius);
}
