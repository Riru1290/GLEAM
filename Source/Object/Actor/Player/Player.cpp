#ifdef _DEBUG
#include "../../../../External/imgui/imgui.h"
#include "../../../../External/imgui/imgui_impl_win32.h"
#include "../../../../External/imgui/imgui_impl_dx12.h"
#include "../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../../Utility/Math/Quaternion.h"
#include "../../../Resource/ResourceManager.h"
#include "../../../Input/InputManager.h"
#include "../../../Camera/Camera.h"
#include "../../Component/Common/Transform.h"
#include "../../Component/Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "../../Component/Input/InputController.h"
#include "../../Component/Collider/Colliders/CylinderCollider.h"
#include "../../Component/Collider/Colliders/SphereCollider.h"
#include "../../Component/Collider/Colliders/LineCollider.h"
#include "../../Component/Object/GravityController.h"
#include "../../Component/Actor/AnimationController.h"
#include "../../Component/Game/LightBulbPlacer.h"
#include "../../Component/Game/KeyInventory.h"
#include "../../Component/Game/PadLockUnlocker.h"
#include "../../Component/Game/FlashLightHolder.h"
#include "../../Component/Controller/MoveController.h"
#include "../../Component/Controller/RotateController.h"
#include "../../Component/Controller/RotateBoneController.h"
#include "../../../Resource/Model/Model.h"
#include "../../../Resource/Animation/Animation.h"
#include "../../../Collision/CollisionManager.h"
#include "../../../Game/Event/EventManager.h"
#include "../../../Game/GameProgressManager.h"
#include "../../../Sound/SoundManager.h"
#include "../../../Graphics/PostEffect/PostEffectApplier.h"
#include "Player.h"

namespace
{
	// �ړ��ݒ�
	const float WALK_SPEED = (250.0f);
	const float RUN_SPEED = (550.0f);

	// �����ݒ�
	const float MAX_HEAD_ANGLE = (60.0f);
	const float MIN_HEAD_ANGLE = (-70.0f);
	const Vector3 ROTATE_POW_X = { 0.0f, Deg2Radian(5.0f), 0.0f };
	const Vector3 ROTATE_POW_Y = { Deg2Radian(5.0f),0.0f , 0.0f };

	// ���̃{�[���ԍ�
	const int BONE_NO_HEAD = (15);

	// ���͐ݒ�
	const string INPUT_KEY_ACTION = "PLAYER_ACTION";
	const string INPUT_KEY_ACTION_LIGHT = "PLAYER_ACTION_LIGHT";
	const float ACTION_WAIT_TIME = (0.2f);
	const string ACTION_WAIT_TIME_KEY = "PLAYER_ACTION_WAIT";
	
	// ���S���ݒ�
	const float DEAD_WAIT_TIME = (1.5f);
	const string DEAD_WAIT_TIME_KEY = "PLAYER_DEAD_WAIT";

	// �d��
	const int LIGHTBULB_NUM_MAX = (20);
	const Quaternion LIGHTBULB_ROTATE = Quaternion::Euler({ Deg2Radian(-90.0f),0.0f,0.0f });
	const Quaternion LIGHTBULB_ROTATE_LOCAL = Quaternion::Euler({ Deg2Radian(90.0f), 0.0f,0.0f });
	const Scale3D LIGHTBULB_SCALE = { 5.0f,5.0f,5.0f };

	const Quaternion LIGHTBULB_ROTATE_PLACE = Quaternion::Euler({ Deg2Radian(90.0f),0.0f,0.0f });
	const Position3D LIGHTBULB_RELATIVEPOS_PLACE = { 0.0f,0.0f,10.0f };


	// �t���b�V�����C�g
	const Position3D FLASHLIGHT_RELATIVEPOS = { 0.0f,-10.0f,10.0f };
	const Position3D FLASHLIGHT_RELATIVEPOS_HIDE = { 0.0f, 0.0f, 10.0f };
	const Position3D FLASHLIGHT_RELATIVEPOS_DEAD = { 0.0f, 0.0f, 10.0f };

