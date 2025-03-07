#ifdef _DEBUG
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../Common/Transform.h"
#include "BoxCollider.h"

void BoxCollider::ApplyColliderUpdateToFollower()
{
	auto& follower = follower_->get();

	Position3D& fPos = follower.pos;
	Scale3D& fScale = follower.scale;
	Quaternion& fQua = follower.qua;

	fPos = box_.pos - relativePos_;
	fScale = box_.scale;
	fQua = box_.qua;

	box_.UpdateAABB();
}

void BoxCollider::UpdateCollider()
{
	if (!follower_.has_value())return;

	const auto& follower = follower_->get();

	const Position3D& fPos = follower.pos;
	const Scale3D& fScale = follower.scale;
	const Quaternion& fQua = follower.qua;

	// äÓñ{èÓïÒ
	box_.pos = fPos + relativePos_;
	box_.scale = fScale;
	box_.qua = fQua;

	box_.width = box_.defWidth * fScale.x;
	box_.height = box_.defHeight * fScale.y;
	box_.depth = box_.defDepth * fScale.z;

	box_.UpdateAABB();
}

void BoxCollider::PreUpdateCollider()
{
	preBox_ = box_;
}

void BoxCollider::DrawCollider()
{
#ifdef _DEBUG
	DebugRender.DrawBox(box_.AABBpos.first, box_.AABBpos.second, 0xff00ff, 255.0f, true);
#endif
}
