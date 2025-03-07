#pragma once
#include "../CollisionResolveFunction.h"

/// @brief ボックスとシリンダーの衝突後処理
class BoxCylinderCollisionResolveFunction final : public CollisionResolveFunction
{
public:

	[[nodiscard]] BoxCylinderCollisionResolveFunction() = default;
	~BoxCylinderCollisionResolveFunction() = default;

	/// @brief 衝突時データを元に衝突後処理を行う
	/// @param data 衝突時データ
	void Resolve(COL_HIT_DATA data) override;

private:

	/// @brief ボックス側が不動状態の場合の衝突後処理
	/// @param data 衝突時データ
	void ResolveStaticBox(COL_HIT_DATA data);

	/// @brief シリンダー側が不動状態の場合の衝突後処理
	/// @param data 衝突時データ
	void ResolveStaticCylinder(COL_HIT_DATA data);


};

