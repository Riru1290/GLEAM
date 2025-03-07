#include "../../../../Object/Component/Collider/Colliders/PointCollider.h"
#include "../../../../Object/Component/Collider/Colliders/Rect2DCollider.h"
#include "PointRectCollisionDetectFunction.h"

bool PointRectCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // コライダー情報取得
    auto [pointCol, rectCol] = ConvertCollider<PointCollider, Rect2DCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::POINT, COLLISION_SHAPE_TYPE::RECT);

    auto& point = pointCol->GetPointInfo();
    auto& rect = rectCol->GetRectInfo();

    Position2D pointPos = { point.pos.x,point.pos.y };
    Position2D rectPos = { rect.pos.x,rect.pos.y };

    return CollisionPointInBox(pointPos, rectPos, { rect.width,rect.height });
}
