#pragma once

/// @brief シーン基底クラス
class SceneBase
{
public:

	SceneBase() = default;
	virtual ~SceneBase() = default;

	/// @brief 初期化処理
	void Init();

	/// @brief 更新処理
	void Update();

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdate();

	/// @brief 描画処理
	void Draw();

	/// @brief 解放処理
	virtual void Terminate() = 0;

protected:

	/// @brief 初期化処理
	virtual void InitScene() = 0;

	/// @brief 更新処理
	virtual void UpdateScene() = 0;

	/// @brief カメラ更新後に走る処理
	virtual void OnCameraUpdateScene() = 0;

	/// @brief 描画処理
	virtual void DrawScene() = 0;

	/// @brief オブジェクト設定
	virtual void ObjectSetting() = 0;

	/// @brief UI設定
	virtual void UISetting() = 0;

	/// @brief カメラ設定
	virtual void CameraSetting() = 0;

	/// @brief ライト設定
	virtual void LightSetting() = 0;

	/// @brief イベント設定
	virtual void EventSetting() = 0;
};

