#pragma once
#include "../../../Common/Mesh/MeshData.h"

class Mesh;

class Cylinder
{
public:

	[[nodiscard]] Cylinder();
	~Cylinder() = default;

	/// @brief ���b�V���擾
	/// @return ���b�V��
	[[nodiscard]] const unique_ptr<Mesh>& GetMesh()const { return mesh_; }

	static constexpr float DEFAULT_RADIUS = (10.0f);	/// @brief �f�t�H���g���a
	static constexpr float DEFAULT_HEIGHT = (10.0f);	/// @brief �f�t�H���g����

private:

	/// @brief �V�����_�[�̃��b�V���f�[�^���쐬���ĕԂ�
	/// @return �V�����_�[�̃��b�V���f�[�^
	MESH_DATA CreateCylinderMeshData();

	unique_ptr<Mesh> mesh_;	/// @brief ���̂��\�����郁�b�V��
};

