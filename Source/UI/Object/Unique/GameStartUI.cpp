#include "../../../Input/InputManager.h"
#include "../../../Resource/ResourceManager.h"
#include "../../../Collision/CollisionManager.h"
#include "../../../Game/Event/EventManager.h"
#include "../../../Object/Component/Common/Fader.h"
#include "../../../Object/Component/Collider/Colliders/Rect2DCollider.h"
#include "../../../Object/Component/Common/Transform.h"
#include "GameStartUI.h"

namespace
{
	const float BUTTON_WIDTH = (550.0f);
	const float BUTTON_HEIGHT = (130.0f);
}

GameStartUI::GameStartUI() : UIObject()
{
	SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_GAMESTART));

	AddComponent(make_shared<Fader>());
	fader_ = GetComponent<Fader>();
	if (object2DRenderer_.has_value()) {
		fader_->get().SetFadeRenderer(object2DRenderer_->get());
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
	collider->AddOnHitFunction(OBJECT_ID::CURSOR, bind(&GameStartUI::OnHitCursor, this, placeholders::_1));
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

void GameStartUI::UpdateUI()
{

	if (onHit_) {
		transform_->get().scale = { 1.2f,1.2f,1.2f };
	}
	else
	{
		transform_->get().scale = { 1.0f,1.0f,1.0f };
	}

	onHit_ = false;
}

void GameStartUI::InitAfterSetTexture()
{

}

void GameStartUI::OnHitCursor(COL_HIT_DATA data)
{
	if (InputMng.Triggered("ButtonUI")) {
		EventMng.Notify(EVENT_ID::PRESS_GAMESTART);
	}
	onHit_ = true;
}

