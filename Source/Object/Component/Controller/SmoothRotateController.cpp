#include "../Common/Transform.h"
#include "SmoothRotateController.h"

SmoothRotateController::SmoothRotateController() :Component(),
rotPow_(300.0f),
goalQua_({}),
relativePos_({}),
isGoal_(true),
isFixedRotate_(false)
{
}

void SmoothRotateController::SetTransform(Transform& transform)
{
	transform_ = transform;

	standardPos_ = transform.pos;
}

void SmoothRotateController::UpdateComponent()
{
	if (!transform_.has_value() || isGoal_)return;


	transform_->get().qua = Quaternion::RotateTowards(
		transform_->get().qua, goalQua_, Deg2Radian(MainTimer.GetMultDelta(rotPow_))
	);

	if (isFixedRotate_)
	{
		transform_->get().pos = standardPos_ - relativePos_
			+ transform_->get().qua.PosAxis(relativePos_);
	}

	double dif = Quaternion::AngleBetween(transform_->get().qua, goalQua_);

	if (dif <= Deg2Radian(5.0))
	{
		isGoal_ = true;
	}

}
