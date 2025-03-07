#pragma once
#include "../CollisionHitData.h"

class PointCollider;
class LineCollider;
class SphereCollider;
class BoxCollider;
class CylinderCollider;
class ModelCollider;
class Rect2DCollider;

template<typename T>
concept ColliderClass =
is_same_v<T, PointCollider> ||
is_same_v<T, LineCollider> ||
is_same_v<T, SphereCollider> ||
is_same_v<T, BoxCollider> ||
is_same_v<T, CylinderCollider> ||
is_same_v<T, ModelCollider> ||
is_same_v<T, Rect2DCollider>;

template<ColliderClass T, ColliderClass U>
static inline pair<shared_ptr<T>, shared_ptr<U>> ConvertCollider(
	weak_ptr<Collider> colA, weak_ptr<Collider> colB, COLLISION_SHAPE_TYPE colAShape, COLLISION_SHAPE_TYPE colBShape)
{
	shared_ptr<T> retColA = nullptr;
	shared_ptr<U> retColB = nullptr;

	auto shapeA = colA.lock()->GetShapeType();
	auto shapeB = colB.lock()->GetShapeType();

	if (shapeA == colAShape && shapeB == colBShape)
	{
		retColA = static_pointer_cast<T>(colA.lock());
		retColB = static_pointer_cast<U>(colB.lock());
	}
	else if (shapeA == colBShape && shapeB == colAShape)
	{
		retColB = static_pointer_cast<U>(colA.lock());
		retColA = static_pointer_cast<T>(colB.lock());
	}
	else
	{
		// �ϊ��ł��Ȃ������ꍇ�A��O���X���[
		throw runtime_error("�w�肵���R���C�_�[�ɕϊ��ł��܂���ł���");
	}

	return { retColA ,retColB };
}
