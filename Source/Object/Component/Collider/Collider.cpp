#include "../../../Collision/CollisionManager.h"
#include "Collider.h"

Collider::Collider() : Component(),
	relativePos_({}),
	isStatic_(false),
	isRigidBody_(false),
	weight_(1.0f)
{
}


void Collider::OnHit(OBJECT_ID objectID, COL_HIT_DATA hitData)
{
	if (!onHitList_.contains(objectID))return;

	for (auto& onHit : onHitList_.at(objectID))
	{
		onHit(hitData);
	}
}

void Collider::AddOnHitFunction(OBJECT_ID objectID, function<void(COL_HIT_DATA)> onHit)
{
	onHitList_[objectID].emplace_back(onHit);
}

void Collider::SetFollower(Transform& transform)
{
	follower_ = transform;
	UpdateCollider();
	PreUpdateCollider();
}

void Collider::SetSubFollower(Transform& transform)
{
	subFollower_ = transform;
	UpdateCollider();
	PreUpdateCollider();
}

void Collider::UpdateComponent()
{
	if (!isStatic_)
	{
		UpdateCollider();
	}
}

void Collider::PreUpdateComponent()
{
	if (!isStatic_)
	{
		PreUpdateCollider();
	}
}

void Collider::DrawComponent()
{
	DrawCollider();
}

void Collider::ReleaseComponent()
{
	CollisionMng.UnregisterCollider(GetPtr());
}
