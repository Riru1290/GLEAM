#include "../Common/Transform.h"
#include "GravityController.h"

namespace
{
	// �ő�d�̓f�t�H���g
	const float GRAVITY_MAX_DEFAULT = (800.0f);

	// �d�͉����x�f�t�H���g
	const float GRAVITY_SPEED_DEFAULT = (600.0f);

	// �d�͕����f�t�H���g
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
	// �d�͉���
	gravity_ += MainTimer.GetMultDelta(gravityAccel_);
	gravity_ = min(gravity_, gravityMax_);

	// �d�͑���
	transform_->get().pos += 
		gravityDir_ * MainTimer.GetMultDelta(gravity_);
}

void GravityController::ReleaseComponent()
{
}
