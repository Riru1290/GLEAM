#pragma once
#include "../Component.h"

class Transform;

/// @brief 設定されたオブジェクトを回転させる
class RotateController final : public Component
{
public:

	[[nodiscard]] RotateController();
	~RotateController() = default;

	/// @brief トランスフォーム設定
	/// @param transform トランスフォーム
	void SetTransform(Transform& transform)
	{
		transform_ = transform;
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

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ROTATE_CONTROLLER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent()override;

	optional<reference_wrapper<Transform>> transform_;						/// @brief トランスフォーム情報

	Vector3 rotatePowMouseX_;		/// @brief マウスX軸に対応した回転量
	Vector3 rotatePowMouseY_;		/// @brief マウスY軸に対応した回転量


};

