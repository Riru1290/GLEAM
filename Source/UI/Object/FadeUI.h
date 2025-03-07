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


	/// @brief 基本的な更新処理
	void UpdateUI() override;

	/// @brief テクスチャ設定後の初期化
	void InitAfterSetTexture() override;

	optional<reference_wrapper<Fader>> fader_;

	bool isHide_;

};



