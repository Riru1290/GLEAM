#pragma once
#include "../CollisionResolveFunction.h"

/// @brief シリンダーとモデルの衝突後処理
class CylinderModelCollisionResolveFunction final : public CollisionResolveFunction
{
public:

	[[nodiscard]] CylinderModelCollisionResolveFunction() = default;
	~CylinderModelCollisionResolveFunction() = default;

	/// @brief 衝突時データを元に衝突後処理を行う
	/// @param data 衝突時データ
	void Resolve(COL_HIT_DATA data) override;

private:

	/// @brief シリンダー側が不動状態の場合の衝突後処理
	/// @param data 衝突時データ
	void ResolveStaticCylinder(COL_HIT_DATA data);

	/// @brief モデル側が不動状態の場合の衝突後処理
	/// @param data 衝突時データ
	void ResolveStaticModel(COL_HIT_DATA data);


};

