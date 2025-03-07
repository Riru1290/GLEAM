#pragma once
#include "../../../Common/Mesh/MeshData.h"

class Mesh;

class Cylinder
{
public:

	[[nodiscard]] Cylinder();
	~Cylinder() = default;

	/// @brief メッシュ取得
	/// @return メッシュ
	[[nodiscard]] const unique_ptr<Mesh>& GetMesh()const { return mesh_; }

	static constexpr float DEFAULT_RADIUS = (10.0f);	/// @brief デフォルト半径
	static constexpr float DEFAULT_HEIGHT = (10.0f);	/// @brief デフォルト高さ

private:

	/// @brief シリンダーのメッシュデータを作成して返す
	/// @return シリンダーのメッシュデータ
	MESH_DATA CreateCylinderMeshData();

	unique_ptr<Mesh> mesh_;	/// @brief 球体を構成するメッシュ
};

