#pragma once
#include "../Component.h"
#include "../../../Graphics/Light/SpotLight.h"


class Transform;
class AnimationModelRenderer;

/// @brief 懐中電灯を持たせる
class FlashLightHolder final : public Component
{
public:

	[[nodiscard]] FlashLightHolder();
	~FlashLightHolder();

	/// @brief モデルレンダラー設定
	/// @param transform モデルレンダラー
	void SetFollowerTransform(const Transform& follower)
	{
		transform_ = follower;
	}

	void SetGoalSpotLightColor(Vector3 color,float time);

	void SetRelativePos(Position3D relativePos,bool isLarp) {
		if (isLarp) {
			startPos_ = relativePos;
		}
		else
		{
			relativePos_ = relativePos;
			startPos_ = relativePos_;
		}
	}

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::FLASHLIGHT_HOLDER; }

	/// @brief 更新処理
	void UpdateComponent() override {}

	/// @brief 解放処理
	void ReleaseComponent()override {}

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdateComponent() override;


	optional<reference_wrapper<const Transform>> transform_;		/// @brief アニメーションモデルレンダラー

	SPOT_LIGHT light_;

	Vector3 goalColor_;
	Vector3 nowColor_;

	Position3D relativePos_;
	Position3D startPos_;
};

