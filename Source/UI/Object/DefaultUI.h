#pragma once
#include "../UIObject.h"

class DefaultUI final : public UIObject
{
public:

	[[nodiscard]] DefaultUI();
	~DefaultUI() = default;

private:

	/// @brief ��{�I�ȍX�V����
	void UpdateUI() override;

	/// @brief �e�N�X�`���ݒ��̏�����
	void InitAfterSetTexture() override {}
};