	const Vector3 FLASHLIGHT_COLOR_DEAD = { 1.0f,0.0f,0.0f };
	const float FLASHLIGHT_LERP_TIME = (3.0f);

	// ���b�J�[
	const Position3D LOCLER_RELATIVEPOS_IN = { 0.0f,15.0f,-30.0f };
	const Position3D LOCLER_RELATIVEPOS_OUT = { 0.0f,100.0f,75.0f };

	// �R���C�_�[�ݒ�
	const float CYLINDER_COL_RADIUS = (55.0f);
	const float CYLINDER_COL_HEIGHT = (170.0f);
	const Position3D CYLINDER_COL_RELATIVEPOS = { 0.0f,85.0f,0.0f };

	const float SPHERE_COL_RADIUS = (30.0f);
	const Position3D SPHERE_COL_RELATIVEPOS = { 0.0f,85.0f,0.0f };

	const float LINE_COL_LENGTH = (550.0f);
	const float LINE_COL_LENGTH_LONG = (50000.0f);

	// �A�j���[�V����
	const string ANIMATION_KEY_IDLE = "IDLE";
	const string ANIMATION_KEY_WALK = "WALK";
	const string ANIMATION_KEY_RUN = "RUN";
}

Player::Player(const string& uniqueKey) : Actor(uniqueKey),
	neaEyeHitPos_({}),
	lockerPos_({}),
	preLockerPos_({}),
	lockerQua_({}),
	availableAction_(AVAILABLE_ACTION::NONE),
	preAvailableAction_(AVAILABLE_ACTION::NONE),
	walkSoundHandle_(-1),
	runSoundHandle_(-1),
	hideSoundHandle_(-1)
{
	// ���f���ǂݍ���
	modelRenderer_->get().SetModel(ResourceMng.GetModel(RES_ID::MODEL_PLAYER));

	modelRenderer_->get().SetTransform(transform_->get());

	head_ = make_unique<Transform>();

	camera_ = make_unique<Transform>();
	cameraSub_ = make_unique<Transform>();

	eyeA_ = make_unique<Transform>();
	eyeB_ = make_unique<Transform>();

	eyeLongA_ = make_unique<Transform>();
	eyeLongB_ = make_unique<Transform>();

	// ���͏��ݒ�
	InputSetting();

	// �R���C�_�[�ݒ�
	ColliderSetting();

	// �d�͐ݒ�
	GravitySetting();

	// �X�e�[�g�ݒ�
	StateSetting();

	auto flashLight = make_shared<FlashLightHolder>();
	flashLight->SetFollowerTransform(MainCamera.GetTransform());
	flashLight->SetRelativePos(FLASHLIGHT_RELATIVEPOS, false);
	flashLight_ = *flashLight;
	AddComponent(flashLight);

	walkSoundHandle_ = SoundMng.Play(RES_ID::SOUND_PLAYER_WALK, PLAY_TYPE::LOOP);
	SoundMng.StopSound(walkSoundHandle_);

	runSoundHandle_ = SoundMng.Play(RES_ID::SOUND_PLAYER_RUN, PLAY_TYPE::LOOP);
	SoundMng.StopSound(runSoundHandle_);

	hideSoundHandle_ = SoundMng.Play(RES_ID::SOUND_PLAYER_HIDE, PLAY_TYPE::LOOP);
	SoundMng.StopSound(hideSoundHandle_);
}

Player::~Player()
{
	SoundMng.StopSound(walkSoundHandle_);
	SoundMng.StopSound(runSoundHandle_);
	SoundMng.StopSound(hideSoundHandle_);
}

void Player::InitActor()
{
}

void Player::DrawActor()
{

}

void Player::UpdateActor()
{
	EventMng.Notify(EVENT_ID::LOOKAWAY_LOCKER);
	EventMng.Notify(EVENT_ID::LOOKAWAY_KEY);
	EventMng.Notify(EVENT_ID::LOOKAWAY_PADLOCK);
	EventMng.Notify(EVENT_ID::LOOKAWAY_PADLOCKWITHKEY);
	EventMng.Notify(EVENT_ID::LOOKAWAY_LIGHTBULB);
	EventMng.Notify(EVENT_ID::NOTPOSSIBLE_PLACELIGHTBULB);

	animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE);

	neaEyeHitPos_ = { D3D12_FLOAT32_MAX,D3D12_FLOAT32_MAX ,D3D12_FLOAT32_MAX };
}

