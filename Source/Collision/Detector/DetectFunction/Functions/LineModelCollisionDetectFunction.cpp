#ifdef _DEBUG
#include "../../../../../External/imgui/imgui.h"
#include "../../../../../External/imgui/imgui_impl_win32.h"
#include "../../../../../External/imgui/imgui_impl_dx12.h"
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../../../Object/Component/Collider/Colliders/LineCollider.h"
#include "../../../../Object/Component/Collider/Colliders/ModelCollider.h"
#include "../../../CollisionUtility.h"
#include "LineModelCollisionDetectFunction.h"

bool LineModelCollisionDetectFunction::Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
    // コライダー情報取得
    auto [lineCol, modelCol] = ConvertCollider<LineCollider, ModelCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::LINE, COLLISION_SHAPE_TYPE::MODEL);

    auto& line = lineCol->GetLineInfo();
    auto& model = modelCol->GetModelInfo();

    bool hasCollision = false;
    float closestDistance = FLT_MAX;

    // 線分のAABB
    const auto& [lineMax, lineMin] = line.GetAABB();


    // 衝突時データ
    COL_HIT_DATA_POLYGON data;

    int hitCellNum = 0;

    // モデルの各ポリゴンと衝突判定
    for (const auto& cell : model.cells)
    {
        // ポリゴンのAABBを取得
        const auto& [cellMax, cellMin] = cell.GetAABB();




        // AABB同士の交差判定
        if (lineMax.x < cellMin.x || lineMin.x > cellMax.x ||
            lineMax.y < cellMin.y || lineMin.y > cellMax.y ||
            lineMax.z < cellMin.z || lineMin.z > cellMax.z)
        {
            continue; // AABBが交差しない場合はスキップ
        }

//#ifdef _DEBUG
//        int color = 0xffffff;
//        switch (hitCellNum % 8)
//        {
//        case 0:
//            color = 0xff0000;
//            break;
//        case 1:
//            color = 0xffff00;
//            break;
//        case 2:
//            color = 0x0000ff;
//            break;
//        case 3:
//            color = 0x00ffff;
//            break;
//        case 4:
//            color = 0x00ff00;
//            break;
//        case 5:
//            color = 0xff00ff;
//            break;
//        case 6:
//            color = 0xf0f0f0;
//            break;
//        case 7:
//            color = 0xff00f0;
//            break;
//        }
//
//        DebugRender.DrawBox(cellMin, cellMax, color, 255.0f, true);
//#endif
        hitCellNum++;


        for (const auto& polygon : cell.polygons)
        {
            // ポリゴンのAABBを取得
            const auto& [polyMax, polyMin] = polygon.GetAABB();

            // AABB同士の交差判定
            if (lineMax.x < polyMin.x || lineMin.x > polyMax.x ||
                lineMax.y < polyMin.y || lineMin.y > polyMax.y ||
                lineMax.z < polyMin.z || lineMin.z > polyMax.z)
            {
                continue; // AABBが交差しない場合はスキップ
            }

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

            //const auto& vertices = polygon.tPos;
            //const auto& normal = polygon.normal;

            //// 線分と平面の衝突判定

            //Vector3 vecA = (line.posA - vertices[0]);
            //Vector3 vecB = (line.posB - vertices[0]);

            //if (Dot(vecA, normal) * Dot(vecB, normal) > 0.0f)
            //{
            //    continue;
            //}

            //// topからポリゴン平面までの距離
            //Vector3 polToTop = (line.posA - polygon.tPos[0]);
            //float ptLength = fabsf(Dot(normal, polToTop));

            //// bottomからポリゴン平面までの距離
            //Vector3 polToBottom = (line.posB - polygon.tPos[0]);
            //float pbLength = fabsf(Dot(normal, polToBottom));

            //// 内分比からtopから衝突点への距離ベクトルを計算
            //float rate = ptLength / (ptLength + pbLength);
            //Vector3 topToHitPos = (line.posB - line.posA) * rate;

            //// 衝突点計算
            //Position3D hitPos = line.posA + topToHitPos;

            //Vector3 polygonVec[] =
            //{
            //    (polygon.tPos[1] - polygon.tPos[0]),
            //    (polygon.tPos[2] - polygon.tPos[1]),
            //    (polygon.tPos[0] - polygon.tPos[2])
            //};

            //Vector3 PolygonTohitPosVec[] =
            //{
            //    (hitPos - polygon.tPos[1]),
            //    (hitPos - polygon.tPos[2]),
            //    (hitPos - polygon.tPos[0])
            //};

            //bool inPolygon = true;

            //for (int i = 0; i < 3; i++)
            //{
            //    Vector3 c = Cross(polygonVec[i], PolygonTohitPosVec[i]);

            //    if (Dot(c.Normalized(), normal) <= 0)  // 法線との内積を用いて判定
            //    {
            //        inPolygon = false;
            //        break;
            //    }
            //}

            //if (inPolygon)
            //{


            //}
        }
    }

    hitData.hitData = data;

    return hasCollision;
}
