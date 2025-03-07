#pragma once
#include "../Object.h"

class AnimationController;
class AnimationModelRenderer;

/// @brief アニメーションするオブジェクトの基底クラス
class Actor : public Object
{
public:

	Actor(const string& uniqueKey);
	virtual ~Actor() = default;

protected:

	/// @brief 繰り返し初期化処理
	virtual void InitActor() {}

	/// @brief 描画処理
	virtual void DrawActor() {}

	/// @brief 基本的な更新処理
	virtual void UpdateActor() = 0;

	/// @brief 通常Update後に走る処理
	virtual void LateUpdateActor() {}

	/// @brief カメラ更新後に走る処理
	virtual void OnCameraUpdateActor() {}

	/// @brief アニメーション設定
	virtual void AnimationSetting() = 0;

	// 基本コンポーネント--------------------

	optional<reference_wrapper<AnimationModelRenderer>> modelRenderer_;		/// @brief アニメーションモデルレンダラー
	optional<reference_wrapper<AnimationController>> animationController_;	/// @brief アニメーションコントローラ

	//---------------------------------------

private:

	/// @brief 繰り返し初期化処理
	void InitObject() override final;

	/// @brief 描画処理
	void DrawObject() override final;

	/// @brief 基本的な更新処理
	void UpdateObject() override final;

	/// @brief 通常Update後に走る処理
	void LateUpdateObject() override final;

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdateObject() override final;

	/// @brief アニメーション初期化
	void InitAnimation();
};

