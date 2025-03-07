#pragma once
#include "ColliderInfo.h"

class Collider;

struct COL_HIT_DATA_POLYGON
{
	int hitPolygonNum = 0;						/// @brief 衝突していたポリゴンの数
	vector<POLYGON_COL_INFO> hitPolygons;		/// @brief 衝突していたポリゴン
	optional<POLYGON_COL_INFO> neaPolygon;		/// @brief 最も近いポリゴン
	optional<Position3D> neaPolygonVertex;		/// @brief 最も近いポリゴンの頂点
	optional<Position3D> neaHitPos;				/// @brief 最も近いポリゴンの衝突点

};

struct COL_HIT_DATA_PLANE
{
	int hitBoxNum = 0;							/// @brief 衝突していた平面の数
	vector<pair<Position3D, Vector3>> planes;	/// @brief 衝突していた平面
	pair<Position3D, Vector3> neaPlane;			/// @brief 最も近い平面
};

struct COL_HIT_DATA_LINE
{
	optional<Position3D> neaHitPos;				/// @brief 最も近い衝突点
};

/// @brief 衝突時データ
struct COL_HIT_DATA
{
	weak_ptr<Collider> selfCollider;	/// @brief 自分自身のコライダー
	weak_ptr<Collider> targetCollider;	/// @brief 衝突対象のコライダー

	variant<COL_HIT_DATA_POLYGON, COL_HIT_DATA_PLANE, COL_HIT_DATA_LINE> hitData;
};
