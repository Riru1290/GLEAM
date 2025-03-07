#include "../../Loader/MIZLoader.h"
#include "../../Common/Mesh/Meshes/MeshDefault3D.h"
#include "../../Common/Mesh/Meshes/MeshAnimation.h"
#include "Model.h"

Model::Model(const string& filePath)
{
	MODEL_DATA data = MIZLoader::GetInstance().LoadModel(filePath.c_str());

	bool isAnimationModel = (data.bones.size() > 0);

	// メッシュ作成
	meshes_.reserve(data.meshes.size());

	if (isAnimationModel)
	{
		for (const auto& mesh : data.meshes)
		{
			meshes_.emplace_back(move(make_unique<MeshAnimation>(mesh, PSO_ID::ANIMATION_MODEL)));
		}
	}
	else 
	{
		for (const auto& mesh : data.meshes)
		{
			meshes_.emplace_back(move(make_unique<MeshDefault3D>(mesh, PSO_ID::DEFAULT_3D)));
		}
	}

	// ボーンデータセット
	bones_.resize(data.bones.size());
	for (int i = 0;const auto& bone : data.bones)
	{
		bones_[i].name = bone.boneName;
		bones_[i].chiledlenBoneIndices = bone.chiledlenBoneIndices;
		bones_[i].globalBindInverseMat = bone.globalBindInverseMat;
		bones_[i].transformLinkMat = bone.transformLinkMat;
		i++;
	}
}

Model::~Model()
{
}

void Model::Release()
{
}
