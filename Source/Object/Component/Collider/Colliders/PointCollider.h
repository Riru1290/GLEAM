#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

/// @brief 点コライダー
class PointCollider final : public Collider
{
public:

	[[nodiscard]] PointCollider() = default;
	~PointCollider() = default;

	/// @brief 点情報をセット
	/// @param info 点情報
	void SetPointInfo(POINT_COL_INFO info)
	{
		point_ = info;
		point_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] POINT_COL_INFO& GetPointInfo() { return point_; }
	[[nodiscard]] POINT_COL_INFO& GetPrePointInfo() { return prePoint_; }

	/// @brief コライダーの更新を追従対象に適用
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::BOX_COLLIDER; }

	/// @brief コライダーの形を取得
	/// @return コライダーの形
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::POINT; }

	/// @brief 更新処理
	void UpdateCollider() override;

	/// @brief 通常Update前に走る処理
	void PreUpdateCollider() override;

	/// @brief 描画処理
	void DrawCollider() override;

	POINT_COL_INFO point_;			/// @brief 点情報
	POINT_COL_INFO prePoint_;		/// @brief 1フレーム前点情報
};

