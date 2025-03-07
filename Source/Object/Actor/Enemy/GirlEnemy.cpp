#ifdef _DEBUG
#include "../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../../Resource/ResourceManager.h"
#include "../../../Resource/Animation/Animation.h"
#include "../../Component/Common/Transform.h"
#include "../../Component/Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "../../Component/Actor/AnimationController.h"
#include "../../Component/Actor/Algorithm/AStarChaser.h"
#include "../../Component/Actor/Algorithm/RouteWalker.h"
#include "../../Component/Controller/SmoothRotateController.h"
#include "../../Component/Collider/Colliders/SphereCollider.h"
#include "../../Component/Collider/Colliders/CylinderCollider.h"
#include "../../../Collision/CollisionManager.h"
#include "../../../Camera/Camera.h"
#include "../../../Game/Event/EventManager.h"
#include "../../../Sound/SoundManager.h"
#include "GirlEnemy.h"

namespace
{
	// �X�e�[�g�ݒ�
	const string CHASE_TIME_KEY = "GIRL_CHASETIME";
	const float CHASE_TIME = (15.0f);

	const string SCREAM_TIME_KEY = "GIRL_SCREAMTIME";
	const float SCREAM_TIME = (2.5f);

	// �A�j���[�V����
	const string ANIMATION_KEY_IDLE = "IDLE";
	const string ANIMATION_KEY_WALK = "WALK";
	const string ANIMATION_KEY_RUN = "RUN";
	const string ANIMATION_KEY_SCREAM = "SCREAM";
	const string ANIMATION_KEY_ATTACK = "ATTACK";

	// �R���C�_�[�ݒ�
	const float CYLINDER_COL_RADIUS = (80.0f);
	const float CYLINDER_COL_HEIGHT = (180.0f);
	const Position3D CYLINDER_COL_RELATIVEPOS = { 0.0f, 85.0f, 0.0f };

	const float SPHERE_COL_RADIUS = (500.0f);
	const Position3D SPHERE_COL_RELATIVEPOS = { 0.0f, 0.0f, 300.0f };

	// ���̃{�[���ԍ�
	const int BONE_NO_HEAD = (5);

	// ������W
	const vector<Position3D> ENEMY_WALK_ROUTE =
	{
		{ -1950.0f, -80.0f, 1450.0f },
		{ -1950.0f,-80.0f,250.0f },
		{ -3150.0f,-80.0f,250.0f },
		{ -3150.0f,-80.0f,1450.0f }
	};
	const float WALK_IDLE_TIME = (3.0f);

	// �Ǐ]
	const float CHASE_LIMIT_DISTANCE_CHASE = (140.0f);
	const float CHASE_LIMIT_DISTANCE_RETURN = (120.0f);

	// �J�����ݒ�
	const Position3D CAMERA_RELATIVEPOS_HEAD = { 0.0f,0.0f,50.0f };

	const float CAMERA_LERP_RATE = (0.5f);
	const Position3D CAMERA_RELATIVEPOS_DEAD = { 0.0f,0.0f,0.0f };
	const float CAMERA_SHAKE_TIME = (3.0f);
	const Vector3 CAMERA_SHAKE_POW = { 3.0f,0.0f,0.0f };
	const float CAMERA_SHAKE_SPEED = (30.0f);

}

GirlEnemy::GirlEnemy(const string& uniqueKey) : Actor(uniqueKey),
	returnPos_({}),
	isFoundPlayer_(false),
	isHitPlayer_(false)
{
	// ���f���ǂݍ���
	modelRenderer_->get().SetModel(ResourceMng.GetModel(RES_ID::MODEL_ENEMYGIRL));

	modelRenderer_->get().SetTransform(transform_->get());

	auto rotateCon = make_shared<SmoothRotateController>();
	rotateCon->SetTransform(transform_->get());
	AddComponent(rotateCon);
	rotateCon_ = *rotateCon;

	head_ = make_unique<Transform>();

	// �R���C�_�[�ݒ�
	ColliderSetting();

	// �X�e�[�g�ݒ�
	StateSetting();
}

void GirlEnemy::DrawActor()
{
}

void GirlEnemy::UpdateActor()
{
}

