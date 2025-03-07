#ifdef _DEBUG
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../Common/Transform.h"
#include "LineCollider.h"

void LineCollider::ApplyColliderUpdateToFollower()
{
	auto& follower = follower_->get();
	auto& subFollower = subFollower_->get();

	Position3D& fPosA = follower.pos;
	Position3D& fPosB = subFollower.pos;

	fPosA = line_.posA;
	fPosB = line_.posB;

	line_.UpdateAABB();
}

void LineCollider::UpdateCollider()
{
	if (!follower_.has_value() || !subFollower_.has_value())return;

	auto& follower = follower_->get();
	auto& subFollower = subFollower_->get();

	const Position3D& fPosA = follower.pos;
	const Position3D& fPosB = subFollower.pos;

	line_.posA = fPosA;
	line_.posB = fPosB;

	line_.UpdateAABB();
}

void LineCollider::PreUpdateCollider()
{
	preLine_ = line_;
}

void LineCollider::DrawCollider()
{
}
