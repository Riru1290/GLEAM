#pragma once
#include "../../Graphics/Material/Material.h"
#include "../../Common/Mesh/Mesh.h"
#include "../UIObject.h"

class ModelBlinker;

class BlinkUI final : public UIObject
{
public:

	[[nodiscard]] BlinkUI();
	~BlinkUI() = default;

	void SetBlinkSpeed(float blinkSpeed);

private:


	/// @brief 基本的な更新処理
	void UpdateUI() override;

	/// @brief テクスチャ設定後の初期化
	void InitAfterSetTexture() override;

	optional<reference_wrapper<ModelBlinker>> blinker_;
};