void GirlEnemy::LateUpdateActor()
{
	auto dir = (transform_->get().pos - transform_->get().prePos).Normalized();

	if (dir != Vector3())
	{
		auto goal = Quaternion::LookRotation(dir);

		rotateCon_->get().SetGoalQuaternion(goal);
	}

	isFoundPlayer_ = false;
}

void GirlEnemy::OnCollisionUpdateSetting()
{
	onCollisionUpdateList_[COLLISION_FASE::FIRST] =
		[this] {

		UpdateCameraTarget();

		};
}

void GirlEnemy::AnimationSetting()
{
	// �A�j���[�V������K�p�����郂�f���Z�b�g
	animationController_->get().SetAnimationModel(modelRenderer_->get());

	// �A�j���[�V�����ǂݍ��݁E�Z�b�g
	auto animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_GIRL_IDLE).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_IDLE);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_GIRL_WALK).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_WALK);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_GIRL_RUN).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_RUN);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_GIRL_SCREAM).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_SCREAM);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_GIRL_ATTACK).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_ATTACK);

	animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE);
}

void GirlEnemy::StateSetting()
{
	// �X�e�[�g�ݒ�
	AddComponent(make_shared<StateMachine<STATE>>());

	stateMachine_ = GetComponent<StateMachine<STATE>>();

	DefaultStateSetting();
	ChaseStateSetting();
	ReturnStateSetting();
	ScreamStateSetting();
	AttackStateSetting();
	StopStateSetting();

	// �ҋ@��Ԃɐݒ�
	stateMachine_->get().ChangeState(STATE::STOP);

}

void GirlEnemy::ColliderSetting()
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
	collider->AddOnHitFunction(OBJECT_ID::PLAYER, bind(&GirlEnemy::OnHitPlayer, this, placeholders::_1));

	AddComponent(collider);

	// �R���W�����}�l�[�W���[�ɓo�^
	CollisionMng.RegisterCollider(collider, GetObjectUniqueKey(), OBJECT_ID::ENEMY);

	// �ڐ��R���C�_�[�ݒ�
	SPHERE_COL_INFO sphere;
	sphere.radius = SPHERE_COL_RADIUS;
	auto eye = make_shared<SphereCollider>();
	eye->SetSphereInfo(sphere);
	eye->SetRelativePosition(SPHERE_COL_RELATIVEPOS);
	eye->SetFollower(transform_->get());
	eye->AddOnHitFunction(OBJECT_ID::PLAYER_POINT, bind(&GirlEnemy::OnHitPlayerPoint, this, placeholders::_1));

	AddComponent(eye);

	// �R���W�����}�l�[�W���[�ɓo�^
	CollisionMng.RegisterCollider(eye, GetObjectUniqueKey(), OBJECT_ID::ENEMY_EYE);
}

void GirlEnemy::UpdateCameraTarget()
{
	// �}�b�v�Ƃ̏Փ˔����̃v���C���[�ʒu�����g�p����
	// �ڐ������X�V����

	auto mat = modelRenderer_->get().GetGlobalAnimBoneMatrix(BONE_NO_HEAD);

	XMFLOAT3 pos = GetTranslationFromMatrix(mat);

	Quaternion rot = Quaternion::ExtractQuaternion(mat);

	head_->qua = rot;
	head_->pos = { pos.x,pos.y,pos.z };
	head_->pos += rot.PosAxis(CAMERA_RELATIVEPOS_HEAD);

	head_->qua = Quaternion::LookRotation((Position3D(pos.x, pos.y, pos.z) - head_->pos).Normalized());

#ifdef _DEBUG
	auto dPos = head_->pos + head_->qua.PosAxis({ 0.0f,0.0f,100.0f });
	DebugRender.DrawSphere(head_->pos, 10.0f, 0xff0000, 255.0f, true);
	DebugRender.DrawSphere(dPos, 10.0f, 0x00ff00, 255.0f, true);
#endif
}

