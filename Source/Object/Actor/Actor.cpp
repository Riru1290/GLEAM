#include "../../Graphics/RenderManager.h"
#include "../Component/Common/Transform.h"
#include "../Component/Actor/AnimationController.h"
#include "../Component/Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "Actor.h"

Actor::Actor(const string& uniqueKey) : Object(uniqueKey)
{
	AddComponent(RenderMng.CreateRenderer<AnimationModelRenderer>());
	AddComponent(make_shared<AnimationController>());

	modelRenderer_ = GetComponent<AnimationModelRenderer>();
	animationController_ = GetComponent<AnimationController>();


}

void Actor::InitObject()
{
	InitActor();

	InitAnimation();
}

void Actor::DrawObject()
{
	DrawActor();
}

void Actor::UpdateObject()
{
	UpdateActor();
}

void Actor::LateUpdateObject()
{
	LateUpdateActor();
}

void Actor::OnCameraUpdateObject()
{
	OnCameraUpdateActor();
}

void Actor::InitAnimation()
{
	AnimationSetting();
}
