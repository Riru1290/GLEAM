#include "../../Object/Component/Common/ModelBlinker.h"
#include "BlinkUI.h"

BlinkUI::BlinkUI() : UIObject()
{
}

void BlinkUI::SetBlinkSpeed(float blinkSpeed)
{
	blinker_->get().SetBlinkSpeed(blinkSpeed);
}

void BlinkUI::UpdateUI()
{
}

void BlinkUI::InitAfterSetTexture()
{
	AddComponent(make_shared<ModelBlinker>());
	blinker_ = GetComponent<ModelBlinker>();
	if (modelRenderer_.has_value()) {
		blinker_->get().SetFadeRenderer(modelRenderer_->get());
	}
	else if (animModelRenderer_.has_value()) {
		blinker_->get().SetFadeRenderer(animModelRenderer_->get());
	}
	else if (object2DRenderer_.has_value()) {
		blinker_->get().SetFadeRenderer(object2DRenderer_->get());
	}
	else if (meshRenderer_.has_value()) {
		blinker_->get().SetFadeRenderer(meshRenderer_->get());
	}
}
