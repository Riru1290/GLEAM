#pragma once
#include "../../Common/ConvertCollider.h"

class CollisionDetectFunction
{
public:

	CollisionDetectFunction() = default;
	virtual ~CollisionDetectFunction() = default;

	virtual bool Detection(
		weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData) = 0;

protected:

	//template<ColliderClass T, ColliderClass U>
	//pair<shared_ptr<T>, shared_ptr<U>> ConvertCollider(
	//	weak_ptr<Collider> colA, weak_ptr<Collider> colB, COLLISION_SHAPE_TYPE colAShape, COLLISION_SHAPE_TYPE colBShape)
	//{
	//	shared_ptr<T> retColA = nullptr;
	//	shared_ptr<U> retColB = nullptr;

	//	auto shapeA = colA.lock()->GetShapeType();
	//	auto shapeB = colB.lock()->GetShapeType();

	//	if (shapeA == colAShape && shapeB == colBShape)
	//	{
	//		retColA = static_pointer_cast<T>(colA.lock());
	//		retColB = static_pointer_cast<U>(colB.lock());
	//	}
	//	else if (shapeA == colBShape && shapeB == colAShape)
	//	{
	//		retColB = static_pointer_cast<U>(colA.lock());
	//		retColA = static_pointer_cast<T>(colB.lock());
	//	}
	//	else
	//	{
	//		// 変換できなかった場合、例外をスロー
	//		throw runtime_error("指定したコライダーに変換できませんでした");
	//	}

	//	return { retColA ,retColB };
	//}
};

