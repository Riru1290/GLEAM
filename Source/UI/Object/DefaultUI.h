#pragma once
#include "../UIObject.h"

class DefaultUI final : public UIObject
{
public:

	[[nodiscard]] DefaultUI();
	~DefaultUI() = default;

private:

	/// @brief 基本的な更新処理
	void UpdateUI() override;

	/// @brief テクスチャ設定後の初期化
	void InitAfterSetTexture() override {}
};

