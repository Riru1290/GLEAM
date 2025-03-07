#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

/// @brief ボックスコライダー
class BoxCollider final : public Collider
{
public:

	[[nodiscard]] BoxCollider() = default;
	~BoxCollider() = default;

	/// @brief ボックス情報をセット
	/// @param info ボックス情報
	void SetBoxInfo(BOX_COL_INFO info)
	{
		box_ = info;
		box_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] BOX_COL_INFO& GetBoxInfo() { return box_; }
	[[nodiscard]] BOX_COL_INFO& GetPreBoxInfo() { return preBox_; }

	/// @brief コライダーの更新を追従対象に適用
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::BOX_COLLIDER; }

	/// @brief コライダーの形を取得
	/// @return コライダーの形
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::BOX; }

	/// @brief 更新処理
	void UpdateCollider() override;

	/// @brief 通常Update前に走る処理
	void PreUpdateCollider() override;

	/// @brief 描画処理
	void DrawCollider() override;

	BOX_COL_INFO box_;			/// @brief ボックス情報
	BOX_COL_INFO preBox_;		/// @brief 1フレーム前ボックス情報
};

