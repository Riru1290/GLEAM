#pragma once
#include "../Component.h"
#include "../../../Utility/Math/Quaternion.h"

class Transform;

/// @brief 設定されたオブジェクトを指定の回転値までゆっくり回転させる
class SmoothRotateController final : public Component
{
public:

	[[nodiscard]] SmoothRotateController();
	~SmoothRotateController() = default;

	/// @brief トランスフォーム設定
	/// @param transform トランスフォーム
	void SetTransform(Transform& transform);

	/// @brief 回転軸を固定するかを設定
	/// @param fixedState 回転軸を固定するか
	void SetFixedState(bool fixedState) { isFixedRotate_ = fixedState; }

	/// @brief 目標回転値を設定
	/// @param goalQua 目標回転値
	void SetGoalQuaternion(Quaternion goalQua)
	{
		isGoal_ = false;
		goalQua_ = goalQua;
	}

	void SetRelativePosition(Position3D relativePos)
	{
		relativePos_ = relativePos;
	}

	void SetRotatePow(float pow)
	{
		rotPow_ = pow;
	}

	/// @brief 目標値に到達しているか
	/// @return 目標値に到達しているか
	[[nodiscard]] const bool& IsGoal()const { return isGoal_; }

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::SMOOTHROTATE_CONTROLLER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent()override {}

	optional<reference_wrapper<Transform>> transform_;						/// @brief トランスフォーム情報

	Quaternion goalQua_;

	Position3D relativePos_;

	Position3D standardPos_;

	float rotPow_;

	bool isGoal_;

	bool isFixedRotate_;
};

