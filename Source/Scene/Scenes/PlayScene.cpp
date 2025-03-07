#if defined(_DEBUG)
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_impl_win32.h"
#include "../../External/imgui/imgui_impl_dx12.h"
#include "../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../Camera/Camera.h"
#include "../../Input/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Collision/CollisionManager.h"
#include "../../Object/Actor/Player/Player.h"
//#include "../../Object/Actor/Enemy/SlenderManEnemy.h"
//#include "../../Object/Actor/Enemy/GirlEnemy.h"
//#include "../../Object/Actor/Enemy/MannequinEnemy.h"
//#include "../../Object/Actor/Enemy/ZombieEnemy.h"
//#include "../../Object/Map/Map.h"
//#include "../../Object/Props/LightBulb.h"
//#include "../../Object/Props/Door.h"
//#include "../../Object/Props/Key.h"
//#include "../../Object/Props/Padlock.h"
//#include "../../Object/Props/Locker.h"
//#include "../../Object/Map/Wave/Wave.h"
//#include "../../Object/Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
//#include "../../Object/Component/Renderer/Renderers/Single/2D/Object2DRenderer.h"
//#include "../../Object/Component/Actor/Algorithm/AStarChaser.h"
//#include "../../Object/Component/Common/OrientationTracker.h"
#include "../../Object/Component/Game/LightBulbPlacer.h"
#include "../../Graphics/RenderManager.h"
#include "../../Resource/ResourceManager.h"
#include "../../Graphics/Light/LightController.h"
#include "../../Object/ObjectManager.h"
#include "../../Object/Component/Common/Transform.h"
#include "../../Game/Event/EventManager.h"
#include "../../Sound/SoundManager.h"
#include "../../UI/UIManager.h"
#include "../../UI/Object/Unique/GuideUI.h"
#include "../../UI/Object/Unique/NumberUI.h"
#include "../../UI/Object/FadeUI.h"
#include "../../Game/ObjectSpawner.h"
#include "../../Game/GameDifficultyController.h"
#include "../../Game/GameProgressManager.h"
#include "PlayScene.h"

namespace
{
	// ライト設定
	// ToDo 外部ファイルに
	const string SPOT_LIGHT_KEY = "SPOTLIGHT_PLAYSCENE";
	const SPOT_LIGHT SPOT_LIGHT_CONFIG = { 
		.pos = { 50.0f,300.0f,-650.0f } ,
		.range = 2500.0f ,
		.direction = Vector3(0.0f, -1.0f, 0.0f).Normalized(),
		.angle = Deg2Radian(45.0f),
		.color = { 1.0f,0.0f,0.0f }
	};

	// UI設定
	// ToDo 外部ファイルに
	const string UI_KEY_GUIDELOCKER = "GUIDELOCKER";
	const Position3D UI_POSITION_GUIDELOCKER = { SCREEN_CENTER.x,800.0f,0.0f };

	const string UI_KEY_GUIDEGETKEY = "GUIDEGETKEY";
	const Position3D UI_POSITION_GUIDEGETKEY = { SCREEN_CENTER.x,800.0f,0.0f };

	const string UI_KEY_GUIDEUSEKEY = "GUIDEUSEKEY";
	const Position3D UI_POSITION_GUIDEUSEKEY = { SCREEN_CENTER.x,800.0f,0.0f };

	const string UI_KEY_GUIDEDONOTHAVEKEY = "GUIDEDONOTHAVEKEY";
	const Position3D UI_POSITION_GUIDEDONOTHAVEKEY = { SCREEN_CENTER.x,800.0f,0.0f };

	const string UI_KEY_GUIDEHIDE = "GUIDEHIDE";
	const Position3D UI_POSITION_GUIDEHIDE = { SCREEN_CENTER.x,800.0f,0.0f };

	const string UI_KEY_GUIDEGETLIGHT = "GUIDEGETLIGHT";
	const Position3D UI_POSITION_GUIDEGETLIGHT = { SCREEN_CENTER.x,800.0f,0.0f };

