#pragma once

class Mesh;

/// @brief 線
class Line
{
public:

	[[nodiscard]] Line();
	~Line();

	/// @brief メッシュ取得
	/// @return メッシュ
	[[nodiscard]] const vector<unique_ptr<Mesh>>& GetMeshes()const { return meshes_; }

private:

	vector<unique_ptr<Mesh>> meshes_;	/// @brief 線を構成するメッシュ
};

