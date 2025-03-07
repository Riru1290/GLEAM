#include "../../Common/Transform.h"
#include "Rect2DCollider.h"


void Rect2DCollider::ApplyColliderUpdateToFollower()
{
	auto& follower = follower_->get();

	Position3D& fPos = follower.pos;
	Scale3D& fScale = follower.scale;
	Quaternion& fQua = follower.qua;

	fPos = rect_.pos - relativePos_;
	fScale = rect_.scale;
	fQua = rect_.qua;

	rect_.UpdateAABB();
}

void Rect2DCollider::UpdateCollider()
{
	if (!follower_.has_value())return;

	const auto& follower = follower_->get();

	const Position3D& fPos = follower.pos;
	const Scale3D& fScale = follower.scale;
	const Quaternion& fQua = follower.qua;

	// äÓñ{èÓïÒ
	rect_.pos = fPos + relativePos_;
	rect_.scale = fScale;
	rect_.qua = fQua;

	rect_.width = rect_.defWidth * fScale.x;
	rect_.height = rect_.defHeight * fScale.y;

	rect_.UpdateAABB();
}

void Rect2DCollider::PreUpdateCollider()
{
	preRect_ = rect_;
}

void Rect2DCollider::DrawCollider()
{
#ifdef _DEBUG
	//DebugRender.DrawBox(box_.AABBpos.first, box_.AABBpos.second, 0xff00ff, 255.0f, true);
#endif
}