	const string UI_KEY_GUIDEPLACELIGHT = "GUIDEPLACELIGHT";
	const Position3D UI_POSITION_GUIDEPLACELIGHT = { SCREEN_CENTER.x,800.0f,0.0f };

	const string UI_KEY_MOVECONFIG = "MOVECONFIG";
	const Position3D UI_POSITION_MOVECONFIG = { 270.0f,850.0f,0.0f };

	const string UI_KEY_LIGHTBULBFRAME = "LIGHTBULBFRAME";
	const Position3D UI_POSITION_LIGHTBULBFRAME = { 270.0f,950.0f,0.0f };

	const string UI_KEY_LIGHTBULBNUM = "LIGHTBULBNUM";
	const Position3D UI_POSITION_LIGHTBULBNUM = { 360.0f,950.0f,0.0f };
	const float UINUMBER_LIGHTBULBNUM_OFFSET = (30.0f);

	const string UI_KEY_ITEMFRAME = "ITEMFRAME";
	const Position3D UI_POSITION_ITEMFRAME = { 1650.0f,950.0f,0.0f };

	const string UI_KEY_KEYA = "KEYA";
	const Position3D UI_POSITION_KEYA = { 1550.0f,950.0f,0.0f };

	const string UI_KEY_KEYB = "KEYB";
	const Position3D UI_POSITION_KEYB = { 1650.0f,950.0f,0.0f };

	const string UI_KEY_KEYC = "KEYC";
	const Position3D UI_POSITION_KEYC = { 1750.0f,950.0f,0.0f };

	// カメラ設定
	const Position3D CAMERA_FOLLOW_RELATIVE_POS = { 0.0f,10.0f,25.0f };
	const Position3D CAMERA_SUB_FOLLOW_RELATIVE_POS = { 0.0f,10.0f,35.0f };
	const float CAMERA_FOLLOW_RATE = (0.75f);


	// プレイヤーオブジェクトキー
	const string OBJECT_KEY_PLAYER = "PLAYER";
}

void PlayScene::InitScene()
{
	// マウス設定
	InputMng.SetIsFixedMouse(true);
	ShowCursor(false);

	// リスナー設定
	SoundMng.SetListener(MainCamera.GetTransform());

	GameProgress.ContinueGame();
}

void PlayScene::UpdateScene()
{
	auto& objMng = ObjectMng;

#ifdef _DEBUG
	auto preTimeAnim = chrono::system_clock::now();
#endif

	objMng.PreUpdateObjects();
	objMng.UpdateObjects();
	objMng.LateUpdateObjects();

#ifdef _DEBUG
	auto animtime = chrono::system_clock::now();

	auto animt = static_cast<float>(
		chrono::duration_cast<chrono::nanoseconds>(animtime - preTimeAnim).count() / 1000000000.0);

	ImGui::Text("AnimTime = %f", animt);
#endif

	CollisionMng.Collision(COLLISION_FASE::FIRST, 3);

	objMng.OnCollisionUpdateObjects(COLLISION_FASE::FIRST);

#ifdef _DEBUG
	auto preTimeCol = chrono::system_clock::now();
#endif

	CollisionMng.Collision(COLLISION_FASE::SECOND, 1);

#ifdef _DEBUG
	auto coltime = chrono::system_clock::now();

	auto colt = static_cast<float>(
		chrono::duration_cast<chrono::nanoseconds>(coltime - preTimeCol).count() / 1000000000.0);

	ImGui::Text("CollisionTime = %f", colt);
#endif

	objMng.OnCollisionUpdateObjects(COLLISION_FASE::SECOND);


#ifdef _DEBUG

	const auto& lights = objMng.GetSpecificObjectArray(OBJECT_ID::LIGHTBULB);
	for (int index = 0;const auto& light : lights)
	{

		const auto& pos = light.lock()->GetTransform().pos;
		ImGui::Text("lightPos : %0.2f,%0.2f,%0.2f", pos.x, pos.y,pos.z);

		index++;
	}

#endif
}

