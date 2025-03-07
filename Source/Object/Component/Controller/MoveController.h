#pragma once
#include "../Component.h"

class Transform;

/// @brief 設定されたオブジェクトを移動させる
class MoveController final : public Component
{
public:

	[[nodiscard]] MoveController();
	~MoveController() = default;

	/// @brief トランスフォーム設定
	/// @param transform トランスフォーム
	void SetTransform(Transform& transform)
	{
		transform_ = transform;
	}

	/// @brief 止まった際のアクション設定
	/// @param action アクション
	void SetIdleAction(function<void()> action)
	{
		IdleAction_ = action;
	}

	/// @brief 歩いた際のアクション設定
	/// @param action アクション
	void SetWalkAction(function<void()> action)
	{
		WalkAction_ = action;
	}

	/// @brief 走った際のアクション設定
	/// @param action アクション
	void SetRunAction(function<void()> action)
	{
		RunAction_ = action;
	}

	/// @brief 歩く速さ設定
	/// @param speed 歩く速さ
	void SetWalkSpeed(float speed)
	{
		walkSpeed_ = speed;
	}

	/// @brief 走る速さ設定
	/// @param speed 走る速さ
	void SetRunSpeed(float speed)
	{
		runSpeed_ = speed;
	}

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::MOVE_CONTROLLER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent()override;

	optional<reference_wrapper<Transform>> transform_;						/// @brief トランスフォーム情報

	function<void()> IdleAction_;		/// @brief 止まった際の追加アクション
	function<void()> WalkAction_;		/// @brief 歩いた際の追加アクション
	function<void()> RunAction_;		/// @brief 走った際の追加アクション

	float walkSpeed_;					/// @brief 歩く速さ
	float runSpeed_;					/// @brief 走る速さ
};

