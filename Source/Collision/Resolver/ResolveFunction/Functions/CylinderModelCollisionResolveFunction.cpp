#ifdef _DEBUG
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../../../Object/Component/Collider/Colliders/CylinderCollider.h"
#include "../../../../Object/Component/Collider/Colliders/ModelCollider.h"
#include "CylinderModelCollisionResolveFunction.h"

void CylinderModelCollisionResolveFunction::Resolve(COL_HIT_DATA data)
{
	if (!holds_alternative<COL_HIT_DATA_POLYGON>(data.hitData) ||
		data.targetCollider.lock()->IsStatic() && data.selfCollider.lock()->IsStatic())return;

	// �R���C�_�[���擾
	auto [cylinderCol, modelCol] = ConvertCollider<CylinderCollider, ModelCollider>
		(data.targetCollider, data.selfCollider, COLLISION_SHAPE_TYPE::CYLINDER, COLLISION_SHAPE_TYPE::MODEL);

	if (cylinderCol->IsStatic())
	{
		ResolveStaticCylinder(data);
	}
	else if (modelCol->IsStatic())
	{
		ResolveStaticModel(data);
	}
}

void CylinderModelCollisionResolveFunction::ResolveStaticCylinder(COL_HIT_DATA data)
{
}

void CylinderModelCollisionResolveFunction::ResolveStaticModel(COL_HIT_DATA data)
{
	auto hitData = get<COL_HIT_DATA_POLYGON>(data.hitData);

	// �R���C�_�[���擾
	auto [cylinderCol, modelCol] = ConvertCollider<CylinderCollider, ModelCollider>
		(data.targetCollider, data.selfCollider, COLLISION_SHAPE_TYPE::CYLINDER, COLLISION_SHAPE_TYPE::MODEL);

	auto& cylinder = cylinderCol->GetCylinderInfo();
	auto& preCylinder = cylinderCol->GetPreCylinderInfo();

	// �V�����_�[���W
	Position3D cylinderPos = cylinder.pos;
	Position3D cylinderPrePos = preCylinder.pos;

	// �|���S����@���������Ƃɕ���
	vector<POLYGON_COL_INFO> upNormalPolygons;		// Y=  1.0f���� ��
	vector<POLYGON_COL_INFO> downNormalPolygons;	// Y= -1.0f���� ��
	vector<POLYGON_COL_INFO> sideNormalPolygons;	// Y=  0.0f���� ��
	vector<POLYGON_COL_INFO> otherNormalPolygons;	// ���̑� �΂�

	for (auto& polygon : hitData.hitPolygons)
	{
		Position3D center = {};
		for (auto pos : polygon.tPos)
		{
			center += pos;
		}
		center /= 3.0f;

		if (polygon.normal.y == 1.0f)
		{
			upNormalPolygons.emplace_back(polygon);
		}
		else if (polygon.normal.y == -1.0f)
		{
			downNormalPolygons.emplace_back(polygon);
		}
		else/* (polygon.normal.y == 0.0f)*/
		{
			sideNormalPolygons.emplace_back(polygon);
		}
		//else
		//{
		//	otherNormalPolygons.emplace_back(polygon);
		//}
	}

	// ������
	optional<POLYGON_COL_INFO> sideNeaPolygon = {};
	float nearDis = 0.0f;
	for (const auto& polygon : sideNormalPolygons)
	{
		// �N�����v�p
		// X���W
		float Xmin = polygon.tPos[0].x;
		float Xmax = polygon.tPos[0].x;
		// Z���W
		float Zmin = polygon.tPos[0].z;
		float Zmax = polygon.tPos[0].z;

		for (auto p : polygon.tPos)
		{
			// X���W
			if (Xmin > p.x)Xmin = p.x;
			if (Xmax < p.x)Xmax = p.x;

			// Z���W
			if (Zmin > p.z)Zmin = p.z;
			if (Zmax < p.z)Zmax = p.z;
		}

		if (!sideNeaPolygon.has_value())
		{
			sideNeaPolygon = polygon;

			Position3D nearPos = FindNearestPointOnPlane(cylinderPos, polygon.tPos[0], polygon.normal);
			// �|���S���̒�����ŃN�����v
			nearPos.x = clamp(nearPos.x, Xmin, Xmax);
			nearPos.z = clamp(nearPos.z, Zmin, Zmax);

			nearDis = (nearPos - cylinderPos).Length();

			continue;
		}

		Position3D nearPos = FindNearestPointOnPlane(cylinderPos, polygon.tPos[0], polygon.normal);

		// �|���S���̒�����ŃN�����v
		nearPos.x = clamp(nearPos.x, Xmin, Xmax);
		nearPos.z = clamp(nearPos.z, Zmin, Zmax);

		float dis = (nearPos - cylinderPos).Length();

		if (dis < nearDis)
		{
			sideNeaPolygon = polygon;
			nearDis = dis;
		}
	}

	if (sideNeaPolygon.has_value())
	{
		const auto& pol = sideNeaPolygon.value();

		// �v�Z�p�v���C���[���W(Y���l�����Ȃ�)
		Position3D pPos = cylinderPos;
		pPos.y = 0.0f;

		// �v�Z�p����(Y���l�����Ȃ�)
		Vector3 normal = pol.normal;
		Position3D planePos = pol.tPos[0];
		planePos.y = 0.0f;

		// �N�����v�p
		// X���W
		float Xmin = pol.tPos[0].x;
		float Xmax = pol.tPos[0].x;
		// Z���W
		float Zmin = pol.tPos[0].z;
		float Zmax = pol.tPos[0].z;

		for (auto p : pol.tPos)
		{
			// X���W
			if (Xmin > p.x)Xmin = p.x;
			if (Xmax < p.x)Xmax = p.x;

			// Z���W
			if (Zmin > p.z)Zmin = p.z;
			if (Zmax < p.z)Zmax = p.z;
		}

		// �v���C���[�ɍł��߂����ʏ�̍��W
		Position3D nearPos = FindNearestPointOnPlane(pPos, planePos, normal);

		// �|���S���̒�����ŃN�����v
		nearPos.x = clamp(nearPos.x, Xmin, Xmax);
		nearPos.z = clamp(nearPos.z, Zmin, Zmax);

		// �����߂������x�N�g���v�Z
		Vector3 vec = (pPos - nearPos).Normalized();

		// �v���C���[���X�e�[�W��Ɉړ�
		// ���a�擾
		nearPos.y = cylinderPos.y;
		cylinder.pos = nearPos + (vec * (cylinder.radius + 0.1f));
		cylinder.pos.y = nearPos.y;

		cylinderCol->ApplyColliderUpdateToFollower();



	}

	// �����
	optional<POLYGON_COL_INFO> upNeaPolygon = {};
	nearDis = 0.0f;
	for (const auto& polygon : upNormalPolygons)
	{
		if (!upNeaPolygon.has_value())
		{
			upNeaPolygon = polygon;

			nearDis = fabsf(polygon.tPos[0].y - cylinderPos.y);

			continue;
		}

		float dis = fabsf(polygon.tPos[0].y - cylinderPos.y);

		if (dis < nearDis)
		{
			upNeaPolygon = polygon;
			nearDis = dis;
		}
	}

	if (upNeaPolygon.has_value())
	{
		const auto& pol = upNeaPolygon.value();

		cylinder.pos.y = (pol.tPos[0].y + cylinder.height / 2.0f) + 0.1f;

		cylinderCol->ApplyColliderUpdateToFollower();
	}
}
