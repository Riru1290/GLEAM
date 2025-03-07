#pragma once
#include "../../../Common/Mesh/MeshData.h"

class Mesh;

/// @brief カプセル
class Capsule
{
public:

	[[nodiscard]] Capsule();
	~Capsule() = default;

	/// @brief メッシュ取得
	/// @return メッシュ
	[[nodiscard]] const vector<unique_ptr<Mesh>>& GetMeshes()const { return meshes_; }

	static constexpr float DEFAULT_RADIUS = (10.0f);	/// @brief デフォルト半径
	static constexpr float DEFAULT_HEIGHT = (10.0f);	/// @brief デフォルト高さ

private:

	/// @brief 球体のメッシュデータを作成して返す
	/// @return 球体のメッシュデータ
	MESH_DATA CreateCapsuleTopMeshData();
	MESH_DATA CreateCapsuleMiddleMeshData();
	MESH_DATA CreateCapsuleDownMeshData();

	vector<unique_ptr<Mesh>> meshes_;	/// @brief ポリゴンを構成するメッシュ
};