void Player::LateUpdateActor()
{
	preAvailableAction_ = availableAction_;
	availableAction_ = AVAILABLE_ACTION::NONE;
}

void Player::OnCameraUpdateActor()
{
	if (GameProgress.IsDetectedByEnemy())
	{
		SoundMng.StartSound(hideSoundHandle_);
	}
	else
	{
		SoundMng.StopSound(hideSoundHandle_);
	}
}

void Player::OnCollisionUpdateSetting()
{
	onCollisionUpdateList_[COLLISION_FASE::FIRST] =
		[this] {

		LateUpdate();
		UpdateCameraTarget();

		};
}

void Player::AnimationSetting()
{
	// �A�j���[�V������K�p�����郂�f���Z�b�g
	animationController_->get().SetAnimationModel(modelRenderer_->get());

	// �A�j���[�V�����ǂݍ��݁E�Z�b�g
	auto animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_PLAYER_IDLE).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_IDLE);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_PLAYER_WALK).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_WALK);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_PLAYER_RUN).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_RUN);
	animationController_->get().SetAnimPlaySpeed(0.75f, ANIMATION_KEY_RUN);

	animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE);
}

void Player::StateSetting()
{
	// �X�e�[�g�ݒ�
	AddComponent(make_shared<StateMachine<STATE>>());

	stateMachine_ = GetComponent<StateMachine<STATE>>();

	DefaultStateSetting();
	HideStateSetting();
	DeadStateSetting();

	// �ʏ��Ԃɐݒ�
	stateMachine_->get().ChangeState(STATE::DEFAULT);
}

void Player::InputSetting()
{
	InputMng.AddKey(INPUT_KEY_ACTION, { {INPUT_TYPE::Keyboard,VK_R} });
	InputMng.AddKey(INPUT_KEY_ACTION_LIGHT, { {INPUT_TYPE::Keyboard,VK_RBUTTON} });

	AddComponent(make_shared<InputController>());

	input_ = GetComponent<InputController>();
}

