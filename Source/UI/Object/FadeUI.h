#pragma once
#include "../UIObject.h"

class Fader;

class FadeUI final : public UIObject
{
public:

	[[nodiscard]] FadeUI(bool isHide = false);
	~FadeUI() = default;

	[[nodiscard]] bool IsEnd();
	[[nodiscard]] bool IsFadeIn();

	void FadeIn(float fadeInTime);
	void FadeOut(float fadeOutTime);

private:


	/// @brief ��{�I�ȍX�V����
	void UpdateUI() override;

	/// @brief �e�N�X�`���ݒ��̏�����
	void InitAfterSetTexture() override;

	optional<reference_wrapper<Fader>> fader_;

	bool isHide_;

};



