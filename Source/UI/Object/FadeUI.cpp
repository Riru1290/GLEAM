#include "../../Object/Component/Common/Fader.h"
#include "FadeUI.h"

FadeUI::FadeUI(bool isHide) : UIObject()
{
	isHide_ = isHide;
}

bool FadeUI::IsEnd()
{
	return fader_->get().IsEnd();
}

bool FadeUI::IsFadeIn()
{
	return fader_->get().IsFadeIn();
}

void FadeUI::FadeIn(float fadeInTime)
{
	fader_->get().FadeIn(fadeInTime);
}

void FadeUI::FadeOut(float fadeOutTime)
{
	fader_->get().FadeOut(fadeOutTime);
}

void FadeUI::UpdateUI()
{
}

void FadeUI::InitAfterSetTexture()
{
	AddComponent(make_shared<Fader>());
	fader_ = GetComponent<Fader>();

	if (modelRenderer_.has_value()) {
		fader_->get().SetFadeRenderer(modelRenderer_->get());
	}
	else if (animModelRenderer_.has_value()) {
		fader_->get().SetFadeRenderer(animModelRenderer_->get());
	}
	else if (object2DRenderer_.has_value()) {
		fader_->get().SetFadeRenderer(object2DRenderer_->get());
	}
	else if (meshRenderer_.has_value()) {
		fader_->get().SetFadeRenderer(meshRenderer_->get());
	}
	else if (spriteRenderer_.has_value()) {
		fader_->get().SetFadeRenderer(spriteRenderer_->get());
	}

	if (isHide_) {
		fader_->get().SetRate(0.0f);
		fader_->get().SetIsFadeIn(false);
	}
	else {
		fader_->get().SetRate(1.0f);
		fader_->get().SetIsFadeIn(true);
	}
}
