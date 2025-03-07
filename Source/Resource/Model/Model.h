#pragma once
#include "../Resource.h"
#include "ModelData.h"

class Mesh;
struct Bone;

/// @brief ���f���f�[�^
class Model final : public Resource
{
public:

	[[nodiscard]] Model(const string& filePath);
	~Model();

	/// @brief �������
	void Release()override;

	/// @brief ���b�V���擾
	/// @return ���b�V��
	[[nodiscard]] const vector<unique_ptr<Mesh>>& GetMeshes()const { return meshes_; }

	/// @brief �{�[���擾
	/// @return �{�[��
	[[nodiscard]] const vector<Bone>& GetBones()const { return bones_; }

private:

	vector<unique_ptr<Mesh>> meshes_;	/// @brief ���f�����\�����郁�b�V��

	vector<Bone> bones_;				/// @brief ���f�����\������{�[��
};

struct Bone
{
	// �{�[���̖��O
	string name;

	// �q���̃C���f�b�N�X
	vector<int> chiledlenBoneIndices;

	// �����p���s��
	XMMATRIX transformLinkMat;

	// �����p���t�s��
	XMMATRIX globalBindInverseMat;
};

