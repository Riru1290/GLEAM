#include "../../../Input/InputManager.h"
#include "../../../Resource/ResourceManager.h"
#include "../../../Collision/CollisionManager.h"
#include "../../../Game/Event/EventManager.h"
#include "../../../Object/Component/Common/Fader.h"
#include "../../../Object/Component/Collider/Colliders/Rect2DCollider.h"
#include "../../../Object/Component/Common/Transform.h"
#include "../../../Game/GameDifficultyController.h"
#include "LevelButtonUI.h"

namespace
{
	const float BUTTON_WIDTH = (64.0f);
	const float BUTTON_HEIGHT = (64.0f);
}

LevelButtonUI::LevelButtonUI(int level, int defNo, int pushNo) :
	level_(level),
	defNo_(defNo),
	pushNo_(pushNo),
	onClick_(false)
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
	collider->AddOnHitFunction(OBJECT_ID::CURSOR, bind(&LevelButtonUI::OnHitCursor, this, placeholders::_1));
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

void LevelButtonUI::PressButton()
{
	EventMng.Notify(EVENT_ID::PRESS_LEVELBUTTON);
	onClick_ = true;
	GameDifficulty.SetLevel(level_);
}

void LevelButtonUI::ReleaseButton()
{
	onClick_ = false;
}

void LevelButtonUI::UpdateUI()
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

void LevelButtonUI::InitAfterSetTexture()
{

}

void LevelButtonUI::OnHitCursor(COL_HIT_DATA data)
{
	if (InputMng.Triggered("ButtonUI")) {
		EventMng.Notify(EVENT_ID::PRESS_LEVELBUTTON);
		GameDifficulty.SetLevel(level_);
		onClick_ = true;
	}
	if (InputMng.Pressed("ButtonUI")) {
		if (!preOnHit_) {
			EventMng.Notify(EVENT_ID::PRESS_LEVELBUTTON);
			GameDifficulty.SetLevel(level_);
			onClick_ = true;
		}
	}
	onHit_ = true;
}
