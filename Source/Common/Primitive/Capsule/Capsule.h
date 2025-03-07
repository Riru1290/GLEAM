#pragma once
#include "../../../Common/Mesh/MeshData.h"

class Mesh;

/// @brief �J�v�Z��
class Capsule
{
public:

	[[nodiscard]] Capsule();
	~Capsule() = default;

	/// @brief ���b�V���擾
	/// @return ���b�V��
	[[nodiscard]] const vector<unique_ptr<Mesh>>& GetMeshes()const { return meshes_; }

	static constexpr float DEFAULT_RADIUS = (10.0f);	/// @brief �f�t�H���g���a
	static constexpr float DEFAULT_HEIGHT = (10.0f);	/// @brief �f�t�H���g����

private:

	/// @brief ���̂̃��b�V���f�[�^���쐬���ĕԂ�
	/// @return ���̂̃��b�V���f�[�^
	MESH_DATA CreateCapsuleTopMeshData();
	MESH_DATA CreateCapsuleMiddleMeshData();
	MESH_DATA CreateCapsuleDownMeshData();

	vector<unique_ptr<Mesh>> meshes_;	/// @brief �|���S�����\�����郁�b�V��
};

