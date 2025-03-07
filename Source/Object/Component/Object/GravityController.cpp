#include "../Common/Transform.h"
#include "GravityController.h"

namespace
{
	// 最大重力デフォルト
	const float GRAVITY_MAX_DEFAULT = (800.0f);

	// 重力加速度デフォルト
	const float GRAVITY_SPEED_DEFAULT = (600.0f);

	// 重力方向デフォルト
	const Vector3 GRAVITY_DIR_DEFAULT = { 0.0f,-1.0f,0.0f };
}

GravityController::GravityController() : Component(),
	gravity_(0.0f),
	gravityMax_(GRAVITY_MAX_DEFAULT),
	gravityAccel_(GRAVITY_SPEED_DEFAULT), 
	gravityDir_(GRAVITY_DIR_DEFAULT)
{
}

void GravityController::UpdateComponent()
{
	// 重力加速
	gravity_ += MainTimer.GetMultDelta(gravityAccel_);
	gravity_ = min(gravity_, gravityMax_);

	// 重力操作
	transform_->get().pos += 
		gravityDir_ * MainTimer.GetMultDelta(gravity_);
}

void GravityController::ReleaseComponent()
{
}
