#pragma once
#include "../Vertex/VertexData.h"

/// @brief メッシュ作成時必要データ
struct MESH_DATA
{
	vector<VERTEX_2D> verticesDefault2D;			/// @brief 頂点データ2D
	vector<VERTEX_3D> verticesDefault3D;			/// @brief 頂点データ3D
	vector<VERTEX_ANIMATION> verticesAnimation;		/// @brief 頂点データ(アニメーション)
	vector<unsigned short>indices;					/// @brief インデックスデータ

	string diffuseTexturePath;						/// @brief 拡散反射光
	string specularTexturePath;						/// @brief 鏡面反射光
	string normalTexturePath;						/// @brief ノーマル
	string aoTexturePath;							/// @brief アンビエントオクルージョン
};
