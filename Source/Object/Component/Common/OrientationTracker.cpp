#include "../Common/Transform.h"
#include "../Controller/SmoothRotateController.h"
#include "OrientationTracker.h"

OrientationTracker::OrientationTracker()
{
	rotateCon_ = make_unique<SmoothRotateController>();
}

void OrientationTracker::SetTransform(Transform& transform)
{
	transform_ = transform;
	rotateCon_->SetTransform(transform);
}

void OrientationTracker::SetTargetTransform(const Transform& target, bool rotateOnlyOnY)
{
	rotateOnlyOnY_ = rotateOnlyOnY;
	target_ = target;
}

void OrientationTracker::LateUpdateComponent()
{
	Track();
}

void OrientationTracker::Track()
{
	Quaternion goal = {};

	Position3D self = transform_->get().pos;
	Position3D target = target_->get().pos;

	if (rotateOnlyOnY_)
	{
		self.y = 0.0f;
		target.y = 0.0f;
	}

	Vector3 toTargetVec = (target - self).Normalized();

	goal = Quaternion::LookRotation(toTargetVec);

	rotateCon_->SetGoalQuaternion(goal);

	rotateCon_->Update();
}
