#include "../../Camera/Camera.h"
#include "../../Input/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Resource/ResourceManager.h"
#include "../../Graphics/RenderManager.h"
#include "../../Graphics/Light/LightController.h"
#include "../../Object/Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../../Object/Component/Renderer/Renderers/Single/2D/SpriteRenderer.h"
#include "../../Object/Component/Renderer/Renderers/Single/2D/Object2DRenderer.h"
#include "../../Object/Props/TV.h"
#include "../../Object/Map/LobbyRoom.h"
#include "../../Object/Actor/Player/VisualPlayer.h"
#include "../../Object/ObjectManager.h"
#include "../../Object/Component/Common/Transform.h"
#include "../../UI/Object/FieldUI.h"
#include "../../UI/Object/DefaultUI.h"
#include "../../UI/Object/FadeUI.h"
#include "../../UI/Object/BlinkUI.h"
#include "../../UI/Object/Unique/ButtonUI.h"
#include "../../UI/Object/Unique/LevelButtonUI.h"
#include "../../UI/Object/Unique/GameStartUI.h"
#include "../../UI/UIManager.h"
#include "../../Object/Component/Collider/Colliders/Rect2DCollider.h"
#include "../../Game/Event/EventManager.h"
#include "../../Collision/CollisionManager.h"
#include "../../Sound/SoundManager.h"
#include "../../Object/Effect/EffectController.h"
#include "LobbyScene.h"

namespace
{
	// ライト設定
	// ToDo 外部ファイルに
	const string SPOT_LIGHT_KEY = "SPOTLIGHT_LOBBYSCENE";
	const SPOT_LIGHT SPOT_LIGHT_CONFIG = {
	.pos = { 100.0f,150.0f,50.0f },
	.range = 2500.0f ,
	.direction = Vector3(0.0f, -1.0f, -1.0f).Normalized(),
	.angle = Deg2Radian(50.0f),
	.color = { 1.0f,0.0f,0.0f }
	};

	// 入力
	const string INPUT_KEY_CHANGESCENE = "CHANGE_SCENE";


	// UI設定
	// ToDo 外部ファイルに
	const string UI_KEY_TITLELOGO = "TITLELOGO";
	const Position3D UI_POSITION_TITLELOGO = { 10.0f,80.0f,-160.0f };
	const Quaternion UI_ROTATE_TITLELOGO = Quaternion::Euler({ 0.0f,Deg2Radian(180.0f),0.0f });

	const string UI_KEY_PRESS = "PRESS";
	const Position3D UI_POSITION_PRESS = { 10.0f,30.0f,-160.0f };
	const Quaternion UI_ROTATE_PRESS = Quaternion::Euler({ 0.0f,Deg2Radian(180.0f),0.0f });
	const float UIBLINK_SPEED_PRESS = (2.5f);

	const string UI_KEY_GAMESTART = "GAMESTART";
	const Position3D UI_POSITION_GAMESTART = { 1500.0f,800.0f,0.0f };

	const string UI_KEY_CHANNEL = "CHANNEL";
	const Position3D UI_POSITION_CHANNEL = { 1500.0f,200.0f,0.0f };
	const float UIFADE_FADEIN_TIME_CHANNEL = (0.3f);
	const float UIFADE_FADEOUT_TIME_CHANNEL = (0.3f);

	const string UI_KEY_LEVEL = "LEVEL";
	const Position3D UI_POSITION_LEVEL = { 1500.0f,500.0f,0.0f };
	const float UIFADE_FADEIN_TIME_LEVEL = (0.3f);
	const float UIFADE_FADEOUT_TIME_LEVEL = (0.3f);

	const string UI_KEY_BUTTON = "BUTTON";
	const Position3D UI_POSITION_BUTTON = { 1500.0f - (64.0f * (5.0f / 2.0f)) + 32.0f,300.0f,0.0f };
	const int UIBUTTON_NUM = (10);
	const float UIBUTTON_OFFSET = (64.0f);

	const string UI_KEY_BUTTONLEVEL = "BUTTONLEVEL";
	const Position3D UI_POSITION_BUTTONLEVEL = { 1500.0f - (64.0f * 5.0f / 2.0f) + 32.0f,600.0f,0.0f };
	const int UIBUTTONLEVEL_NUM = (10);
	const float UIBUTTONLEVEL_OFFSET = (64.0f);

