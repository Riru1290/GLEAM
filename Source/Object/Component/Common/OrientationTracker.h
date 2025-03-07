#pragma once
#include "../Component.h"

class Transform;
class SmoothRotateController;

/// @brief 指定オブジェクトの方向を向き続ける
class OrientationTracker final : public Component
{
public:

	[[nodiscard]] OrientationTracker();
	~OrientationTracker() = default;

	/// @brief トランスフォーム設定
	/// @param transform トランスフォーム
	void SetTransform(Transform& transform);


	/// @brief 追従対象設定
	/// @param target 追従対象
	/// @param rotateOnlyOnY Y軸回転のみを行うか
	void SetTargetTransform(const Transform& target, bool rotateOnlyOnY = false);


private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ORIENTATION_TRACKER; }

	/// @brief 更新処理
	void UpdateComponent() override {}

	/// @brief 更新処理
	void LateUpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent() override {}

	/// @brief 向き追従
	void Track();

	optional<reference_wrapper<Transform>> transform_;	/// @brief 基準オブジェクト
	optional<reference_wrapper<const Transform>> target_;		/// @brief 追従対象

	unique_ptr<SmoothRotateController> rotateCon_;				/// @brief 回転コントローラ

	bool rotateOnlyOnY_;	/// @brief Y軸回転のみを行うか
};

