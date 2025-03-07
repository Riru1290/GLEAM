#pragma once
#include "../Component.h"

class ModelRenderer;
class AnimationModelRenderer;
class Object2DRenderer;
class MeshRenderer;
class SpriteRenderer;

/// @brief 対象をフェードさせる
class Fader final : public Component
{
public:

	[[nodiscard]] Fader();
	~Fader() = default;

	[[nodiscard]] bool IsEnd();
	[[nodiscard]] bool IsFadeIn();

	void FadeIn(float fadeInTime);
	void FadeOut(float fadeOutTime);

	void FadeInNow(float fadeInTime);
	void FadeOutNow(float fadeOutTime);

	[[nodiscard]] const float& GetRate()const { return rate_; }

	void SetRate(float rate);
	void SetIsFadeIn(bool isFadeIn) { isFadeIn_ = isFadeIn; }

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

	/// @brief フェードさせるレンダラーをセット
	/// @param renderer レンダラー
	void SetFadeRenderer(SpriteRenderer& renderer);

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::FADER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent() override {}

	float rate_;
	bool isFadeIn_;

	float timeCount_ = 0.0f;
	float fadeTime_ = 0.0f;

	optional<reference_wrapper<ModelRenderer>> modelRenderer_;
	optional<reference_wrapper<AnimationModelRenderer>> animModelRenderer_;
	optional<reference_wrapper<Object2DRenderer>> object2DRenderer_;
	optional<reference_wrapper<MeshRenderer>> meshRenderer_;
	optional<reference_wrapper<SpriteRenderer>> spriteRenderer_;
};

