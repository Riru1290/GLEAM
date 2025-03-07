#if defined(_DEBUG)
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_impl_win32.h"
#include "../../External/imgui/imgui_impl_dx12.h"
#include "../Input/InputManager.h"
#endif
#include "../Camera/Camera.h"
#include "../Camera/Camera2D.h"
#include "../Resource/ResourceManager.h"
#include "../Graphics/Fade/Fade.h"
#include "../Game/ClearChecker.h"
#include "../UI/UIManager.h"
#include "../Game/Event/EventManager.h"
#include "Scenes/LobbyScene.h"
#include "Scenes/PlayScene.h"
#include "Scenes/ResultScene.h"
#include "SceneManager.h"

SceneManager::SceneManager() :
	scene_(nullptr),
	sceneID_(SCENE_ID::LOBBY),
	nextSceneID_(SCENE_ID::LOBBY)
{
	DoChangeScene();

	fade_ = make_unique<Fade>();
	fade_->FadeOut(1.0f);

	clearChecker_ = make_unique<ClearChecker>();
}

SceneManager::~SceneManager()
{
}



void SceneManager::Terminate()
{
	scene_->Terminate();
}

void SceneManager::Execute()
{
#if defined(_DEBUG)

	string scene = "";
	switch (sceneID_)
	{
	case SCENE_ID::LOBBY:
		scene = "LOBBY";
		break;
	case SCENE_ID::PLAY:
		scene = "PLAY";
		break;
	case SCENE_ID::RESULT:
		scene = "RESULT";
		break;
	}

	ImGui::Text("Scene: %s", scene.c_str());
	ImGui::Text("Mouse: %f,%f", InputMng.GetMousePosition().x, InputMng.GetMousePosition().y);
#endif

	fade_->Update();
	if (fade_->IsEnd()) {
		DoChangeScene();
		fade_->FadeOut(1.0f);
	}
	else {
		scene_->Update();
		UIMng.Execute();
	}


	MainCamera.Update();
	MainCamera2D.Update();


	scene_->OnCameraUpdate();

	scene_->Draw();
	fade_->Draw();
}

void SceneManager::ChangeScene(SCENE_ID sceneID)
{
	if (nextSceneID_ == sceneID)return;

	nextSceneID_ = sceneID;

	fade_->FadeIn(1.0f);
}

void SceneManager::DoChangeScene()
{

	if (scene_ != nullptr)
	{
		EventMng.ResetEventActions();
		UIMng.ClearUIs();
		scene_->Terminate();
		scene_.reset();
	}

	ResourceMng.LoadAndRelease(nextSceneID_);
	

	switch (nextSceneID_)
	{
	case SCENE_ID::LOBBY:
		scene_ = make_unique<LobbyScene>();
		break;
	case SCENE_ID::PLAY:
		scene_ = make_unique<PlayScene>();
		break;
	case SCENE_ID::RESULT:
		scene_ = make_unique<ResultScene>(clearChecker_->IsClear());
		break;
	}

	scene_->Init();

	MainTimer.RecalculationDelta();

	sceneID_ = nextSceneID_;
}
