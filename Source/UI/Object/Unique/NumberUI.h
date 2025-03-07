#pragma once
#include "../../UIObject.h"

class Fader;

/// @brief 数字UI
class NumberUI final : public UIObject
{
public:

	[[nodiscard]] NumberUI(const int& num);
	~NumberUI() = default;

	void SetOffsetX(float offset) { offsetX_ = offset; }

private:

	/// @brief 基本的な更新処理
	void UpdateUI() override;

	/// @brief テクスチャ設定後の初期化
	void InitAfterSetTexture() override;


	optional<reference_wrapper<const int>> num_;

	float offsetX_;
};

