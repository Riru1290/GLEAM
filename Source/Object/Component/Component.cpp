#include "Component.h"

Component::Component() :
	isActive_(true)
{
}

void Component::Init()
{
	InitComponent();
}

void Component::Update()
{
	if (isActive_)
	{
		UpdateComponent();
	}
}

void Component::PreUpdate()
{
	if (isActive_)
	{
		PreUpdateComponent();
	}
}

void Component::LateUpdate()
{
	if (isActive_)
	{
		LateUpdateComponent();
	}
}

void Component::OnCollisionUpdate(COLLISION_FASE fase)
{
	if (!onCollisionUpdateList_.contains(fase))return;

	onCollisionUpdateList_.at(fase)();
}

void Component::OnCameraUpdate()
{
	if (isActive_)
	{
		OnCameraUpdateComponent();
	}
}

void Component::Draw()
{
	if (isActive_)
	{
		DrawComponent();
	}
}

void Component::Release()
{
	ReleaseComponent();
}
