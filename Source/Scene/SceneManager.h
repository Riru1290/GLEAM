#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "SceneID.h"

#define SceneMng SceneManager::GetInstance()

class SceneBase;
class Fade;
class ClearChecker;

/// @brief �V�[���Ǘ�
class SceneManager final : public StaticSingleton<SceneManager>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief ���s
	void Execute();

	/// @brief �V�[���ύX
	/// @param sceneID �J�ڐ�V�[��ID
	void ChangeScene(SCENE_ID sceneID);

private:

	SceneManager();
	~SceneManager();
	PROOF_OF_STATIC_SINGLETON(SceneManager);

	/// @brief �V�[���J��
	void DoChangeScene();

	unique_ptr<SceneBase> scene_;			/// @brief �V�[��
	unique_ptr<Fade> fade_;					/// @brief �t�F�[�h
	unique_ptr<ClearChecker> clearChecker_;	/// @brief �N���A�`�F�b�J�[

	SCENE_ID sceneID_;				/// @brief ���݂̃V�[��ID
	SCENE_ID nextSceneID_;			/// @brief �J�ڐ�V�[��ID
};

