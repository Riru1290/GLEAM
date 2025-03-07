#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

class LineCollider final : public Collider
{
public:

	[[nodiscard]] LineCollider() = default;
	~LineCollider() = default;

	/// @brief シリンダー情報をセット
	/// @param info シリンダー情報
	void SetLineInfo(LINE_COL_INFO info)
	{
		line_ = info;
		line_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] LINE_COL_INFO& GetLineInfo() { return line_; }
	[[nodiscard]] LINE_COL_INFO& GetPreLineInfo() { return preLine_; }

	/// @brief コライダーの更新を追従対象に適用
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::LINE_COLLIDER; }

	/// @brief コライダーの形を取得
	/// @return コライダーの形
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::LINE; }

	/// @brief 更新処理
	void UpdateCollider() override;

	/// @brief 通常Update前に走る処理
	void PreUpdateCollider() override;

	/// @brief 描画処理
	void DrawCollider() override;

	LINE_COL_INFO line_;		/// @brief 線分情報
	LINE_COL_INFO preLine_;		/// @brief 1フレーム前線分情報
};

