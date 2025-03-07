#include "../../../Resource/ResourceManager.h"
#include "../../Component/Actor/AnimationController.h"
#include "../../../Resource/Animation/Animation.h"
#include "../../Component/Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "VisualPlayer.h"

VisualPlayer::VisualPlayer(const string& uniqueKey, RES_ID animationID) : Actor(uniqueKey),
animationID_(animationID)
{
	// モデル読み込み
	modelRenderer_->get().SetModel(ResourceMng.GetModel(RES_ID::MODEL_PLAYER));

	modelRenderer_->get().SetTransform(transform_->get());
}

VisualPlayer::~VisualPlayer()
{
}

void VisualPlayer::UpdateActor()
{
}

void VisualPlayer::AnimationSetting()
{
	// アニメーションを適用させるモデルセット
	animationController_->get().SetAnimationModel(modelRenderer_->get());

	// アニメーション読み込み・セット
	auto animData = ResourceMng.GetAnimation(animationID_).GetData();
	animationController_->get().AddAnimation(animData, "ANIMATION");

	animationController_->get().ChangeAnimation("ANIMATION");
}