void GirlEnemy::DefaultStateSetting()
{
	// �X�e�[�g�J�ڏ����ݒ�
	unordered_map<STATE, function<bool()>> list = {
		{STATE::SCREAM , [this]{
			if (isFoundPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_SCREAM);
				MainTimer.SetTimer(SCREAM_TIME_KEY, SCREAM_TIME, true);
				int handle = SoundMng.PlaySoundObject(RES_ID::SOUND_ENEMY_GIRL_SCREAM, transform_->get().pos);
				SoundMng.SetSoundObjectVolume(handle, 2.0f);
			}
			return isFoundPlayer_;
		}},
		{STATE::ATTACK , [this] {
			if (isHitPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_ATTACK);
			}
			return (isHitPlayer_);
		}}
	};

	// �ʏ���
	unique_ptr<State<STATE>> defaultState =
		make_unique<State<STATE>>(list);

	// �w�胋�[�g��p�j
	auto routeWalker = make_shared<RouteWalker>();
	routeWalker->SetTransform(transform_->get());
	routeWalker->CreateStraightRoute(ENEMY_WALK_ROUTE);

	routeWalker->InitStart(0);
	routeWalker->SetIdleAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE); });
	routeWalker->SetWalkAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_WALK); });
	routeWalker->SetIdleTime(WALK_IDLE_TIME);
	defaultState->AddComponent(routeWalker);

	walker_ = *routeWalker;

	// �ʏ�X�e�[�g�ǉ�
	stateMachine_->get().RegisterState(STATE::DEFAULT, move(defaultState));
}

void GirlEnemy::ChaseStateSetting()
{
	// �X�e�[�g�J�ڏ����ݒ�
	unordered_map<STATE, function<bool()>> list = {
		{STATE::ATTACK , [this] {
			if (isHitPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_ATTACK);
			}
			return (isHitPlayer_);
		}},
		{STATE::RETURN , [this] {
			bool isChange = (!MainTimer.InProgress(CHASE_TIME_KEY.c_str()));

			if (isChange)
			{
				EventMng.Notify(EVENT_ID::UNDETECTED_BY_ENEMY);
				returnPos_.pos = walker_->get().GetNeaPosition(transform_->get().pos);
			}
			return isChange;
		}}
	};

	// �Ǐ]���
	unique_ptr<State<STATE>> chaseState =
		make_unique<State<STATE>>(list);

	// A*�ǐ�
	auto chaser = make_shared<AStarChaser>();
	chaser->SetMoveAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_RUN); });
	chaser->SetIdleAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE); });
	chaser->SetTransform(transform_->get());
	chaser->SetLimitDistance(CHASE_LIMIT_DISTANCE_CHASE);
	chaser_ = *chaser;
	chaseState->AddComponent(chaser);

	// �Ǐ]�X�e�[�g�ǉ�
	stateMachine_->get().RegisterState(STATE::CHASE, move(chaseState));
}

void GirlEnemy::ReturnStateSetting()
{
	// �X�e�[�g�J�ڏ����ݒ�
	unordered_map<STATE, function<bool()>> list = {
		{STATE::DEFAULT , [this] {
			bool isGoal = returnChaser_->get().IsGoal();
			if (isGoal)
			{
				walker_->get().SetNextNodeToNearestRoutePoint(transform_->get().pos);
			}
			return isGoal;
		}},
		{STATE::SCREAM , [this]
		{
			if (isFoundPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_SCREAM);
				MainTimer.SetTimer(SCREAM_TIME_KEY, SCREAM_TIME, true);
				int handle = SoundMng.PlaySoundObject(RES_ID::SOUND_ENEMY_GIRL_SCREAM, transform_->get().pos);
				SoundMng.SetSoundObjectVolume(handle, 2.0f);
			}
			return isFoundPlayer_;
		}},
		{STATE::ATTACK , [this] {
			if (isHitPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_ATTACK);
			}
			return (isHitPlayer_);
		}}
	};

	// �A����
	unique_ptr<State<STATE>> returnState =
		make_unique<State<STATE>>(list);

	// A*�ǐ�
	auto chaser = make_shared<AStarChaser>();
	chaser->SetMoveAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_WALK); });
	chaser->SetIdleAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE); });
	chaser->SetLimitDistance(CHASE_LIMIT_DISTANCE_RETURN);
	chaser->SetTransform(transform_->get());
	chaser->SetTargetTransform(returnPos_);
	returnState->AddComponent(chaser);

	returnChaser_ = *chaser;

	// �Ǐ]�X�e�[�g�ǉ�
	stateMachine_->get().RegisterState(STATE::RETURN, move(returnState));
}