void Player::ColliderSetting()
{
	// �V�����_�[�R���C�_�[�ݒ�
	// ��Ƀ}�b�v��G�l�~�[�Ƃ̏Փ˔���Ɏg��
	CYLINDER_COL_INFO cylinder;
	cylinder.radius = CYLINDER_COL_RADIUS;
	cylinder.height = CYLINDER_COL_HEIGHT;

	auto collider = make_shared<CylinderCollider>();
	collider->SetCylinderInfo(cylinder);
	collider->SetRelativePosition(CYLINDER_COL_RELATIVEPOS);
	collider->SetFollower(transform_->get());
	collider->SetRigidState(true);
	collider->AddOnHitFunction(OBJECT_ID::MAP, bind(&Player::OnHitMap, this, placeholders::_1));
	collider->AddOnHitFunction(OBJECT_ID::ENEMY, bind(&Player::OnHitEnemy, this, placeholders::_1));

	bodyCollider_ = *collider;

	AddComponent(collider);

	// �R���W�����}�l�[�W���[�ɓo�^
	CollisionMng.RegisterCollider(collider, GetObjectUniqueKey(), OBJECT_ID::PLAYER);

	// �ڐ��R���C�_�[�ݒ�
	// ��Ƀ}�b�v�⌮�Ƃ̏Փ˔���Ɏg��
	auto eye = make_shared<LineCollider>();
	eye->SetFollower(*eyeA_);
	eye->SetSubFollower(*eyeB_);

	eye->AddOnHitFunction(OBJECT_ID::MAP, bind(&Player::OnHitEyeMap, this, placeholders::_1));
	eye->AddOnHitFunction(OBJECT_ID::LOCKER, bind(&Player::OnHitEyeLocker, this, placeholders::_1));
	eye->AddOnHitFunction(OBJECT_ID::KEY, bind(&Player::OnHitEyeKey, this, placeholders::_1));
	eye->AddOnHitFunction(OBJECT_ID::PADLOCK, bind(&Player::OnHitEyePadLock, this, placeholders::_1));
	eye->AddOnHitFunction(OBJECT_ID::LIGHTBULB, bind(&Player::OnHitEyeLightBulb, this, placeholders::_1));

	AddComponent(eye);

	// �R���W�����}�l�[�W���[�ɓo�^
	CollisionMng.RegisterCollider(eye, GetObjectUniqueKey(), OBJECT_ID::PLAYER_EYE);

	eyeCollider_ = *eye;


	// �ڐ��R���C�_�[�ݒ�
	// ��Ƀ}�b�v�⌮�Ƃ̏Փ˔���Ɏg��
	eye = make_shared<LineCollider>();
	eye->SetFollower(*eyeLongA_);
	eye->SetSubFollower(*eyeLongB_);

	AddComponent(eye);

	// �R���W�����}�l�[�W���[�ɓo�^
	CollisionMng.RegisterCollider(eye, GetObjectUniqueKey(), OBJECT_ID::PLAYER_EYE_LONG);

	eyeLongCollider_ = *eye;

	// �G�l�~�[�T���p�R���C�_�[�ݒ�
	SPHERE_COL_INFO sphere;
	sphere.radius = SPHERE_COL_RADIUS;
	auto point = make_shared<SphereCollider>();
	point->SetSphereInfo(sphere);
	point->SetFollower(transform_->get());
	point->SetRelativePosition(SPHERE_COL_RELATIVEPOS);

	AddComponent(point);

	// �R���W�����}�l�[�W���[�ɓo�^
	CollisionMng.RegisterCollider(point, GetObjectUniqueKey(), OBJECT_ID::PLAYER_POINT);

	pointCollider_ = *point;
}

void Player::GravitySetting()
{
	auto gravity = make_shared<GravityController>();

	gravity->SetTransform(transform_->get());

	AddComponent(gravity);

	gravity_ = GetComponent<GravityController>();
}

void Player::UpdateCameraTarget()
{
	// �}�b�v�Ƃ̏Փ˔����̃v���C���[�ʒu�����g�p����
	// �ڐ������X�V����

	auto mat = modelRenderer_->get().GetGlobalAnimBoneMatrix(15);

	XMFLOAT3 pos = GetTranslationFromMatrix(mat);

	Quaternion rot = Quaternion::ExtractQuaternion(mat);

	camera_->pos = { pos.x,pos.y,pos.z };
	camera_->qua = rot;

	mat = modelRenderer_->get().GetGlobalBoneMatrix(15);

	pos = GetTranslationFromMatrix(mat);

	rot = Quaternion::ExtractQuaternion(mat);

	cameraSub_->pos = { pos.x,pos.y,pos.z };
	cameraSub_->qua = rot;

	eyeA_->pos = MainCamera.GetLatestPos();
	eyeB_->pos = MainCamera.GetLatestTargetPos();

	eyeB_->pos = eyeA_->pos + ((eyeB_->pos - eyeA_->pos).Normalized() * LINE_COL_LENGTH);

	// �ڐ�������ɃR���C�_�[���X�V
	eyeCollider_->get().Update();




	eyeLongA_->pos = MainCamera.GetLatestPos();
	eyeLongB_->pos = MainCamera.GetLatestTargetPos();

	eyeLongB_->pos = eyeLongA_->pos + ((eyeLongB_->pos - eyeLongA_->pos).Normalized() * LINE_COL_LENGTH_LONG);

	// �ڐ�������ɃR���C�_�[���X�V
	eyeLongCollider_->get().Update();

}

