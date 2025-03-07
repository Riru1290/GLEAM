#pragma once
#include "../Resource.h"
#include "../../Object/Effect/EffectController.h"

/// @brief Effekseerエフェクトリソース
class EffekseerEffect final : public Resource
{
public:

	[[nodiscard]] EffekseerEffect(const wstring& filename);
	~EffekseerEffect();

	/// @brief エフェクト本体を取得
	/// @return エフェクト本体
	[[nodiscard]] const Effekseer::EffectRef& GetEffect()const { return effect_; }

private:

	/// @brief 解放処理
	void Release()override;

	Effekseer::EffectRef effect_;	/// @brief エフェクト本体
};

