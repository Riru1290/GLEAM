#pragma once
#include "../CollisionDetectFunction.h"

/// @brief シリンダーとモデルの衝突判定
class CylinderModelCollisionDetectFunction final : public CollisionDetectFunction
{
public:

	[[nodiscard]] CylinderModelCollisionDetectFunction() = default;
	~CylinderModelCollisionDetectFunction() = default;

	/// @brief 衝突判定
	/// @param colA コライダーA
	/// @param colB コライダーB
	/// @param hitData 衝突時データ
	/// @return 衝突しているかどうか
	bool Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData) override;

};

