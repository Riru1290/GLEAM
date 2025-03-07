#pragma once
#include "../Component.h"
#include "../../../Utility/Math/Quaternion.h"

class Transform;
class AnimationModelRenderer;

/// @brief 設定されたオブジェクトの指定ボーンを回転させる
class RotateBoneController final : public Component
{
public:

	[[nodiscard]] RotateBoneController();
	~RotateBoneController() = default;

	/// @brief モデルレンダラー設定
	/// @param transform モデルレンダラー
	void SetAnimationModelRenderer(AnimationModelRenderer& modelRenderer)
	{
		modelRenderer_ = modelRenderer;
	}

	/// @brief マウスのX軸移動に対応した回転量を設定
	/// @param rotatePowMouseX マウスのX軸移動に対応した回転量
	void SetRotatePowMouseX(Vector3 rotatePowMouseX)
	{
		rotatePowMouseX_ = rotatePowMouseX;
	}

	/// @brief マウスのY軸移動に対応した回転量を設定
	/// @param rotatePowMouseY マウスのY軸移動に対応した回転量
	void SetRotatePowMouseY(Vector3 rotatePowMouseY)
	{
		rotatePowMouseY_ = rotatePowMouseY;
	}

	/// @brief 回転制御設定
	/// @param min 最小値
	/// @param max 最大値
	void SetRotateLimit(Vector3 min, Vector3 max, bool isLimitX, bool isLimitY, bool isLimitZ)
	{
		rotateMin_ = min;
		rotateMax_ = max;
		isLimitRotateX_ = isLimitX;
		isLimitRotateY_ = isLimitY;
		isLimitRotateZ_ = isLimitZ;
	}

	/// @brief 回転をリセット
	void ResetRotate();

	/// @brief 回転をセット
	/// @param rotate 回転
	void SetRotate(Quaternion rotate);

	/// @brief 回転させるボーン番号を設定
	/// @param no 回転させるボーン番号
	void SetBoneNo(int no)
	{
		boneNo_ = no;
	}

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ROTATEBONE_CONTROLLER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent()override;

	Quaternion boneRot_;

	int boneNo_;
	optional<reference_wrapper<AnimationModelRenderer>> modelRenderer_;		/// @brief アニメーションモデルレンダラー

	Vector3 rotatePowMouseX_;		/// @brief マウスX軸に対応した回転量
	Vector3 rotatePowMouseY_;		/// @brief マウスY軸に対応した回転量

	Vector3 rotateMin_;				/// @brief 回転量制限最小値
	Vector3 rotateMax_;				/// @brief 回転量制限最大値

	bool isLimitRotateX_;
	bool isLimitRotateY_;
	bool isLimitRotateZ_;

};

