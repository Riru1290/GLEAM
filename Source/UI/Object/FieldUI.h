#pragma once
#include "../UIObject.h"

/// @brief �t�B�[���h��ɓo�ꂷ��UI
class FieldUI final : public UIObject
{
public:

	[[nodiscard]] FieldUI();
	~FieldUI() = default;

private:

	/// @brief ��{�I�ȍX�V����
	void UpdateUI() override {}

	/// @brief �e�N�X�`���ݒ��̏�����
	void InitAfterSetTexture() override {}
};
