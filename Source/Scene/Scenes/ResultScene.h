#pragma once
#include "../SceneBase.h"

/// @brief リザルトシーン
class ResultScene : public SceneBase
{
public:

	[[nodiscard]] ResultScene(bool isClear);
	~ResultScene() = default;

private:

	/// @brief 初期化処理
	void InitScene() override;

	/// @brief 更新処理
	void UpdateScene() override;

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdateScene() override;

	/// @brief 描画処理
	void DrawScene() override;

	/// @brief 解放処理
	void Terminate() override;

	/// @brief オブジェクト設定
	void ObjectSetting()override;

	/// @brief UI設定
	void UISetting() override;

	/// @brief カメラ設定
	void CameraSetting() override;

	/// @brief ライト設定
	void LightSetting() override;

	/// @brief イベント設定
	void EventSetting() override;

	bool isClear_;	/// @brief クリアしたかどうか
};

