#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

/// @brief シリンダーコライダー
class CylinderCollider final : public Collider
{
public:

	[[nodiscard]] CylinderCollider() = default;
	~CylinderCollider() = default;

	/// @brief シリンダー情報をセット
	/// @param info シリンダー情報
	void SetCylinderInfo(CYLINDER_COL_INFO info)
	{
		cylinder_ = info;
		cylinder_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] CYLINDER_COL_INFO& GetCylinderInfo() { return cylinder_; }
	[[nodiscard]] CYLINDER_COL_INFO& GetPreCylinderInfo() { return preCylinder_; }

	/// @brief コライダーの更新を追従対象に適用
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::CYLINDER_COLLIDER; }

	/// @brief コライダーの形を取得
	/// @return コライダーの形
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::CYLINDER; }

	/// @brief 更新処理
	void UpdateCollider() override;

	/// @brief 通常Update前に走る処理
	void PreUpdateCollider() override;

	/// @brief 描画処理
	void DrawCollider() override;

	CYLINDER_COL_INFO cylinder_;		/// @brief シリンダー情報
	CYLINDER_COL_INFO preCylinder_;		/// @brief 1フレーム前シリンダー情報

};

