#include "DetectFunction/Functions/PointRectCollisionDetectFunction.h"
#include "DetectFunction/Functions/LineSphereCollisionDetectFunction.h"
#include "DetectFunction/Functions/LineBoxCollisionDetectFunction.h"
#include "DetectFunction/Functions/LineModelCollisionDetectFunction.h"
#include "DetectFunction/Functions/SpheresCollisionDetectFunction.h"
#include "DetectFunction/Functions/BoxCylinderCollisionDetectFunction.h"
#include "DetectFunction/Functions/CylindersCollisionDetectFunction.h"
#include "DetectFunction/Functions/CylinderModelCollisionDetectFunction.h"
#include "CollisionDetector.h"

CollisionDetector::CollisionDetector()
{
	int index = static_cast<int>(COLLISION_SHAPE_TYPE::POINT) + static_cast<int>(COLLISION_SHAPE_TYPE::RECT);
	detectFunctions_[index] = move(make_unique<PointRectCollisionDetectFunction>());

	index = static_cast<int>(COLLISION_SHAPE_TYPE::LINE) + static_cast<int>(COLLISION_SHAPE_TYPE::SPHERE);
	detectFunctions_[index] = move(make_unique<LineSphereCollisionDetectFunction>());

	index = static_cast<int>(COLLISION_SHAPE_TYPE::LINE) + static_cast<int>(COLLISION_SHAPE_TYPE::BOX);
	detectFunctions_[index] = move(make_unique<LineBoxCollisionDetectFunction>());

	index = static_cast<int>(COLLISION_SHAPE_TYPE::LINE) + static_cast<int>(COLLISION_SHAPE_TYPE::MODEL);
	detectFunctions_[index] = move(make_unique<LineModelCollisionDetectFunction>());

	index = static_cast<int>(COLLISION_SHAPE_TYPE::SPHERE) + static_cast<int>(COLLISION_SHAPE_TYPE::SPHERE);
	detectFunctions_[index] = move(make_unique<SpheresCollisionDetectFunction>());

	index = static_cast<int>(COLLISION_SHAPE_TYPE::BOX) + static_cast<int>(COLLISION_SHAPE_TYPE::CYLINDER);
	detectFunctions_[index] = move(make_unique<BoxCylinderCollisionDetectFunction>());

	index = static_cast<int>(COLLISION_SHAPE_TYPE::CYLINDER) + static_cast<int>(COLLISION_SHAPE_TYPE::CYLINDER);
	detectFunctions_[index] = move(make_unique<CylindersCollisionDetectFunction>());

	index = static_cast<int>(COLLISION_SHAPE_TYPE::CYLINDER) + static_cast<int>(COLLISION_SHAPE_TYPE::MODEL);
	detectFunctions_[index] = move(make_unique<CylinderModelCollisionDetectFunction>());
}

CollisionDetector::~CollisionDetector()
{
}

bool CollisionDetector::Detection(int index, weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData)
{
	if (!detectFunctions_.contains(index))return false;

	return detectFunctions_.at(index)->Detection(colA, colB, hitData);
}