	// カメラ設定
	const Transform CAMERA_TRANSFORM_TITLE = { Position3D(38.0f,40.0f,99.0f) ,Quaternion::Euler({ Deg2Radian(-2.0f),Deg2Radian(175.0f), Deg2Radian(-5.0f) }) };
	const Transform CAMERA_TRANSFORM_LOBBY = { Position3D(52.0f,23.0f,6.0f) ,Quaternion::Euler({ Deg2Radian(-6.0f),Deg2Radian(129.0f), Deg2Radian(2.0f) }) };

	// オブジェクト設定
	const string OBJECT_KEY_TV = "TV";
	const Transform OBJECT_TRANSFORM_TV = { Position3D(100.0f,13.0f,-20.0f),	Quaternion::Euler({0.0f,Deg2Radian(-30.0f),0.0f}) };
	const int OBJECT_TV_TEXTURE_NUM_X = (3);
	const int OBJECT_TV_TEXTURE_NUM_Y = (3);

	const string OBJECT_KEY_ROOM = "ROOM";

	const string OBJECT_KEY_PLAYER = "PLAYER";
	const Transform OBJECT_TRANSFORM_PLAYER = { Position3D(125.0f,15.0f,-100.0f),	Quaternion::Euler({0.0f,0.0f,0.0f}) };
}

LobbyScene::LobbyScene():
	isLobby_(false),
	effectHandle_(-1),
	bgmHandle_(-1)
{
}

void LobbyScene::InitScene()
{
	ShowCursor(false);
	isLobby_ = false;

	InputMng.AddKey(INPUT_KEY_CHANGESCENE, { {INPUT_TYPE::Keyboard,VK_SPACE  } });

	bgmHandle_ = SoundMng.Play(RES_ID::BGM_LOBBY, PLAY_TYPE::LOOP);

	effectHandle_ = EffectCon.PlayEffect(EFFECT_ID::ATMOSPHERE, {});
}

void LobbyScene::UpdateScene()
{
	ObjectMng.UpdateObjects();
	ObjectMng.LateUpdateObjects();

	CollisionMng.Collision(COLLISION_FASE::FIRST);

	if (MainCamera.IsEndLerp())
	{

		if (!isLobby_ && InputMng.AnyTriggered())
		{
			EventMng.Notify(EVENT_ID::CHANGE_TO_LOBBY);
			MainCamera.SetLerpConfig(
				CAMERA_TRANSFORM_TITLE,
				CAMERA_TRANSFORM_LOBBY,
				0.5f
			);
			isLobby_ = true;

		}
		else if (isLobby_ && InputMng.Hold(0.5f, INPUT_KEY_CHANGESCENE))
		{
			EventMng.Notify(EVENT_ID::CHANGE_TO_TITLE);
			MainCamera.SetLerpConfig(
				CAMERA_TRANSFORM_LOBBY,
				CAMERA_TRANSFORM_TITLE,
				0.5f
			);
			isLobby_ = false;
		}
	}
}

void LobbyScene::OnCameraUpdateScene()
{
	ObjectMng.OnCameraUpdateObjects();
}

void LobbyScene::DrawScene()
{
	ObjectMng.DrawObjects();
}

void LobbyScene::Terminate()
{
	Light.DeleteSpotLightInfo(SPOT_LIGHT_KEY);
	ObjectMng.ReleaseObjects();
	SoundMng.DeleteSound(bgmHandle_);
	EffectCon.StopEffect(effectHandle_);

#ifdef _DEBUG
	RenderMng.DeleteRenderer(depth_);
#endif
}

void LobbyScene::ObjectSetting()
{
#ifdef _DEBUG
	depth_ =
		RenderMng.CreateRenderer<Object2DRenderer>(LAYER::UI);
	depth_->CreateMesh(Size2D(192.0f * 3.0f, 108.0f * 3.0f));
	Transform d = {};
	d.pos = { 192.0f * 1.5f,108.0f * 1.5f,0.0f };
	depth_->SetMeshTransformMatrixFromTransform(d);
	depth_->GetMesh().GetMaterial().SetPSOID(PSO_ID::SHADOW_2D);
#endif

	// TV
	auto tv = make_shared<TV>(OBJECT_KEY_TV, RES_ID::IMAGE_TVSCREEN_LOBBY, OBJECT_TV_TEXTURE_NUM_X, OBJECT_TV_TEXTURE_NUM_Y);
	tv->SetTransform(OBJECT_TRANSFORM_TV);
	ObjectMng.AddObject(tv);


	// ルーム
	ObjectMng.AddObject(make_shared<LobbyRoom>(OBJECT_KEY_ROOM));

	// プレイヤー
	auto player = make_shared<VisualPlayer>(OBJECT_KEY_PLAYER, RES_ID::ANIMATION_PLAYER_SITTINGIDLE);
	player->SetTransform(OBJECT_TRANSFORM_PLAYER);
	ObjectMng.AddObject(player);


	ObjectMng.InitObjects();
}

