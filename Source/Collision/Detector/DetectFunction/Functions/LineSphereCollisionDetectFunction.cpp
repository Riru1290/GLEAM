#ifdef _DEBUG
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../../../Object/Component/Collider/Colliders/LineCollider.h"
#include "../../../../Object/Component/Collider/Colliders/SphereCollider.h"
#include "../../../CollisionUtility.h"
#include "LineSphereCollisionDetectFunction.h"

bool LineSphereCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // コライダー情報取得
    auto [lineCol, sphereCol] = ConvertCollider<LineCollider, SphereCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::LINE, COLLISION_SHAPE_TYPE::SPHERE);

    auto& line = lineCol->GetLineInfo();
    auto& sphere = sphereCol->GetSphereInfo();

    Position3D neaPosOnLine = ClosestPointOnSegment(sphere.pos, line.posA, line.posB);

    float disLintToSphere = (neaPosOnLine - sphere.pos).LengthSquared();

    float radiusSquared = sphere.radius * sphere.radius;

    if ((disLintToSphere > radiusSquared))
    {
        return false;
    }
    //return (disLintToSphere <= radiusSquared);

    // 衝突点を計算
    float disFromSphereCenterToCollision = sqrt(radiusSquared - disLintToSphere); // 球体表面からの距離
    Vector3 lineDir = (line.posB - line.posA).Normalized(); // 線分の方向

    // 衝突点を線分上に射影
    Position3D collisionPoint = neaPosOnLine - lineDir * disFromSphereCenterToCollision;

    // 衝突データに保存
    COL_HIT_DATA_LINE data;
    data.neaHitPos = collisionPoint;

    hitData.hitData = data;

#ifdef _DEBUG
    DebugRender.DrawSphere(collisionPoint, 5.0f, 0xff00ff, 255.0f, true);
#endif

    return true; // 衝突している
}
