#include "../../../Resource/ResourceManager.h"
#include "../../Component/Actor/AnimationController.h"
#include "../../../Resource/Animation/Animation.h"
#include "../../Component/Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "VisualPlayer.h"

VisualPlayer::VisualPlayer(const string& uniqueKey, RES_ID animationID) : Actor(uniqueKey),
animationID_(animationID)
{
	// ���f���ǂݍ���
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
	// �A�j���[�V������K�p�����郂�f���Z�b�g
	animationController_->get().SetAnimationModel(modelRenderer_->get());

	// �A�j���[�V�����ǂݍ��݁E�Z�b�g
	auto animData = ResourceMng.GetAnimation(animationID_).GetData();
	animationController_->get().AddAnimation(animData, "ANIMATION");

	animationController_->get().ChangeAnimation("ANIMATION");
}
