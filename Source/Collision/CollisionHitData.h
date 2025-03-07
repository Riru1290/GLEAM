#pragma once
#include "ColliderInfo.h"

class Collider;

struct COL_HIT_DATA_POLYGON
{
	int hitPolygonNum = 0;						/// @brief �Փ˂��Ă����|���S���̐�
	vector<POLYGON_COL_INFO> hitPolygons;		/// @brief �Փ˂��Ă����|���S��
	optional<POLYGON_COL_INFO> neaPolygon;		/// @brief �ł��߂��|���S��
	optional<Position3D> neaPolygonVertex;		/// @brief �ł��߂��|���S���̒��_
	optional<Position3D> neaHitPos;				/// @brief �ł��߂��|���S���̏Փ˓_

};

struct COL_HIT_DATA_PLANE
{
	int hitBoxNum = 0;							/// @brief �Փ˂��Ă������ʂ̐�
	vector<pair<Position3D, Vector3>> planes;	/// @brief �Փ˂��Ă�������
	pair<Position3D, Vector3> neaPlane;			/// @brief �ł��߂�����
};

struct COL_HIT_DATA_LINE
{
	optional<Position3D> neaHitPos;				/// @brief �ł��߂��Փ˓_
};

/// @brief �Փˎ��f�[�^
struct COL_HIT_DATA
{
	weak_ptr<Collider> selfCollider;	/// @brief �������g�̃R���C�_�[
	weak_ptr<Collider> targetCollider;	/// @brief �ՓˑΏۂ̃R���C�_�[

	variant<COL_HIT_DATA_POLYGON, COL_HIT_DATA_PLANE, COL_HIT_DATA_LINE> hitData;
};
