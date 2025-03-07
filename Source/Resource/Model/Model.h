#pragma once
#include "../Resource.h"
#include "ModelData.h"

class Mesh;
struct Bone;

/// @brief モデルデータ
class Model final : public Resource
{
public:

	[[nodiscard]] Model(const string& filePath);
	~Model();

	/// @brief 解放処理
	void Release()override;

	/// @brief メッシュ取得
	/// @return メッシュ
	[[nodiscard]] const vector<unique_ptr<Mesh>>& GetMeshes()const { return meshes_; }

	/// @brief ボーン取得
	/// @return ボーン
	[[nodiscard]] const vector<Bone>& GetBones()const { return bones_; }

private:

	vector<unique_ptr<Mesh>> meshes_;	/// @brief モデルを構成するメッシュ

	vector<Bone> bones_;				/// @brief モデルを構成するボーン
};

struct Bone
{
	// ボーンの名前
	string name;

	// 子供のインデックス
	vector<int> chiledlenBoneIndices;

	// 初期姿勢行列
	XMMATRIX transformLinkMat;

	// 初期姿勢逆行列
	XMMATRIX globalBindInverseMat;
};

