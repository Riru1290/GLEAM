#pragma once
#include "../../../Common/Mesh/MeshData.h"

class Mesh;

/// @brief �{�b�N�X
class Box
{
public:

public:

	[[nodiscard]] Box();
	~Box() = default;

	/// @brief ���b�V���擾
	/// @return ���b�V��
	[[nodiscard]] const unique_ptr<Mesh>& GetMesh()const { return mesh_; }

	static constexpr float DEFAULT_SIDE_LENGTH = (10.0f);	/// @brief �f�t�H���g�̈�ӂ̒���

private:

	/// @brief ���̂̃��b�V���f�[�^���쐬���ĕԂ�
	/// @return ���̂̃��b�V���f�[�^
	MESH_DATA CreateBoxMeshData();

	unique_ptr<Mesh> mesh_;	/// @brief ���̂��\�����郁�b�V��

};

