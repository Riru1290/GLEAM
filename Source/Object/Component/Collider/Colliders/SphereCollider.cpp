#ifdef _DEBUG
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../Common/Transform.h"
#include "SphereCollider.h"

void SphereCollider::SetPosition(Position3D pos)
{
	sphere_.pos = pos;
	sphere_.UpdateAABB();
}

void SphereCollider::ApplyColliderUpdateToFollower()
{
	if (!follower_.has_value())return;

	auto& follower = follower_->get();

	Position3D& fPos = follower.pos;
	Scale3D& fScale = follower.scale;
	Quaternion& fQua = follower.qua;

	fPos = sphere_.pos - relativePos_;
	fScale = sphere_.scale;
	fQua = sphere_.qua;

	sphere_.UpdateAABB();
}

void SphereCollider::UpdateCollider()
{
	if (!follower_.has_value())return;

	const auto& follower = follower_->get();

	const Position3D& fPos = follower.pos;
	const Scale3D& fScale = follower.scale;
	const Quaternion& fQua = follower.qua;

	// äÓñ{èÓïÒ
	sphere_.pos = fPos + fQua.PosAxis(relativePos_);
	sphere_.scale = fScale;
	sphere_.qua = fQua;

	sphere_.radius = sphere_.defRadius * fScale.x;

	sphere_.UpdateAABB();
}

void SphereCollider::PreUpdateCollider()
{
	preSphere_ = sphere_;
}

void SphereCollider::DrawCollider()
{
#ifdef _DEBUG
	DebugRender.DrawSphere(sphere_.pos,sphere_.radius, 0xff0000, 255.0f, true);
#endif
}
