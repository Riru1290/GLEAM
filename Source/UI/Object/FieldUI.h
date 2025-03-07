#pragma once
#include "../UIObject.h"

/// @brief フィールド上に登場するUI
class FieldUI final : public UIObject
{
public:

	[[nodiscard]] FieldUI();
	~FieldUI() = default;

private:

	/// @brief 基本的な更新処理
	void UpdateUI() override {}

	/// @brief テクスチャ設定後の初期化
	void InitAfterSetTexture() override {}
};