void Player::DefaultStateSetting()
{
	// �X�e�[�g�J�ڏ����ݒ�
	unordered_map<STATE, function<bool()>> list = {
	{STATE::HIDE , [this]
		{
			bool isChange =
				(availableAction_ == AVAILABLE_ACTION::HIDE &&
					InputMng.Triggered(INPUT_KEY_ACTION) &&
					!MainTimer.InProgress(ACTION_WAIT_TIME_KEY));

			if (isChange)
			{
				transform_->get().pos = lockerPos_;
				transform_->get().qua = lockerQua_;
				rotateBoneCon_->get().ResetRotate();
				pointCollider_->get().SetActive(false);
				bodyCollider_->get().SetActive(false);
				gravity_->get().SetActive(false);
				flashLight_->get().SetRelativePos(FLASHLIGHT_RELATIVEPOS_HIDE, true);
				MainTimer.SetTimer(ACTION_WAIT_TIME_KEY, ACTION_WAIT_TIME, true);
				EventMng.Notify(EVENT_ID::HIDE_LOCKER);

				SoundMng.StopSound(runSoundHandle_);
				SoundMng.StopSound(walkSoundHandle_);

				SoundMng.Play(RES_ID::SOUND_LOCKER_IN);

			}

			return isChange;
		}}
	};

	// �ʏ���
	unique_ptr<State<STATE>> defaultState =
		make_unique<State<STATE>>(list);

	auto& animCon = GetComponent<AnimationController>();

	// �ړ��R���|�[�l���g�ǉ�
	shared_ptr<MoveController> moveCon = make_shared<MoveController>();
	moveCon->SetTransform(GetComponent<Transform>());
	moveCon->SetWalkAction([&animCon,this] {
		animCon.ChangeAnimation(ANIMATION_KEY_WALK);
		SoundMng.StartSound(walkSoundHandle_);
		SoundMng.StopSound(runSoundHandle_);
		});
	moveCon->SetRunAction([&animCon, this] {
		animCon.ChangeAnimation(ANIMATION_KEY_RUN);
		SoundMng.StartSound(runSoundHandle_);
		SoundMng.StopSound(walkSoundHandle_);
		});
	moveCon->SetIdleAction([&animCon, this] {
		SoundMng.StopSound(walkSoundHandle_);
		SoundMng.StopSound(runSoundHandle_); });
	moveCon->SetWalkSpeed(WALK_SPEED);
	moveCon->SetRunSpeed(RUN_SPEED);
	defaultState->AddComponent(moveCon);

	// ��]�R���|�[�l���g�ǉ�
	shared_ptr<RotateController> rotateCon = make_shared<RotateController>();
	rotateCon->SetTransform(GetComponent<Transform>());
	rotateCon->SetRotatePowMouseX(ROTATE_POW_X);
	defaultState->AddComponent(rotateCon);

	// ���𓮂����R���|�[�l���g�ǉ�
	shared_ptr<RotateBoneController> rotateBoneCon = make_shared<RotateBoneController>();
	rotateBoneCon->SetBoneNo(BONE_NO_HEAD);
	rotateBoneCon->SetAnimationModelRenderer(GetComponent<AnimationModelRenderer>());
	rotateBoneCon->SetRotatePowMouseY(ROTATE_POW_Y);
	rotateBoneCon->SetRotateLimit(
		{ Deg2Radian(MIN_HEAD_ANGLE),0.0f,0.0f },
		{ Deg2Radian(MAX_HEAD_ANGLE),0.0f,0.0f },
		true, false, false);
	rotateBoneCon_ = *rotateBoneCon;
	defaultState->AddComponent(rotateBoneCon);


	// ���C�g�ݒu�R���|�[�l���g�ǉ�
	auto lightBulbPlacer = make_shared<LightBulbPlacer>();
	lightBulbPlacer->SetLightBulbNumMax(LIGHTBULB_NUM_MAX);
	lightBulbPlacer->SetIsPossibleCheckFunction(
		[&] {return InputMng.Pressed(INPUT_KEY_ACTION_LIGHT); }
	);
	defaultState->AddComponent(lightBulbPlacer);

	// ���Ǘ��R���|�[�l���g�ǉ�
	auto keyInventory = make_shared<KeyInventory>();
	defaultState->AddComponent(keyInventory);
	keyInventory_ = defaultState->GetComponent<KeyInventory>();

	// �싞�������R���|�[�l���g�ǉ�
	auto padLockUnlocker = make_shared<PadLockUnlocker>();
	defaultState->AddComponent(padLockUnlocker);
	padLockUnlocker_ = defaultState->GetComponent<PadLockUnlocker>();

	// �Փ˔���㏈����ݒ�
	lightBulbPlacer_ = defaultState->GetComponent<LightBulbPlacer>();
	lightBulbPlacer_->get().SetTempLightBulbModelTransState(false);

	defaultState->onCollisionUpdateList_[COLLISION_FASE::FIRST] =
		[this] {
		auto cPos = MainCamera.GetLatestPos();
		auto tPos = MainCamera.GetLatestTargetPos();
		Transform t = {};
		t.qua = LIGHTBULB_ROTATE;
		t.pos = cPos + (tPos - cPos).Normalized() * LINE_COL_LENGTH;
		t.localQua = LIGHTBULB_ROTATE_LOCAL;
		t.scale = LIGHTBULB_SCALE;
		lightBulbPlacer_->get().SetPlaceLightTransform(t);
		};

	defaultState->onCollisionUpdateList_[COLLISION_FASE::SECOND] =
		[this] {
		bool isAction = InputMng.Triggered(INPUT_KEY_ACTION) &&
			!MainTimer.InProgress(ACTION_WAIT_TIME_KEY);
		if (InputMng.Pressed(INPUT_KEY_ACTION_LIGHT))
		{
			lightBulbPlacer_->get().SetTempLightBulbModelTransState(true);
			PostEffect.ReservePostEffect(POSTEFFECT_ID::MONOCHROME);
			if (availableAction_ == AVAILABLE_ACTION::PLACE_LIGHT)
			{
				if (isAction)
				{
					lightBulbPlacer_->get().PlaceLightBulb();
				}

				bool isAction = InputMng.Pressed(INPUT_KEY_ACTION_LIGHT);
				if (isAction)
				{
					EventMng.Notify(EVENT_ID::POSSIBLE_PLACELIGHTBULB);
				}
			}
		}
		else
		{
			lightBulbPlacer_->get().SetTempLightBulbModelTransState(false);
			if (availableAction_ == AVAILABLE_ACTION::GET_LIGHT)
			{
				EventMng.Notify(EVENT_ID::LOOK_LIGHTBULB);
				if (isAction)
				{
					lightBulbPlacer_->get().GetLightBulb();
				}
			}
			else if (availableAction_ == AVAILABLE_ACTION::GET_KEY)
			{
				EventMng.Notify(EVENT_ID::LOOK_KEY);
				if (isAction)
				{
					keyInventory_->get().GetKey();
				}
			}
			else if (availableAction_ == AVAILABLE_ACTION::UNLOCK_PADLOCK)
			{
				if (keyInventory_->get().GetKeyCount() > 0)
				{
					EventMng.Notify(EVENT_ID::LOOK_PADLOCKWITHKEY);
				}
				else
				{
					EventMng.Notify(EVENT_ID::LOOK_PADLOCK);
				}
				if (isAction)
				{
					if (keyInventory_->get().UseKey())
					{
						padLockUnlocker_->get().UnLock();
						isAction = true;
					}
				}
			}
			else if (availableAction_ == AVAILABLE_ACTION::HIDE)
			{
				EventMng.Notify(EVENT_ID::LOOK_LOCKER);
			}
		}


		if (isAction)
		{
			MainTimer.SetTimer(ACTION_WAIT_TIME_KEY, ACTION_WAIT_TIME, true);
		}
	};
	// �ʏ�X�e�[�g�ǉ�
	stateMachine_->get().RegisterState(STATE::DEFAULT, move(defaultState));
}


