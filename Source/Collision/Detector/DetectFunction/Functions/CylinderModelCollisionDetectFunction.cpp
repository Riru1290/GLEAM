#ifdef _DEBUG
#include "../../../../../External/imgui/imgui.h"
#include "../../../../../External/imgui/imgui_impl_win32.h"
#include "../../../../../External/imgui/imgui_impl_dx12.h"
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../../../Object/Component/Collider/Colliders/CylinderCollider.h"
#include "../../../../Object/Component/Collider/Colliders/ModelCollider.h"
#include "../../../CollisionUtility.h"
#include "CylinderModelCollisionDetectFunction.h"


bool CylinderModelCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // コライダー情報取得
    auto [cylinderCol, modelCol] = ConvertCollider<CylinderCollider, ModelCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::CYLINDER, COLLISION_SHAPE_TYPE::MODEL);

    auto& cylinder = cylinderCol->GetCylinderInfo();
    auto& preCylinder = cylinderCol->GetPreCylinderInfo();
    auto& model = modelCol->GetModelInfo();

    // シリンダーの上部と下部の中心
    Position3D top = cylinder.GetTopPosition();
    Position3D bottom = cylinder.GetBottomPosition();
    Vector3 cylinderAxis = top - bottom;

    // 衝突時データ
    COL_HIT_DATA_POLYGON data;

    const auto& [cylinderMax, cylinderMin] = cylinder.GetAABB();

    // シリンダーとポリゴンの衝突判定
    for (auto& cell : model.cells)
    {
        // セルのAABBを取得
        const auto& [cellMax, cellMin] = cell.GetAABB();

        // AABB同士の交差判定
        if (cylinderMax.x < cellMin.x || cylinderMin.x > cellMax.x ||
            cylinderMax.y < cellMin.y || cylinderMin.y > cellMax.y ||
            cylinderMax.z < cellMin.z || cylinderMin.z > cellMax.z)
        {
            continue; // AABBが交差しない場合はスキップ
        }

        for (auto& polygon : cell.polygons)
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
    }
    hitData.hitData = data;

    return (data.hitPolygonNum > 0);
}
