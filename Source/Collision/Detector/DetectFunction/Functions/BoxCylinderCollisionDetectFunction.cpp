#include "../../../../Object/Component/Collider/Colliders/BoxCollider.h"
#include "../../../../Object/Component/Collider/Colliders/CylinderCollider.h"
#include "../../../CollisionUtility.h"
#include "BoxCylinderCollisionDetectFunction.h"

bool BoxCylinderCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // コライダー情報取得
    auto [boxCol, cylinderCol] = ConvertCollider<BoxCollider, CylinderCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::BOX, COLLISION_SHAPE_TYPE::CYLINDER);

    auto& box = boxCol->GetBoxInfo();
    auto& cylinder = cylinderCol->GetCylinderInfo();

    // 衝突時データ
    COL_HIT_DATA_POLYGON data;

    // AABBによる広域判定
    const auto& [cylinderMax, cylinderMin] = cylinder.GetAABB();
    const auto& [boxMax, boxMin] = box.GetAABB();

    if (boxMax.x < cylinderMin.x || boxMin.x > cylinderMax.x ||
        boxMax.y < cylinderMin.y || boxMin.y > cylinderMax.y ||
        boxMax.z < cylinderMin.z || boxMin.z > cylinderMax.z)
    {
        // AABB範囲外で衝突なし
        return false;
    }

    // 詳細判定: シリンダーの軸方向とボックスのOBBとの距離
    const Vector3 cylinderAxis = Vector3(0.0f, 1.0f, 0.0f); // シリンダーの軸方向
    const Vector3 cylinderTop = cylinder.GetTopPosition();
    const Vector3 cylinderBottom = cylinder.GetBottomPosition();

    // ボックスの頂点を取得
    vector<Vector3> boxVertices = CalculateOBBVertices(box.pos, box.width, box.height, box.depth, box.qua);

    vector<POLYGON_COL_INFO> polygons = GeneratePolygonsFromBox(box);


    for (auto& polygon : polygons)
    {
        // ポリゴンのAABBを取得
        const auto& [polyMax, polyMin] = polygon.GetAABB();

        // AABB同士の交差判定
        if (cylinderMax.x < polyMin.x || cylinderMin.x > polyMax.x ||
            cylinderMax.y < polyMin.y || cylinderMin.y > polyMax.y ||
            cylinderMax.z < polyMin.z || cylinderMin.z > polyMax.z)
        {
            continue; // AABBが交差しない場合はスキップ
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