void Player::HideStateSetting()
{
	// �X�e�[�g�J�ڏ����ݒ�
	unordered_map<STATE, function<bool()>> list = {
		{STATE::DEFAULT , [this] {
			bool isChange = (
				InputMng.Triggered(INPUT_KEY_ACTION) &&
				!MainTimer.InProgress(ACTION_WAIT_TIME_KEY));

			if (isChange)
			{
				transform_->get().SetActive(true);
				transform_->get().pos = preLockerPos_;
				pointCollider_->get().SetActive(true);
				bodyCollider_->get().SetActive(true);
				gravity_->get().SetActive(true); 
				flashLight_->get().SetRelativePos(FLASHLIGHT_RELATIVEPOS, true);
				MainTimer.SetTimer(ACTION_WAIT_TIME_KEY, ACTION_WAIT_TIME, true);
				EventMng.Notify(EVENT_ID::GETOUT_LOCKER);
				SoundMng.Play(RES_ID::SOUND_LOCKER_OUT);

			}

			return isChange;

		}}
	};

	// �n�C�h���
	unique_ptr<State<STATE>> hideState =
		make_unique<State<STATE>>(list);

	hideState->onCollisionUpdateList_[COLLISION_FASE::SECOND] =
		[&] {
		PostEffect.ReservePostEffect(POSTEFFECT_ID::LOCKER);
		};

	// �n�C�h�X�e�[�g�ǉ�
	stateMachine_->get().RegisterState(STATE::HIDE, move(hideState));
}

