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
    // �R���C�_�[���擾
    auto [cylinderCol, modelCol] = ConvertCollider<CylinderCollider, ModelCollider>
        (colA, colB, COLLISION_SHAPE_TYPE::CYLINDER, COLLISION_SHAPE_TYPE::MODEL);

    auto& cylinder = cylinderCol->GetCylinderInfo();
    auto& preCylinder = cylinderCol->GetPreCylinderInfo();
    auto& model = modelCol->GetModelInfo();

    // �V�����_�[�̏㕔�Ɖ����̒��S
    Position3D top = cylinder.GetTopPosition();
    Position3D bottom = cylinder.GetBottomPosition();
    Vector3 cylinderAxis = top - bottom;

    // �Փˎ��f�[�^
    COL_HIT_DATA_POLYGON data;

    const auto& [cylinderMax, cylinderMin] = cylinder.GetAABB();

    // �V�����_�[�ƃ|���S���̏Փ˔���
    for (auto& cell : model.cells)
    {
        // �Z����AABB���擾
        const auto& [cellMax, cellMin] = cell.GetAABB();

        // AABB���m�̌�������
        if (cylinderMax.x < cellMin.x || cylinderMin.x > cellMax.x ||
            cylinderMax.y < cellMin.y || cylinderMin.y > cellMax.y ||
            cylinderMax.z < cellMin.z || cylinderMin.z > cellMax.z)
        {
            continue; // AABB���������Ȃ��ꍇ�̓X�L�b�v
        }

        for (auto& polygon : cell.polygons)
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
    }
    hitData.hitData = data;

    return (data.hitPolygonNum > 0);
}