void PlayScene::OnCameraUpdateScene()
{
	ObjectMng.OnCameraUpdateObjects();
}

void PlayScene::DrawScene()
{
	ObjectMng.DrawObjects();
}

void PlayScene::Terminate()
{
	Light.DeleteSpotLightInfo(SPOT_LIGHT_KEY);

	ObjectMng.ReleaseObjects();

	InputMng.SetIsFixedMouse(false);
	ShowCursor(true);

#ifdef _DEBUG
	RenderMng.DeleteRenderer(depth_);
#endif
}

void PlayScene::ObjectSetting()
{
#ifdef _DEBUG
	depth_ =
		RenderMng.CreateRenderer<Object2DRenderer>(LAYER::UI);
	depth_->CreateMesh(Size2D(192.0f * 3.0f, 108.0f * 3.0f));
	Transform d = {};
	d.pos = { 192.0f * 1.5f + 10.0f,108.0f * 1.5f + 10.0f,0.0f };
	depth_->SetMeshTransformMatrixFromTransform(d);
	depth_->GetMesh().GetMaterial().SetPSOID(PSO_ID::SHADOW_2D);
#endif

	GameObjectSpawner.ObjectSpawn(GameDifficulty.GetLevel());
}

void PlayScene::UISetting()
{
	shared_ptr<UIObject>ui = make_shared<GuideUI>(EVENT_ID::LOOK_LOCKER, EVENT_ID::LOOKAWAY_LOCKER);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_GUIDEHIDE));
	ui->SetPosition(UI_POSITION_GUIDELOCKER);
	UIMng.AddUI(UI_KEY_GUIDELOCKER, ui);

	ui = make_shared<GuideUI>(EVENT_ID::LOOK_KEY, EVENT_ID::LOOKAWAY_KEY);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_GUIDEGETKEY));
	ui->SetPosition(UI_POSITION_GUIDEGETKEY);
	UIMng.AddUI(UI_KEY_GUIDEGETKEY, ui);

	ui = make_shared<GuideUI>(EVENT_ID::LOOK_PADLOCKWITHKEY, EVENT_ID::LOOKAWAY_PADLOCKWITHKEY);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_GUIDEUSEKEY));
	ui->SetPosition(UI_POSITION_GUIDEUSEKEY);
	UIMng.AddUI(UI_KEY_GUIDEUSEKEY, ui);

	ui = make_shared<GuideUI>(EVENT_ID::LOOK_PADLOCK, EVENT_ID::LOOKAWAY_PADLOCK);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_GUIDEDONOTHAVEKEY));
	ui->SetPosition(UI_POSITION_GUIDEDONOTHAVEKEY);
	UIMng.AddUI(UI_KEY_GUIDEDONOTHAVEKEY, ui);

	ui = make_shared<GuideUI>(EVENT_ID::HIDE_LOCKER, EVENT_ID::GETOUT_LOCKER);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_GUIDEGETOUT));
	ui->SetPosition(UI_POSITION_GUIDEHIDE);
	UIMng.AddUI(UI_KEY_GUIDEHIDE, ui);

	ui = make_shared<GuideUI>(EVENT_ID::LOOK_LIGHTBULB, EVENT_ID::LOOKAWAY_LIGHTBULB);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_GUIDEGETLIGHT));
	ui->SetPosition(UI_POSITION_GUIDEGETLIGHT);
	UIMng.AddUI(UI_KEY_GUIDEGETLIGHT, ui);

	ui = make_shared<GuideUI>(EVENT_ID::POSSIBLE_PLACELIGHTBULB, EVENT_ID::NOTPOSSIBLE_PLACELIGHTBULB);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_GUIDEPLACELIGHT));
	ui->SetPosition(UI_POSITION_GUIDEPLACELIGHT);
	UIMng.AddUI(UI_KEY_GUIDEPLACELIGHT, ui);

	ui = make_shared<FadeUI>();
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_MOVECONFIG));
	ui->SetPosition(UI_POSITION_MOVECONFIG);
	UIMng.AddUI(UI_KEY_MOVECONFIG, ui);

	ui = make_shared<FadeUI>();
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_LIGHTBULBFRAME));
	ui->SetPosition(UI_POSITION_LIGHTBULBFRAME);
	UIMng.AddUI(UI_KEY_LIGHTBULBFRAME, ui);

	auto& objMng = ObjectMng;
	auto& stateMachineP = objMng.GetSpecificObject(OBJECT_KEY_PLAYER)->GetComponent<StateMachine<Player::STATE>>();
	const auto& num = stateMachineP.GetState(Player::STATE::DEFAULT).GetComponent<LightBulbPlacer>().GetLightBulbNum();
	ui = make_shared<NumberUI>(num);
	ui->SetPosition(UI_POSITION_LIGHTBULBNUM);
	static_pointer_cast<NumberUI>(ui)->SetOffsetX(UINUMBER_LIGHTBULBNUM_OFFSET);
	UIMng.AddUI(UI_KEY_LIGHTBULBNUM, ui);

	ui = make_shared<FadeUI>();
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_ITEMFRAME));
	ui->SetPosition(UI_POSITION_ITEMFRAME);
	UIMng.AddUI(UI_KEY_ITEMFRAME, ui);

	ui = make_shared<GuideUI>(EVENT_ID::GET_KEY_A, EVENT_ID::USE_KEY_A);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_KEYICON));
	ui->SetPosition(UI_POSITION_KEYA);
	UIMng.AddUI(UI_KEY_KEYA, ui);

	ui = make_shared<GuideUI>(EVENT_ID::GET_KEY_B, EVENT_ID::USE_KEY_B);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_KEYICON));
	ui->SetPosition(UI_POSITION_KEYB);
	UIMng.AddUI(UI_KEY_KEYB, ui);

	ui = make_shared<GuideUI>(EVENT_ID::GET_KEY_C, EVENT_ID::USE_KEY_C);
	ui->SetTexture<Object2DRenderer>(ResourceMng.GetTexture(RES_ID::IMAGE_KEYICON));
	ui->SetPosition(UI_POSITION_KEYC);
	UIMng.AddUI(UI_KEY_KEYC, ui);
}

