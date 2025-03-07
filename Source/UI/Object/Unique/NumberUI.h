#pragma once
#include "../../UIObject.h"

class Fader;

/// @brief ����UI
class NumberUI final : public UIObject
{
public:

	[[nodiscard]] NumberUI(const int& num);
	~NumberUI() = default;

	void SetOffsetX(float offset) { offsetX_ = offset; }

private:

	/// @brief ��{�I�ȍX�V����
	void UpdateUI() override;

	/// @brief �e�N�X�`���ݒ��̏�����
	void InitAfterSetTexture() override;


	optional<reference_wrapper<const int>> num_;

	float offsetX_;
};

