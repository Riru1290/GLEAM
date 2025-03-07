#pragma once
#include "../../../Common/Mesh/MeshData.h"

class Mesh;

/// @brief ����
class Sphere
{
public:

	[[nodiscard]] Sphere();
	~Sphere() = default;

	/// @brief ���b�V���擾
	/// @return ���b�V��
	[[nodiscard]] const unique_ptr<Mesh>& GetMesh()const { return mesh_; }

	static constexpr float DEFAULT_RADIUS = (10.0f);	/// @brief �f�t�H���g���a

private:

	/// @brief ���̂̃��b�V���f�[�^���쐬���ĕԂ�
	/// @return ���̂̃��b�V���f�[�^
	MESH_DATA CreateSphereMeshData();

	unique_ptr<Mesh> mesh_;	/// @brief ���̂��\�����郁�b�V��
};