void LobbyScene::UISetting()
{
	shared_ptr<UIObject> ui = make_shared<FieldUI>();
	ui->SetTexture<MeshRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_TITLELOGO));
	ui->SetPosition(UI_POSITION_TITLELOGO);
	ui->SetRotation(UI_ROTATE_TITLELOGO);
	UIMng.AddUI(UI_KEY_TITLELOGO, ui);

	ui = make_shared<BlinkUI>();
	ui->SetTexture<MeshRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_PRESS));
	ui->SetPosition(UI_POSITION_PRESS);
	ui->SetRotation(UI_ROTATE_PRESS);
	static_pointer_cast<BlinkUI>(ui)->SetBlinkSpeed(UIBLINK_SPEED_PRESS);
	UIMng.AddUI(UI_KEY_PRESS, ui);

	ui = make_shared<FadeUI>(true);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_CHANNEL));
	ui->SetPosition(UI_POSITION_CHANNEL);
	UIMng.AddUI(UI_KEY_CHANNEL, ui);

	ui = make_shared<FadeUI>(true);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_LEVEL));
	ui->SetPosition(UI_POSITION_LEVEL);
	UIMng.AddUI(UI_KEY_LEVEL, ui);

	ui = make_shared<GameStartUI>();
	ui->SetPosition(UI_POSITION_GAMESTART);
	UIMng.AddUI(UI_KEY_GAMESTART, ui);

	// ボタン
	Position3D buttonBasePos = UI_POSITION_BUTTON;
	for (int i = 0; i < UIBUTTON_NUM; i++)
	{
		EVENT_ID id = static_cast<EVENT_ID>(static_cast<int>(EVENT_ID::PRESS_BUTTON0) + i);
		ui = make_shared<ButtonUI>(id, i, i + UIBUTTON_NUM);
		Position3D buttonPos = buttonBasePos +
			Position3D(static_cast<float>(
				i % (UIBUTTON_NUM / 2)) * UIBUTTON_OFFSET,
				static_cast<float>(i / (UIBUTTON_NUM / 2)) * UIBUTTON_OFFSET, 0.0f);
		ui->SetPosition(buttonPos);
		UIMng.AddUI(UI_KEY_BUTTON + to_string(i), ui);
	}

	buttonBasePos = UI_POSITION_BUTTONLEVEL;
	for (int i = 0; i < UIBUTTONLEVEL_NUM; i++)
	{
		ui = make_shared<LevelButtonUI>(i, i, i + UIBUTTONLEVEL_NUM);
		Position3D buttonPos = buttonBasePos +
			Position3D(static_cast<float>(
				i % (UIBUTTONLEVEL_NUM / 2)) * UIBUTTONLEVEL_OFFSET,
				static_cast<float>(i / (UIBUTTONLEVEL_NUM / 2)) * UIBUTTONLEVEL_OFFSET, 0.0f);
		ui->SetPosition(buttonPos);
		UIMng.AddUI(UI_KEY_BUTTONLEVEL + to_string(i), ui);
	}

	static_pointer_cast<LevelButtonUI>(UIMng.GetUI(UI_KEY_BUTTONLEVEL + to_string(5)).lock())->PressButton();
	static_pointer_cast<ButtonUI>(UIMng.GetUI(UI_KEY_BUTTON + to_string(0)).lock())->PressButton();
}
 
void LobbyScene::CameraSetting()
{
	MainCamera.ChangeState(Camera::STATE::LERP);
	MainCamera.SetLerpConfig(
		CAMERA_TRANSFORM_LOBBY,
		CAMERA_TRANSFORM_TITLE,
		0.01f
	);
}

void LobbyScene::LightSetting()
{
	Light.AddSpotLight(SPOT_LIGHT_CONFIG, SPOT_LIGHT_KEY,true);
}

