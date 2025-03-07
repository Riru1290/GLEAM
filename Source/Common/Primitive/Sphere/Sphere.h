#pragma once
#include "../../../Common/Mesh/MeshData.h"

class Mesh;

/// @brief 球体
class Sphere
{
public:

	[[nodiscard]] Sphere();
	~Sphere() = default;

	/// @brief メッシュ取得
	/// @return メッシュ
	[[nodiscard]] const unique_ptr<Mesh>& GetMesh()const { return mesh_; }

	static constexpr float DEFAULT_RADIUS = (10.0f);	/// @brief デフォルト半径

private:

	/// @brief 球体のメッシュデータを作成して返す
	/// @return 球体のメッシュデータ
	MESH_DATA CreateSphereMeshData();

	unique_ptr<Mesh> mesh_;	/// @brief 球体を構成するメッシュ
};

