#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "SceneID.h"

#define SceneMng SceneManager::GetInstance()

class SceneBase;
class Fade;
class ClearChecker;

/// @brief シーン管理
class SceneManager final : public StaticSingleton<SceneManager>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 実行
	void Execute();

	/// @brief シーン変更
	/// @param sceneID 遷移先シーンID
	void ChangeScene(SCENE_ID sceneID);

private:

	SceneManager();
	~SceneManager();
	PROOF_OF_STATIC_SINGLETON(SceneManager);

	/// @brief シーン遷移
	void DoChangeScene();

	unique_ptr<SceneBase> scene_;			/// @brief シーン
	unique_ptr<Fade> fade_;					/// @brief フェード
	unique_ptr<ClearChecker> clearChecker_;	/// @brief クリアチェッカー

	SCENE_ID sceneID_;				/// @brief 現在のシーンID
	SCENE_ID nextSceneID_;			/// @brief 遷移先シーンID
};

