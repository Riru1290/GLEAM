#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

class ModelCollider final : public Collider
{
public:

	[[nodiscard]] ModelCollider() = default;
	~ModelCollider() = default;

	/// @brief モデル情報をセット
	/// @param info モデル情報
	void SetModelInfo(MODEL_COL_INFO info)
	{
		model_ = info;
		model_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] MODEL_COL_INFO& GetModelInfo() { return model_; }
	[[nodiscard]] MODEL_COL_INFO& GetPreModelInfo() { return preModel_; }

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::MODEL_COLLIDER; }

	/// @brief コライダーの形を取得
	/// @return コライダーの形
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::MODEL; }

	/// @brief 更新処理
	void UpdateCollider() override;

	/// @brief 通常Update前に走る処理
	void PreUpdateCollider() override;

	/// @brief コライダーの更新を追従対象に適用
	void ApplyColliderUpdateToFollower() override;

	MODEL_COL_INFO model_;			/// @brief モデル情報
	MODEL_COL_INFO preModel_;		/// @brief 1フレーム前モデル情報
};

