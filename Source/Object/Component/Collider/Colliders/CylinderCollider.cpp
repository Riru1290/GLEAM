#ifdef _DEBUG
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../Common/Transform.h"
#include "CylinderCollider.h"

void CylinderCollider::ApplyColliderUpdateToFollower()
{
	auto& follower = follower_->get();

	Position3D& fPos = follower.pos;
	Scale3D& fScale = follower.scale;
	Quaternion& fQua = follower.qua;

	fPos = cylinder_.pos - relativePos_;
	fScale = cylinder_.scale;
	fQua = cylinder_.qua;

	cylinder_.UpdateAABB();
}

void CylinderCollider::UpdateCollider()
{
	if (!follower_.has_value())return;

	const auto& follower = follower_->get();

	const Position3D& fPos = follower.pos;
	const Scale3D& fScale = follower.scale;
	const Quaternion& fQua = follower.qua;

	// äÓñ{èÓïÒ
	cylinder_.pos = fPos + relativePos_;
	cylinder_.scale = fScale;
	cylinder_.qua = fQua;

	// çÇÇ≥
	cylinder_.height = cylinder_.defHeight * fScale.x;

	// îºåa
	cylinder_.radius = cylinder_.defRadius * fScale.x;

	cylinder_.UpdateAABB();
}

void CylinderCollider::PreUpdateCollider()
{
	preCylinder_ = cylinder_;
}

void CylinderCollider::DrawCollider()
{
#ifdef _DEBUG
	DebugRender.DrawCylinder(cylinder_.pos, cylinder_.height, cylinder_.radius, cylinder_.qua.ToEuler(), 0xff00ff, 255.0f, true);
#endif
}
