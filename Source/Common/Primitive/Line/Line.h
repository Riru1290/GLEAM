#pragma once

class Mesh;

/// @brief ��
class Line
{
public:

	[[nodiscard]] Line();
	~Line();

	/// @brief ���b�V���擾
	/// @return ���b�V��
	[[nodiscard]] const vector<unique_ptr<Mesh>>& GetMeshes()const { return meshes_; }

private:

	vector<unique_ptr<Mesh>> meshes_;	/// @brief �����\�����郁�b�V��
};

