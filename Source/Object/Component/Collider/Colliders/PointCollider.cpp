#include "../../Common/Transform.h"
#include "PointCollider.h"

void PointCollider::ApplyColliderUpdateToFollower()
{
	auto& follower = follower_->get();

	Position3D& fPos = follower.pos;
	Scale3D& fScale = follower.scale;
	Quaternion& fQua = follower.qua;

	fPos = point_.pos - relativePos_;
	fScale = point_.scale;
	fQua = point_.qua;

	point_.UpdateAABB();
}

void PointCollider::UpdateCollider()
{
	if (!follower_.has_value())return;

	const auto& follower = follower_->get();

	const Position3D& fPos = follower.pos;
	const Scale3D& fScale = follower.scale;
	const Quaternion& fQua = follower.qua;

	// äÓñ{èÓïÒ
	point_.pos = fPos + relativePos_;
	point_.scale = fScale;
	point_.qua = fQua;

	point_.UpdateAABB();
}

void PointCollider::PreUpdateCollider()
{
	prePoint_ = point_;
}

void PointCollider::DrawCollider()
{
#ifdef _DEBUG
	//DebugRender.DrawBox(box_.AABBpos.first, box_.AABBpos.second, 0xff00ff, 255.0f, true);
#endif
}
