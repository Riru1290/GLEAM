#ifdef _DEBUG
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../../../Object/Component/Collider/Colliders/LineCollider.h"
#include "../../../../Object/Component/Collider/Colliders/SphereCollider.h"
#include "../../../CollisionUtility.h"
#include "LineSphereCollisionDetectFunction.h"

bool LineSphereCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // �R���C�_�[���擾
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

    // �Փ˓_���v�Z
    float disFromSphereCenterToCollision = sqrt(radiusSquared - disLintToSphere); // ���̕\�ʂ���̋���
    Vector3 lineDir = (line.posB - line.posA).Normalized(); // �����̕���

    // �Փ˓_�������Ɏˉe
    Position3D collisionPoint = neaPosOnLine - lineDir * disFromSphereCenterToCollision;

    // �Փ˃f�[�^�ɕۑ�
    COL_HIT_DATA_LINE data;
    data.neaHitPos = collisionPoint;

    hitData.hitData = data;

#ifdef _DEBUG
    DebugRender.DrawSphere(collisionPoint, 5.0f, 0xff00ff, 255.0f, true);
#endif

    return true; // �Փ˂��Ă���
}
