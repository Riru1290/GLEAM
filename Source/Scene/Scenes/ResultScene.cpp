#include "../../Camera/Camera.h"
#include "../../Input/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Resource/ResourceManager.h"
#include "../../Collision/CollisionManager.h"
#include "../../Graphics/Light/LightController.h"
#include "../../Object/ObjectManager.h"
#include "../../Object/Map/LobbyRoom.h"
#include "../../Object/Props/TV.h"
#include "../../UI/UIManager.h"
#include "../../UI/Object/DefaultUI.h"
#include "../../UI/Object/CollisionUI.h"
#include "../../Game/Event/EventManager.h"
#include "../../Game/GameProgressManager.h"
#include "../../Sound/SoundManager.h"
#include "ResultScene.h"

namespace
{
	// ライト設定
	// ToDo 外部ファイルに
	const string SPOT_LIGHT_KEY = "SPOTLIGHT_RESULTSCENE";
	const SPOT_LIGHT SPOT_LIGHT_CONFIG = {
	.pos = { 0.0f,30.0f,200.0f },
	.range = 2500.0f ,
	.direction = Vector3(0.0f, 0.0f, -1.0f).Normalized(),
	.angle = Deg2Radian(45.0f),
	.color = { 1.0f,0.0f,0.0f }
	};

	// 入力設定
	const string INPUT_KEY_PRESS_BUTTONUI = "PRESS_BUTTONUI";

	// UI設定
	// ToDo 外部ファイルに
	const string UI_KEY_RESULT = "RESULT";
	const Position3D UI_POSITION_RESULT = { 830.0f,350.0f,0.0f };

	const string UI_KEY_CONTINUE = "CONTINUE";
	const Position3D UI_POSITION_CONTINUE = { 830.0f,550.0f,0.0f };

	const string UI_KEY_CONTINUEKEY = "CONTINUEKEY";
	const Position3D UI_POSITION_CONTINUEKEY = { 830.0f,600.0f,0.0f };

	const string UI_KEY_YES = "YES";
	const Position3D UI_POSITION_YES = { 630.0f,710.0f,0.0f };

	const string UI_KEY_NO = "NO";
	const Position3D UI_POSITION_NO = { 1000.0f,710.0f,0.0f };

	// オブジェクト設定
	const string OBJECT_KEY_TV = "TV";
	const Transform OBJECT_TRANSFORM_TV = { Position3D(0.0f,15.0f,0.0f),	Quaternion::Euler({0.0f,0.0f,0.0f}) };
	const int OBJECT_TV_TEXTURE_NUM_X = (2);
	const int OBJECT_TV_TEXTURE_NUM_Y = (1);

	const string OBJECT_KEY_ROOM = "ROOM";
}

ResultScene::ResultScene(bool isClear):SceneBase(),
isClear_(isClear)
{

}

void ResultScene::InitScene()
{
	InputMng.AddKey(INPUT_KEY_PRESS_BUTTONUI, { {INPUT_TYPE::Keyboard,VK_LBUTTON} });
	if (isClear_)
	{
		SoundMng.Play(RES_ID::BGM_CLEAR, PLAY_TYPE::LOOP);
	}
	else
	{
		SoundMng.Play(RES_ID::BGM_GAMEOVER, PLAY_TYPE::LOOP);
	}
}

void ResultScene::UpdateScene()
{
	ObjectMng.UpdateObjects();
	ObjectMng.LateUpdateObjects();

	CollisionMng.Collision(COLLISION_FASE::FIRST);
}

void ResultScene::OnCameraUpdateScene()
{
	ObjectMng.OnCameraUpdateObjects();
}

void ResultScene::DrawScene()
{
	ObjectMng.DrawObjects();
}

void ResultScene::Terminate()
{
	Light.DeleteSpotLightInfo(SPOT_LIGHT_KEY);
	ObjectMng.ReleaseObjects();
}

void ResultScene::ObjectSetting()
{
	ObjectMng.AddObject(make_shared<LobbyRoom>(OBJECT_KEY_ROOM));

	auto tv = make_shared<TV>(OBJECT_KEY_TV, RES_ID::IMAGE_TVSCREEN_RESULT, OBJECT_TV_TEXTURE_NUM_X, OBJECT_TV_TEXTURE_NUM_Y);
	tv->SetTransform(OBJECT_TRANSFORM_TV);
	ObjectMng.AddObject(tv);

	tv->ChangeTVChannel(isClear_);

	if (isClear_)
	{
		GameProgress.ResetGame();
	}
}

void ResultScene::UISetting()
{
	shared_ptr<UIObject> ui = make_shared<DefaultUI>();

	if (isClear_) {
		ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_YOUMADEITOUT));
		ui->SetPosition(UI_POSITION_RESULT);
		UIMng.AddUI(UI_KEY_RESULT, ui);
	}
	else
	{
		ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_YOUDIED));
		ui->SetPosition(UI_POSITION_RESULT);
		UIMng.AddUI(UI_KEY_RESULT, ui);

		ui = make_shared<DefaultUI>();
		ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_CONTINUEKEY));
		ui->SetPosition(UI_POSITION_CONTINUEKEY);
		UIMng.AddUI(UI_KEY_CONTINUEKEY, ui);
	}

	ui = make_shared<DefaultUI>();
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_CONTINUE));
	ui->SetPosition(UI_POSITION_CONTINUE);
	UIMng.AddUI(UI_KEY_CONTINUE, ui);

	ui = make_shared<CollisionUI>();
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_YES));
	ui->SetPosition(UI_POSITION_YES);
	static_pointer_cast<CollisionUI>(ui)->AddColliderOnHitFunction(OBJECT_ID::CURSOR,
		[&](COL_HIT_DATA data)
		{
			if (InputMng.Triggered(INPUT_KEY_PRESS_BUTTONUI))
			{
				EventMng.Notify(EVENT_ID::PRESS_TO_CONTINUE);
			}
		}
	);
	UIMng.AddUI(UI_KEY_YES, ui);

	ui = make_shared<CollisionUI>();
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_NO));
	ui->SetPosition(UI_POSITION_NO);
	static_pointer_cast<CollisionUI>(ui)->AddColliderOnHitFunction(OBJECT_ID::CURSOR,
		[&](COL_HIT_DATA data)
		{
			if (InputMng.Triggered(INPUT_KEY_PRESS_BUTTONUI))
			{
				EventMng.Notify(EVENT_ID::PRESS_TO_LOBBY);
			}
		}
	);
	UIMng.AddUI(UI_KEY_NO, ui);
}

void ResultScene::CameraSetting()
{
	MainCamera.ChangeState(Camera::STATE::FREE,true);
	MainCamera.SetPosition({ 0.0f,30.0f,55.0f });
	MainCamera.SetRotate(Quaternion::Euler({ 0.0f,Deg2Radian(190.0f),0.0f }));
}

void ResultScene::LightSetting()
{
	Light.AddSpotLight(SPOT_LIGHT_CONFIG, SPOT_LIGHT_KEY);
}

void ResultScene::EventSetting()
{
	EventMng.RegisterEventAction(EVENT_ID::PRESS_TO_CONTINUE, [&] {SceneMng.ChangeScene(SCENE_ID::PLAY); });
	EventMng.RegisterEventAction(EVENT_ID::PRESS_TO_LOBBY, [&] {
		SceneMng.ChangeScene(SCENE_ID::LOBBY);
		GameProgress.ResetGame(); });
}

