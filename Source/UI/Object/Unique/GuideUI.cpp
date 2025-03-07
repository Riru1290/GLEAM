#include "../../../Game/Event/EventManager.h"
#include "../../../Object/Component/Common/Fader.h"
#include "GuideUI.h"

GuideUI::GuideUI(EVENT_ID eventID, EVENT_ID afterEventID):UIObject(),
eventID_(eventID)
{
	AddComponent(make_shared<Fader>());
	fader_ = GetComponent<Fader>();

	EventMng.RegisterEventAction(eventID, [this] {
		fader_->get().FadeInNow(0.3f);
		});

	EventMng.RegisterEventAction(afterEventID, [this] {
		fader_->get().FadeOutNow(0.3f);
		});
}

void GuideUI::UpdateUI()
{
}

void GuideUI::InitAfterSetTexture()
{
	if (object2DRenderer_.has_value()) {
		fader_->get().SetFadeRenderer(object2DRenderer_->get());
	}
	fader_->get().SetRate(0.0f);
	fader_->get().SetIsFadeIn(false);
}
