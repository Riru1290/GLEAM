#pragma once
#include "../Component.h"

class Transform;

/// @brief 指定オブジェクト達との距離を計測する
class DistanceChecker final : public Component
{
public:

	[[nodiscard]] DistanceChecker();
	~DistanceChecker() = default;

	/// @brief 計測ターゲットの中で最も近いターゲットとの距離を返す
	/// @return 最も近いターゲットとの距離
	[[nodiscard]] float GetClosestTargetDistance();

	/// @brief トランスフォーム設定
	/// @param transform トランスフォーム
	void SetTransform(const Transform& transform)
	{
		transform_ = transform;
	}

	/// @brief 追従対象追加
	/// @param key 追従対象識別キー
	/// @param target 追従対象
	void AddTargetTransform(const string& key ,const Transform& target)
	{
		targets_[key] = target;

		distances_[key] = 0.0f;
	}

	/// @brief 追従対象削除
	/// @param key 追従対象識別キー
	void DeleteTargetTransform(const string& key)
	{
		targets_.erase(key);
		distances_.erase(key);
	}

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::DISTANCE_CHECKER; }

	/// @brief 更新処理
	void UpdateComponent() override {}

	/// @brief 解放処理
	void ReleaseComponent() override {}

	/// @brief 距離計測
	void Measurement();

	optional<reference_wrapper<const Transform>> transform_;							/// @brief 基準オブジェクト
	unordered_map<string,optional<reference_wrapper<const Transform>>> targets_;		/// @brief 計測対象

	unordered_map<string, float> distances_;									/// @brief 計測対象との距離
};