void Player::DeadStateSetting()
{
	// �X�e�[�g�J�ڏ����ݒ�
	unordered_map<STATE, function<bool()>> list = {

	};

	// ���S���
	unique_ptr<State<STATE>> deadState =
		make_unique<State<STATE>>(list);

	deadState->onCollisionUpdateList_[COLLISION_FASE::FIRST] =
		[this] {
		if (!MainTimer.InProgress(DEAD_WAIT_TIME_KEY))
		{
			EventMng.Notify(EVENT_ID::DEAD_PLAYER);
		}
		};

	// ���S�X�e�[�g�ǉ�
	stateMachine_->get().RegisterState(STATE::DEAD, move(deadState));
}

void Player::OnHitMap(COL_HIT_DATA data)
{
	data.selfCollider.lock()->Update();
}

void Player::OnHitEnemy(COL_HIT_DATA data)
{
	GetComponent<CylinderCollider>().SetActive(false);
	eyeCollider_->get().SetActive(false);
	eyeLongCollider_->get().SetActive(false);

	MainTimer.SetTimer(DEAD_WAIT_TIME_KEY, DEAD_WAIT_TIME, true);

	// ���S��Ԃɐݒ�
	stateMachine_->get().ChangeState(STATE::DEAD);

	int handle = SoundMng.Play(RES_ID::SOUND_PLAYER_DEAD);
	SoundMng.SetSoundVolume(handle, 3.0f);
	handle = SoundMng.Play(RES_ID::SOUND_PLAYER_DEAD2);
	SoundMng.SetSoundVolume(handle, 3.0f);
	handle = SoundMng.Play(RES_ID::SOUND_PLAYER_DEAD3);
	SoundMng.SetSoundVolume(handle, 3.0f);
	SoundMng.StopSound(runSoundHandle_);
	SoundMng.StopSound(walkSoundHandle_);

	flashLight_->get().SetGoalSpotLightColor(FLASHLIGHT_COLOR_DEAD, FLASHLIGHT_LERP_TIME);

	flashLight_->get().SetRelativePos(FLASHLIGHT_RELATIVEPOS_DEAD, true);

	GameProgress.ResetDetectedByEnemyNum();
}

void Player::OnHitEyeMap(COL_HIT_DATA data)
{
	if (holds_alternative<COL_HIT_DATA_POLYGON>(data.hitData))
	{
		auto& hitData = get<COL_HIT_DATA_POLYGON>(data.hitData);
		if (hitData.neaHitPos.has_value())
		{
			Transform t = {};
			t.qua = Quaternion::LookRotation(hitData.neaPolygon.value().normal.ClampEpsilon());
			t.pos = hitData.neaHitPos.value() + t.qua.PosAxis(LIGHTBULB_RELATIVEPOS_PLACE);
			t.localQua = LIGHTBULB_ROTATE_PLACE;
			t.scale = LIGHTBULB_SCALE;
			lightBulbPlacer_->get().SetPlaceLightTransform(t);

			neaEyeHitPos_ = hitData.neaHitPos.value();

			availableAction_ = AVAILABLE_ACTION::PLACE_LIGHT;
		}
	}

}

