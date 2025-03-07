#pragma once
#include "../Component.h"

class Transform;

/// @brief 重力を適用
class GravityController final : public Component
{
public:

	[[nodiscard]] GravityController();
	~GravityController() = default;

	/// @brief 重力を適用させるオブジェクトをセット
	/// @param transform 重力を適用させるオブジェクト
	void SetTransform(Transform& transform) { transform_ = transform; }

	// 重力変更
	void ChangeGravity(float gravity) { gravity_ = gravity; }

	// 最大重力変更
	void ChangeMaxGravity(float maxGravity) { gravityMax_ = maxGravity; }

	// 重力加速度変更
	void ChangeGravitySpeed(float gravitySpeed) { gravityAccel_ = gravitySpeed; }

	// 重力取得
	[[nodiscard]] const float& GetGravity()const { return gravity_; }

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::GRAVITY_CONTROLLER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent() override;

	optional<reference_wrapper<Transform>> transform_;		/// @brief 重力を適用させるオブジェクト

	float gravity_;			/// @brief 重力
	float gravityMax_;		/// @brief 最大重力
	float gravityAccel_;	/// @brief 重力加速度
	Vector3 gravityDir_;	/// @brief 重力方向
};

