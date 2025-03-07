#include "../../../Input/InputManager.h"
#include "../../../Resource/ResourceManager.h"
#include "../../../Collision/CollisionManager.h"
#include "../../../Game/Event/EventManager.h"
#include "../../../Object/Component/Common/Fader.h"
#include "../../../Object/Component/Collider/Colliders/Rect2DCollider.h"
#include "../../../Object/Component/Common/Transform.h"
#include "ButtonUI.h"

namespace
{
	const float BUTTON_WIDTH = (64.0f);
	const float BUTTON_HEIGHT = (64.0f);
}

ButtonUI::ButtonUI(EVENT_ID eventID, int defNo, int pushNo):
	onClick_(false),
	eventID_(eventID),
	defNo_(defNo),
	pushNo_(pushNo)
{
	SetTexture(ResourceMng.GetTexture(RES_ID::IMAGE_BUTTON), 10, 2);

	AddComponent(make_shared<Fader>());
	fader_ = GetComponent<Fader>();
	if (spriteRenderer_.has_value()) {
		fader_->get().SetFadeRenderer(spriteRenderer_->get());
	}
	fader_->get().SetRate(0.0f);
	fader_->get().SetIsFadeIn(false);

 	shared_ptr<Rect2DCollider> collider = make_shared<Rect2DCollider>();
	CollisionMng.RegisterCollider(collider, "UI", OBJECT_ID::UI);
	collider->SetFollower(transform_->get());
	RECT_COL_INFO rect;
	rect.width = BUTTON_WIDTH;
	rect.height = BUTTON_HEIGHT;
	collider->SetRectInfo(rect);
	collider->AddOnHitFunction(OBJECT_ID::CURSOR, bind(&ButtonUI::OnHitCursor, this, placeholders::_1));
	collider->SetActive(false);
	collider_ = *collider;

	AddComponent(collider);

	InputMng.AddKey("ButtonUI", { {INPUT_TYPE::Keyboard,VK_LBUTTON} });

	EventMng.RegisterEventAction(EVENT_ID::CHANGE_TO_LOBBY, [&] {
		fader_->get().FadeIn(0.3f);
		collider_->get().SetActive(true);
		});

	EventMng.RegisterEventAction(EVENT_ID::CHANGE_TO_TITLE, [&] {
		fader_->get().FadeOut(0.3f);
		collider_->get().SetActive(false);
		});

}

void ButtonUI::PressButton()
{
	EventMng.Notify(EVENT_ID::PRESS_TUTORIALBUTTON);
	onClick_ = true;
}

void ButtonUI::ReleaseButton()
{
	onClick_ = false;
}

void ButtonUI::UpdateUI()
{

	if (spriteRenderer_.has_value()) {
		if (onClick_) {
			spriteRenderer_->get().RenderSprite(transform_->get(), pushNo_);
		}
		else {
			spriteRenderer_->get().RenderSprite(transform_->get(), defNo_);
		}
	}

	preOnHit_ = onHit_;
	onHit_ = false;
}

void ButtonUI::InitAfterSetTexture()
{

}

void ButtonUI::OnHitCursor(COL_HIT_DATA data)
{
	if (InputMng.Triggered("ButtonUI")) {
		EventMng.Notify(EVENT_ID::PRESS_TUTORIALBUTTON);
		EventMng.Notify(eventID_);
		onClick_ = true;
	}
	if (InputMng.Pressed("ButtonUI")) {
		if (!preOnHit_) {
			EventMng.Notify(EVENT_ID::PRESS_TUTORIALBUTTON);
			EventMng.Notify(eventID_);
			onClick_ = true;
		}
	}
	onHit_ = true;
}
