#include "Transform.h"
#include "DistanceChecker.h"

DistanceChecker::DistanceChecker() : Component()
{
}

float DistanceChecker::GetClosestTargetDistance()
{
	Measurement();

	float neaDis = D3D12_FLOAT32_MAX;

	for (const auto& [key, distance] : distances_)
	{
		if (distance < neaDis)
		{
			neaDis = distance;
		}
	}

	return neaDis;
}

void DistanceChecker::Measurement()
{
	if (!transform_.has_value())return;

	const Position3D& pos = transform_->get().pos;

	for (const auto& [key, target] : targets_)
	{
		const Position3D& tPos = target->get().pos;

		distances_[key] = (pos - tPos).Length();
	}
}
