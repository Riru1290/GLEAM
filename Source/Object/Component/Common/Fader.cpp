#include "../Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "../Renderer/Renderers/Single/2D/Object2DRenderer.h"
#include "../Renderer/Renderers/Single/2D/SpriteRenderer.h"
#include "../Renderer/Renderers/Single/MeshRenderer.h"
#include "Fader.h"

Fader::Fader() : Component()
{
}

bool Fader::IsEnd()
{
	return (rate_ == 1.0f);
}

bool Fader::IsFadeIn()
{
	return isFadeIn_;
}

void Fader::FadeIn(float fadeInTime)
{
	isFadeIn_ = true;
	fadeTime_ = fadeInTime;
	rate_ = 0.0f;
	timeCount_ = 0.0f;
}

void Fader::FadeOut(float fadeOutTime)
{
	isFadeIn_ = false;
	fadeTime_ = fadeOutTime;
	rate_ = 1.0f;
	timeCount_ = fadeTime_;
}

void Fader::FadeInNow(float fadeInTime)
{
	if (fadeTime_ != fadeInTime)
	{
		fadeTime_ = fadeInTime;
	}
	isFadeIn_ = true;
}

void Fader::FadeOutNow(float fadeOutTime)
{
	if (fadeTime_ != fadeOutTime)
	{
		fadeTime_ = fadeOutTime;
	}
	isFadeIn_ = false;
}

void Fader::SetRate(float rate)
{
	rate_ = rate;
	if (modelRenderer_.has_value()) {
		modelRenderer_->get().SetModelAlpha(rate_);
	}
	else if (animModelRenderer_.has_value()) {
		animModelRenderer_->get().SetModelAlpha(rate_);
	}
	else if (object2DRenderer_.has_value()) {
		object2DRenderer_->get().SetMeshAlpha(rate_);
	}
	else if (meshRenderer_.has_value()) {
		meshRenderer_->get().SetMeshAlpha(rate_);
	}
	else if (spriteRenderer_.has_value()) {
		spriteRenderer_->get().SetMeshAlpha(rate_);
	}
}

void Fader::SetFadeRenderer(ModelRenderer& renderer)
{
	modelRenderer_ = renderer;
}

void Fader::SetFadeRenderer(AnimationModelRenderer& renderer)
{
	animModelRenderer_ = renderer;
}

void Fader::SetFadeRenderer(Object2DRenderer& renderer)
{
	object2DRenderer_ = renderer;
}

void Fader::SetFadeRenderer(MeshRenderer& renderer)
{
	meshRenderer_ = renderer;
}

void Fader::SetFadeRenderer(SpriteRenderer& renderer)
{
	spriteRenderer_ = renderer;
}

void Fader::UpdateComponent()
{
	if (fadeTime_ <= 0.0f)return;

	if (isFadeIn_) {
		timeCount_ += MainTimer.GetDelta();
		timeCount_ = min(fadeTime_, timeCount_);
	}
	else {
		timeCount_ -= MainTimer.GetDelta();
		timeCount_ = max(0.0f, timeCount_);
	}

	rate_ = timeCount_ / fadeTime_;

	rate_ = clamp(rate_, 0.0f, 1.0f);

	if (modelRenderer_.has_value()) {
		modelRenderer_->get().SetModelAlpha(rate_);
	}
	else if (animModelRenderer_.has_value()) {
		animModelRenderer_->get().SetModelAlpha(rate_);
	}
	else if (object2DRenderer_.has_value()) {
		object2DRenderer_->get().SetMeshAlpha(rate_);
	}
	else if (meshRenderer_.has_value()) {
		meshRenderer_->get().SetMeshAlpha(rate_);
	}
	else if (spriteRenderer_.has_value()) {
		spriteRenderer_->get().SetMeshAlpha(rate_);
	}
}