void PlayScene::CameraSetting()
{
	auto player = static_pointer_cast<Player>(ObjectMng.GetSpecificObject(OBJECT_KEY_PLAYER));
	if (player == nullptr)
	{
		MainCamera.ChangeState(Camera::STATE::FREE);
		return;
	}

	MainCamera.ChangeState(Camera::STATE::RATE_FOLLOW);
	MainCamera.SetFollower(static_pointer_cast<Player>(player)->GetHeadTransform());
	MainCamera.SetFollowRelativePos(CAMERA_FOLLOW_RELATIVE_POS);
	MainCamera.SetSubFollower(static_pointer_cast<Player>(player)->GetHeadSubTransform());
	MainCamera.SetSubFollowRelativePos(CAMERA_SUB_FOLLOW_RELATIVE_POS);
	MainCamera.SetFollowRate(CAMERA_FOLLOW_RATE);
}

void PlayScene::LightSetting()
{
	Light.AddSpotLight(SPOT_LIGHT_CONFIG, SPOT_LIGHT_KEY);
}

void PlayScene::EventSetting()
{
	EventMng.RegisterEventAction(EVENT_ID::OPEN_DOOR, [&] {SceneMng.ChangeScene(SCENE_ID::RESULT); });
	EventMng.RegisterEventAction(EVENT_ID::DEAD_PLAYER, [&] {SceneMng.ChangeScene(SCENE_ID::RESULT); });
}
