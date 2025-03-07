#pragma once
#include "../Component.h"

class ModelRenderer;
class AnimationModelRenderer;
class Object2DRenderer;
class MeshRenderer;

/// @brief 対象を点滅させる
class ModelBlinker final : public Component
{
public:

	[[nodiscard]] ModelBlinker();
	~ModelBlinker() = default;

	void SetBlinkSpeed(float blinkSpeed)
	{
		blinkSpeed_ = blinkSpeed;
	}

	[[nodiscard]] const float& GetRate()const { return rate_; }

	/// @brief フェードさせるレンダラーをセット
	/// @param renderer レンダラー
	void SetFadeRenderer(ModelRenderer& renderer);

	/// @brief フェードさせるレンダラーをセット
	/// @param renderer レンダラー
	void SetFadeRenderer(AnimationModelRenderer& renderer);

	/// @brief フェードさせるレンダラーをセット
	/// @param renderer レンダラー
	void SetFadeRenderer(Object2DRenderer& renderer);

	/// @brief フェードさせるレンダラーをセット
	/// @param renderer レンダラー
	void SetFadeRenderer(MeshRenderer& renderer);

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::MODEL_BLINKER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent() override {}

	float rate_;

	float timeCount_;

	float fadeTime_;

	float blinkSpeed_ = 1.0f;

	optional<reference_wrapper<ModelRenderer>> modelRenderer_;
	optional<reference_wrapper<AnimationModelRenderer>> animModelRenderer_;
	optional<reference_wrapper<Object2DRenderer>> object2DRenderer_;
	optional<reference_wrapper<MeshRenderer>> meshRenderer_;
};