void LobbyScene::EventSetting()
{
	EventMng.RegisterEventAction(EVENT_ID::CHANGE_TO_LOBBY, [&] {
		static_pointer_cast<FadeUI>(UIMng.GetUI(UI_KEY_CHANNEL).lock())->FadeIn(UIFADE_FADEIN_TIME_CHANNEL);
		static_pointer_cast<FadeUI>(UIMng.GetUI(UI_KEY_LEVEL).lock())->FadeIn(UIFADE_FADEIN_TIME_LEVEL);
		SoundMng.Play(RES_ID::SOUND_GOTOLOBBY);
		ShowCursor(true);
		});

	EventMng.RegisterEventAction(EVENT_ID::CHANGE_TO_TITLE, [&] {
		static_pointer_cast<FadeUI>(UIMng.GetUI(UI_KEY_CHANNEL).lock())->FadeOut(UIFADE_FADEOUT_TIME_CHANNEL);
		static_pointer_cast<FadeUI>(UIMng.GetUI(UI_KEY_LEVEL).lock())->FadeOut(UIFADE_FADEOUT_TIME_LEVEL);
		SoundMng.Play(RES_ID::SOUND_GOTOTITLE);
		ShowCursor(false);
		});

	EventMng.RegisterEventAction(EVENT_ID::PRESS_LEVELBUTTON, [&] {
		for (int i = 0; i < UIBUTTONLEVEL_NUM; i++)
		{
			static_pointer_cast<LevelButtonUI>(UIMng.GetUI(UI_KEY_BUTTONLEVEL + to_string(i)).lock())->ReleaseButton();
		}
		});

	EventMng.RegisterEventAction(EVENT_ID::PRESS_TUTORIALBUTTON, [&] {
		for (int i = 0; i < UIBUTTON_NUM; i++)
		{
			static_pointer_cast<LevelButtonUI>(UIMng.GetUI(UI_KEY_BUTTON + to_string(i)).lock())->ReleaseButton();
		}
		});

	EventMng.RegisterEventAction(EVENT_ID::PRESS_BUTTON0, [&] {
		static_pointer_cast<TV>(ObjectMng.GetSpecificObject(OBJECT_KEY_TV))->ChangeTVChannel(0);
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});
	EventMng.RegisterEventAction(EVENT_ID::PRESS_BUTTON1, [&] {
		static_pointer_cast<TV>(ObjectMng.GetSpecificObject(OBJECT_KEY_TV))->ChangeTVChannel(1);
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});
	EventMng.RegisterEventAction(EVENT_ID::PRESS_BUTTON2, [&] {
		static_pointer_cast<TV>(ObjectMng.GetSpecificObject(OBJECT_KEY_TV))->ChangeTVChannel(2);
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});
	EventMng.RegisterEventAction(EVENT_ID::PRESS_BUTTON3, [&] {
		static_pointer_cast<TV>(ObjectMng.GetSpecificObject(OBJECT_KEY_TV))->ChangeTVChannel(3);
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});
	EventMng.RegisterEventAction(EVENT_ID::PRESS_BUTTON4, [&] {
		static_pointer_cast<TV>(ObjectMng.GetSpecificObject(OBJECT_KEY_TV))->ChangeTVChannel(4);
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});
	EventMng.RegisterEventAction(EVENT_ID::PRESS_BUTTON5, [&] {
		static_pointer_cast<TV>(ObjectMng.GetSpecificObject(OBJECT_KEY_TV))->ChangeTVChannel(5);
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});
	EventMng.RegisterEventAction(EVENT_ID::PRESS_BUTTON6, [&] {
		static_pointer_cast<TV>(ObjectMng.GetSpecificObject(OBJECT_KEY_TV))->ChangeTVChannel(6);
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});
	EventMng.RegisterEventAction(EVENT_ID::PRESS_BUTTON7, [&] {
		static_pointer_cast<TV>(ObjectMng.GetSpecificObject(OBJECT_KEY_TV))->ChangeTVChannel(7);
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});
	EventMng.RegisterEventAction(EVENT_ID::PRESS_BUTTON8, [&] {
		static_pointer_cast<TV>(ObjectMng.GetSpecificObject(OBJECT_KEY_TV))->ChangeTVChannel(8);
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});
	EventMng.RegisterEventAction(EVENT_ID::PRESS_BUTTON9, [&] {
		static_pointer_cast<TV>(ObjectMng.GetSpecificObject(OBJECT_KEY_TV))->ChangeTVChannel(0);
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});

	EventMng.RegisterEventAction(EVENT_ID::PRESS_LEVELBUTTON, [&] {
		SoundMng.Play(RES_ID::SOUND_CLICK);
		});

	EventMng.RegisterEventAction(EVENT_ID::PRESS_GAMESTART, [&] {
		SceneMng.ChangeScene(SCENE_ID::PLAY);
		SoundMng.Play(RES_ID::SOUND_SCENECHANGE);
		});
}