void GirlEnemy::ScreamStateSetting()
{
	// �X�e�[�g�J�ڏ����ݒ�
	unordered_map<STATE, function<bool()>> list = {
		{STATE::CHASE , [this] {
			bool isChange = (!MainTimer.InProgress(SCREAM_TIME_KEY.c_str()));
			if (isChange)
			{
				EventMng.Notify(EVENT_ID::DETECTED_BY_ENEMY);
				MainTimer.SetTimer(CHASE_TIME_KEY.c_str(), CHASE_TIME, true);
			}

			return isChange;
		}},
		{STATE::ATTACK , [this] {
			if (isHitPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_ATTACK);
			}
			return (isHitPlayer_);
		}}
	};

	// ���я��
	unique_ptr<State<STATE>> screamState =
		make_unique<State<STATE>>(list);

	screamState->onCollisionUpdateList_[COLLISION_FASE::FIRST] =
		[this]
		{
			Quaternion goal = Quaternion::LookRotation(chaser_->get().GetToTargetDir());
			rotateCon_->get().SetGoalQuaternion(goal);
		};

	// ���уX�e�[�g�ǉ�
	stateMachine_->get().RegisterState(STATE::SCREAM, move(screamState));

	// �C�x���g�ɂ���Ă��J��
	EventMng.RegisterEventAction(EVENT_ID::GET_KEY_B, [this] {
		if (isFoundPlayer_)
		{
			MainTimer.SetTimer(SCREAM_TIME_KEY.c_str(), SCREAM_TIME, true);
			animationController_->get().ChangeAnimation(ANIMATION_KEY_SCREAM);
			stateMachine_->get().ChangeState(STATE::SCREAM);
			int handle = SoundMng.PlaySoundObject(RES_ID::SOUND_ENEMY_GIRL_SCREAM, transform_->get().pos);
			SoundMng.SetSoundObjectVolume(handle, 2.0f);
		}
		else
		{
			returnPos_.pos = walker_->get().GetNeaPosition(transform_->get().pos);
			stateMachine_->get().ChangeState(STATE::RETURN);
			int handle = SoundMng.PlaySoundObject(RES_ID::SOUND_ENEMY_GIRL_SCREAM, transform_->get().pos);
			SoundMng.SetSoundObjectVolume(handle, 2.0f);
		}

		});
}

void GirlEnemy::AttackStateSetting()
{
	// �X�e�[�g�J�ڏ����ݒ�
	unordered_map<STATE, function<bool()>> list = {
		{STATE::DEFAULT , [this] {
			return (animationController_->get().IsEndAnimation(ANIMATION_KEY_ATTACK));
		}}
	};

	// �U�����
	unique_ptr<State<STATE>> attackState =
		make_unique<State<STATE>>(list);

	// �U���X�e�[�g�ǉ�
	stateMachine_->get().RegisterState(STATE::ATTACK, move(attackState));
}

void GirlEnemy::StopStateSetting()
{
	// �X�e�[�g�J�ڏ����ݒ�
	unordered_map<STATE, function<bool()>> list = {
		{STATE::ATTACK , [this] {
			if (isHitPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_ATTACK);
			}
			return (isHitPlayer_);
		}}
	};

	// �ʏ���
	unique_ptr<State<STATE>> stopState =
		make_unique<State<STATE>>(list);

	// �ʏ�X�e�[�g�ǉ�
	stateMachine_->get().RegisterState(STATE::STOP, move(stopState));
}

void GirlEnemy::OnHitPlayer(COL_HIT_DATA data)
{
	isHitPlayer_ = true;
	MainCamera.ChangeState(Camera::STATE::LERP_FOLLOW);
	MainCamera.SetLerpFollowConfig(*head_, CAMERA_LERP_RATE);
	MainCamera.SetFollowRelativePos(CAMERA_RELATIVEPOS_DEAD);
	MainCamera.Shake(CAMERA_SHAKE_TIME, CAMERA_SHAKE_POW, CAMERA_SHAKE_SPEED);
}

void GirlEnemy::OnHitPlayerPoint(COL_HIT_DATA data)
{
	isFoundPlayer_ = true;
}
