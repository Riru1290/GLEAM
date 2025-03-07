#pragma once
#include "../../Common/Mesh/MeshData.h"
#include "../../Common/Vertex/VertexData.h"

/// @brief ボーンデータ
struct BONE_DATA
{
	string boneName;					/// @brief ボーンの名前

	vector<int> chiledlenBoneIndices;	/// @brief 子供のインデックス

	XMMATRIX transformLinkMat;			/// @brief 初期姿勢行列
	XMMATRIX globalBindInverseMat;		/// @brief 初期姿勢逆行列
};

/// @brief モデル作成時必要データ
struct MODEL_DATA
{
	vector<MESH_DATA> meshes;	/// @brief メッシュデータ

	vector<BONE_DATA> bones;	/// @brief ボーンデータ
};

/// @brief アニメーションデータ
struct ANIMATION_DATA
{
	string animName;								/// @brief アニメーション名

	map<string, vector<XMMATRIX>> animBoneMatrix;	/// @brief ボーンごと時間ごとのアニメーション行列

	double start;									/// @brief アニメーション開始時間(秒)
	double end;										/// @brief アニメーション終了時間(秒)
	double frameRate;								/// @brief アニメーションフレームレート(おおむね60)
};
