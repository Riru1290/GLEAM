#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

/// @brief 矩形(2D)コライダー
class Rect2DCollider final : public Collider
{
public:

	[[nodiscard]] Rect2DCollider() = default;
	~Rect2DCollider() = default;

	/// @brief 矩形(2D)情報をセット
	/// @param info 矩形(2D)情報
	void SetRectInfo(RECT_COL_INFO info)
	{
		rect_ = info;
		rect_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] RECT_COL_INFO& GetRectInfo() { return rect_; }
	[[nodiscard]] RECT_COL_INFO& GetPreRectInfo() { return preRect_; }

	/// @brief コライダーの更新を追従対象に適用
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::RECT2D_COLLIDER; }

	/// @brief コライダーの形を取得
	/// @return コライダーの形
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::RECT; }

	/// @brief 更新処理
	void UpdateCollider() override;

	/// @brief 通常Update前に走る処理
	void PreUpdateCollider() override;

	/// @brief 描画処理
	void DrawCollider() override;

	RECT_COL_INFO rect_;			/// @brief 矩形(2D)情報
	RECT_COL_INFO preRect_;		/// @brief 1フレーム前矩形(2D)情報
};

