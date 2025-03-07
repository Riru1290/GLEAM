#pragma once
#include "../../../Common/Mesh/MeshData.h"

class Mesh;

/// @brief ボックス
class Box
{
public:

public:

	[[nodiscard]] Box();
	~Box() = default;

	/// @brief メッシュ取得
	/// @return メッシュ
	[[nodiscard]] const unique_ptr<Mesh>& GetMesh()const { return mesh_; }

	static constexpr float DEFAULT_SIDE_LENGTH = (10.0f);	/// @brief デフォルトの一辺の長さ

private:

	/// @brief 球体のメッシュデータを作成して返す
	/// @return 球体のメッシュデータ
	MESH_DATA CreateBoxMeshData();

	unique_ptr<Mesh> mesh_;	/// @brief 球体を構成するメッシュ

};

