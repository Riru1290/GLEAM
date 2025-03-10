#pragma once
#include "../CollisionDetectFunction.h"

/// @brief ボックスとシリンダーの衝突判定
class BoxCylinderCollisionDetectFunction final : public CollisionDetectFunction
{
public:

	[[nodiscard]] BoxCylinderCollisionDetectFunction() = default;
	~BoxCylinderCollisionDetectFunction() = default;

	/// @brief 衝突判定
	/// @param colA コライダーA
	/// @param colB コライダーB
	/// @param hitData 衝突時データ
	/// @return 衝突しているかどうか
	bool Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData) override;
};