void Player::OnHitEyeKey(COL_HIT_DATA data)
{
	float neaDis = (neaEyeHitPos_ - eyeA_->pos).Length();

	Position3D hitNeaPos = {};
	if (holds_alternative<COL_HIT_DATA_LINE>(data.hitData)) {
		if (get<COL_HIT_DATA_LINE>(data.hitData).neaHitPos.has_value())
		{
			hitNeaPos = get<COL_HIT_DATA_LINE>(data.hitData).neaHitPos.value();
		}
	}
	float dis = (hitNeaPos - eyeA_->pos).Length();

	if (dis > neaDis) return;

	neaEyeHitPos_ = hitNeaPos;

	availableAction_ = AVAILABLE_ACTION::GET_KEY;

	const auto& key = data.targetCollider.lock()->GetObjectUniqueKey();
	keyInventory_->get().SetGetKeyObjectUniqueKey(key);

}

void Player::OnHitEyeLocker(COL_HIT_DATA data)
{
	float neaDis = (neaEyeHitPos_ - eyeA_->pos).Length();

	Position3D hitNeaPos = {};
	if (holds_alternative<COL_HIT_DATA_POLYGON>(data.hitData)) {
		if (get<COL_HIT_DATA_POLYGON>(data.hitData).neaHitPos.has_value())
		{
			hitNeaPos = get<COL_HIT_DATA_POLYGON>(data.hitData).neaHitPos.value();
		}
	}
	float dis = (hitNeaPos - eyeA_->pos).Length();

	if (dis > neaDis) return;


	neaEyeHitPos_ = hitNeaPos;

	availableAction_ = AVAILABLE_ACTION::HIDE;

	const auto& locker = data.targetCollider.lock()->GetFollowerTransform();
	lockerPos_ = locker.pos + locker.qua.PosAxis(LOCLER_RELATIVEPOS_IN);

	preLockerPos_ = locker.pos + locker.qua.PosAxis(LOCLER_RELATIVEPOS_OUT);

	lockerQua_ = locker.qua;
}

void Player::OnHitEyePadLock(COL_HIT_DATA data)
{
	float neaDis = (neaEyeHitPos_ - eyeA_->pos).Length();

	Position3D hitNeaPos = {};
	if (holds_alternative<COL_HIT_DATA_LINE>(data.hitData)) {
		if (get<COL_HIT_DATA_LINE>(data.hitData).neaHitPos.has_value())
		{
			hitNeaPos = get<COL_HIT_DATA_LINE>(data.hitData).neaHitPos.value();
		}
	}
	float dis = (hitNeaPos - eyeA_->pos).Length();

	if (dis > neaDis) return;

	neaEyeHitPos_ = hitNeaPos;

	availableAction_ = AVAILABLE_ACTION::UNLOCK_PADLOCK;

	const auto& key = data.targetCollider.lock()->GetObjectUniqueKey();
	padLockUnlocker_->get().SetGetKeyObjectUniqueKey(key);

}

void Player::OnHitEyeLightBulb(COL_HIT_DATA data)
{
	float neaDis = (neaEyeHitPos_ - eyeA_->pos).Length();

	Position3D hitNeaPos = {};
	if (holds_alternative<COL_HIT_DATA_LINE>(data.hitData)) {
		if (get<COL_HIT_DATA_LINE>(data.hitData).neaHitPos.has_value())
		{
			hitNeaPos = get<COL_HIT_DATA_LINE>(data.hitData).neaHitPos.value();
		}
	}
	float dis = (hitNeaPos - eyeA_->pos).Length();

	if (dis > neaDis) return;

	neaEyeHitPos_ = hitNeaPos;

	availableAction_ = AVAILABLE_ACTION::GET_LIGHT;

	const auto& key = data.targetCollider.lock()->GetObjectUniqueKey();
	lightBulbPlacer_->get().SetGetLightBulbObjectUniqueKey(key);

}
