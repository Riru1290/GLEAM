#pragma once
#include "../Object.h"
#include "../../Algorithm/NavMesh.h"

class ModelRenderer;

class Map final : public Object
{
public:

	[[nodiscard]] Map(const string& uniqueKey);
	~Map();

	/// @brief �i�r���b�V���擾
	/// @return �i�r���b�V��
	[[nodiscard]] const NavMesh& GetNevMesh()const { return navMesh_; }

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::MAP; }

	/// @brief �`�揈��
	void DrawObject() override;

	/// @brief ��{�I�ȍX�V����
	void UpdateObject() override;

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdateObject() override;

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdateObject() override;

	/// @brief �R���C�_�[�ݒ�
	void ColliderSetting();

	shared_ptr<ModelRenderer> renderer;

	NavMesh navMesh_;
};


