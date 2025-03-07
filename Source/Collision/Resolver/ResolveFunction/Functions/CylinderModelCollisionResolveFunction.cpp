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

	// コライダー情報取得
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

	// コライダー情報取得
	auto [cylinderCol, modelCol] = ConvertCollider<CylinderCollider, ModelCollider>
		(data.targetCollider, data.selfCollider, COLLISION_SHAPE_TYPE::CYLINDER, COLLISION_SHAPE_TYPE::MODEL);

	auto& cylinder = cylinderCol->GetCylinderInfo();
	auto& preCylinder = cylinderCol->GetPreCylinderInfo();

	// シリンダー座標
	Position3D cylinderPos = cylinder.pos;
	Position3D cylinderPrePos = preCylinder.pos;

	// ポリゴンを法線方向ごとに分別
	vector<POLYGON_COL_INFO> upNormalPolygons;		// Y=  1.0f方向 上
	vector<POLYGON_COL_INFO> downNormalPolygons;	// Y= -1.0f方向 下
	vector<POLYGON_COL_INFO> sideNormalPolygons;	// Y=  0.0f方向 横
	vector<POLYGON_COL_INFO> otherNormalPolygons;	// その他 斜め

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

	// 横方向
	optional<POLYGON_COL_INFO> sideNeaPolygon = {};
	float nearDis = 0.0f;
	for (const auto& polygon : sideNormalPolygons)
	{
		// クランプ用
		// X座標
		float Xmin = polygon.tPos[0].x;
		float Xmax = polygon.tPos[0].x;
		// Z座標
		float Zmin = polygon.tPos[0].z;
		float Zmax = polygon.tPos[0].z;

		for (auto p : polygon.tPos)
		{
			// X座標
			if (Xmin > p.x)Xmin = p.x;
			if (Xmax < p.x)Xmax = p.x;

			// Z座標
			if (Zmin > p.z)Zmin = p.z;
			if (Zmax < p.z)Zmax = p.z;
		}

		if (!sideNeaPolygon.has_value())
		{
			sideNeaPolygon = polygon;

			Position3D nearPos = FindNearestPointOnPlane(cylinderPos, polygon.tPos[0], polygon.normal);
			// ポリゴンの直線上でクランプ
			nearPos.x = clamp(nearPos.x, Xmin, Xmax);
			nearPos.z = clamp(nearPos.z, Zmin, Zmax);

			nearDis = (nearPos - cylinderPos).Length();

			continue;
		}

		Position3D nearPos = FindNearestPointOnPlane(cylinderPos, polygon.tPos[0], polygon.normal);

		// ポリゴンの直線上でクランプ
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

		// 計算用プレイヤー座標(Yを考慮しない)
		Position3D pPos = cylinderPos;
		pPos.y = 0.0f;

		// 計算用平面(Yを考慮しない)
		Vector3 normal = pol.normal;
		Position3D planePos = pol.tPos[0];
		planePos.y = 0.0f;

		// クランプ用
		// X座標
		float Xmin = pol.tPos[0].x;
		float Xmax = pol.tPos[0].x;
		// Z座標
		float Zmin = pol.tPos[0].z;
		float Zmax = pol.tPos[0].z;

		for (auto p : pol.tPos)
		{
			// X座標
			if (Xmin > p.x)Xmin = p.x;
			if (Xmax < p.x)Xmax = p.x;

			// Z座標
			if (Zmin > p.z)Zmin = p.z;
			if (Zmax < p.z)Zmax = p.z;
		}

		// プレイヤーに最も近い平面上の座標
		Position3D nearPos = FindNearestPointOnPlane(pPos, planePos, normal);

		// ポリゴンの直線上でクランプ
		nearPos.x = clamp(nearPos.x, Xmin, Xmax);
		nearPos.z = clamp(nearPos.z, Zmin, Zmax);

		// 押し戻す方向ベクトル計算
		Vector3 vec = (pPos - nearPos).Normalized();

		// プレイヤーをステージ上に移動
		// 半径取得
		nearPos.y = cylinderPos.y;
		cylinder.pos = nearPos + (vec * (cylinder.radius + 0.1f));
		cylinder.pos.y = nearPos.y;

		cylinderCol->ApplyColliderUpdateToFollower();



	}

	// 上方向
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
