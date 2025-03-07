#pragma once
#include "../Object.h"
#include "../../Resource/Model/Model.h"


/// @brief �v���C���[���B��郍�b�J�[
class Locker final : public Object
{
public:

	[[nodiscard]] Locker(const string& uniqueKey);
	~Locker() = default;

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::LOCKER; }

	/// @brief ����������
	void InitObject() override;

	/// @brief �`�揈��
	void DrawObject() override;

	/// @brief ��{�I�ȍX�V����
	void UpdateObject() override;

	/// @brief �R���C�_�[�ݒ�
	void ColliderSetting();

	optional<reference_wrapper<const Model>> model_;
};

