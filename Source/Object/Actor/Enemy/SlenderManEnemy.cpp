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
#include "../../../Sound/SoundManager.h"
#include "../../../Game/Event/EventManager.h"
#include "SlenderManEnemy.h"

namespace
{
	// ステート設定
	const string CHASE_TIME_KEY = "SLENDERMAN_CHASETIME";
	const float CHASE_TIME = (15.0f);

	const string SCREAM_TIME_KEY = "SLENDERMAN_SCREAMTIME";
	const float SCREAM_TIME = (2.5f);

	// アニメーション
	const string ANIMATION_KEY_IDLE = "IDLE";
	const string ANIMATION_KEY_WALK = "WALK";
	const string ANIMATION_KEY_RUN = "RUN";
	const string ANIMATION_KEY_SCREAM = "SCREAM";
	const string ANIMATION_KEY_ATTACK = "ATTACK";

	// コライダー設定
	const float CYLINDER_COL_RADIUS = (100.0f);
	const float CYLINDER_COL_HEIGHT = (180.0f);
	const Position3D CYLINDER_COL_RELATIVEPOS = { 0.0f, 85.0f, 0.0f };

	const float SPHERE_COL_RADIUS = (500.0f);
	const Position3D SPHERE_COL_RELATIVEPOS = { 0.0f, 0.0f, 300.0f };

	// 頭のボーン番号
	const int BONE_NO_HEAD = (5);

	// 巡回座標
	const vector<Position3D> ENEMY_WALK_ROUTE =
	{
		{1350.0f,-80.0f,-1350.0f},
		{1350.0f,-80.0f,1850.0f},
		{-1350.0f,-80.0f,1850.0f},
		{-1350.0f,-80.0f,-1350.0f}
	};
	const float WALK_IDLE_TIME = (3.0f);

	// 追従
	const float CHASE_LIMIT_DISTANCE_CHASE = (150.0f);
	const float CHASE_LIMIT_DISTANCE_RETURN = (150.0f);

	// カメラ設定
	const Position3D CAMERA_RELATIVEPOS_HEAD = { 0.0f,0.0f,50.0f };

	const float CAMERA_LERP_RATE = (0.5f);
	const Position3D CAMERA_RELATIVEPOS_DEAD = { 0.0f,0.0f,0.0f };
	const float CAMERA_SHAKE_TIME = (3.0f);
	const Vector3 CAMERA_SHAKE_POW = { 3.0f,0.0f,0.0f };
	const float CAMERA_SHAKE_SPEED = (30.0f);
}

SlenderManEnemy::SlenderManEnemy(const string& uniqueKey) :Actor(uniqueKey),
	returnPos_({}),
	isFoundPlayer_(false),
	isHitPlayer_(false)
{
	// モデル読み込み
	modelRenderer_->get().SetModel(ResourceMng.GetModel(RES_ID::MODEL_ENEMYSLENDERMAN));

	modelRenderer_->get().SetTransform(transform_->get());

	auto rotateCon = make_shared<SmoothRotateController>();
	rotateCon->SetTransform(transform_->get());
	AddComponent(rotateCon);
	rotateCon_ = *rotateCon;

	head_ = make_unique<Transform>();

	// コライダー設定
	ColliderSetting();

	// ステート設定
	StateSetting();
}

void SlenderManEnemy::DrawActor()
{
}

void SlenderManEnemy::UpdateActor()
{
}

void SlenderManEnemy::LateUpdateActor()
{
	auto dir = (transform_->get().pos - transform_->get().prePos).Normalized();

	if (dir != Vector3())
	{
		auto goal = Quaternion::LookRotation(dir);

		rotateCon_->get().SetGoalQuaternion(goal);
	}

	isFoundPlayer_ = false;
}

void SlenderManEnemy::OnCollisionUpdateSetting()
{
	onCollisionUpdateList_[COLLISION_FASE::FIRST] =
		[this] {

		UpdateCameraTarget();

		};
}

void SlenderManEnemy::AnimationSetting()
{	
	// アニメーションを適用させるモデルセット
	animationController_->get().SetAnimationModel(modelRenderer_->get());

	// アニメーション読み込み・セット
	auto animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_SLENDERMAN_IDLE).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_IDLE);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_SLENDERMAN_WALK).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_WALK);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_SLENDERMAN_RUN).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_RUN);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_SLENDERMAN_SCREAM).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_SCREAM);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_SLENDERMAN_ATTACK).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_ATTACK);

	animationController_->get().ChangeAnimation(ANIMATION_KEY_WALK);
}

void SlenderManEnemy::StateSetting()
{
	// ステート設定
	AddComponent(make_shared<StateMachine<STATE>>());

	stateMachine_ = GetComponent<StateMachine<STATE>>();

	DefaultStateSetting();
	ChaseStateSetting();
	ReturnStateSetting();
	ScreamStateSetting();
	AttackStateSetting();

	// 通常状態に設定
	stateMachine_->get().ChangeState(STATE::DEFAULT);
}

void SlenderManEnemy::ColliderSetting()
{
	// シリンダーコライダー設定
	// 主にマップやエネミーとの衝突判定に使う
	CYLINDER_COL_INFO cylinder;
	cylinder.radius = CYLINDER_COL_RADIUS;
	cylinder.height = CYLINDER_COL_HEIGHT;

	auto collider = make_shared<CylinderCollider>();
	collider->SetCylinderInfo(cylinder);
	collider->SetRelativePosition(CYLINDER_COL_RELATIVEPOS);
	collider->SetFollower(transform_->get());
	collider->AddOnHitFunction(OBJECT_ID::PLAYER, bind(&SlenderManEnemy::OnHitPlayer, this, placeholders::_1));

	AddComponent(collider);

	// コリジョンマネージャーに登録
	CollisionMng.RegisterCollider(collider, GetObjectUniqueKey(), OBJECT_ID::ENEMY);

	// 目線コライダー設定
	SPHERE_COL_INFO sphere;
	sphere.radius = SPHERE_COL_RADIUS;
	auto eye = make_shared<SphereCollider>();
	eye->SetSphereInfo(sphere);
	eye->SetRelativePosition(SPHERE_COL_RELATIVEPOS);
	eye->SetFollower(transform_->get());
	eye->AddOnHitFunction(OBJECT_ID::PLAYER_POINT, bind(&SlenderManEnemy::OnHitPlayerPoint, this, placeholders::_1));

	AddComponent(eye);

	// コリジョンマネージャーに登録
	CollisionMng.RegisterCollider(eye, GetObjectUniqueKey(), OBJECT_ID::ENEMY_EYE);
}

void SlenderManEnemy::UpdateCameraTarget()
{
	// マップとの衝突判定後のプレイヤー位置情報を使用して
	// 目線情報を更新する

	auto mat = modelRenderer_->get().GetGlobalAnimBoneMatrix(BONE_NO_HEAD);

	XMFLOAT3 pos = GetTranslationFromMatrix(mat);

	Quaternion rot = Quaternion::ExtractQuaternion(mat);

	head_->qua = rot;
	head_->pos = { pos.x,pos.y,pos.z };
	head_->pos += rot.PosAxis(CAMERA_RELATIVEPOS_HEAD);

	head_->qua = Quaternion::LookRotation((Position3D(pos.x,pos.y,pos.z) - head_->pos).Normalized());
}

void SlenderManEnemy::DefaultStateSetting()
{
	// ステート遷移条件設定
	unordered_map<STATE, function<bool()>> list = {
		{STATE::SCREAM , [this]
		{
			if (isFoundPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_SCREAM);
				MainTimer.SetTimer(SCREAM_TIME_KEY, SCREAM_TIME, true);
				int handle = SoundMng.PlaySoundObject(RES_ID::SOUND_ENEMY_SLENDERMAN_SCREAM,transform_->get().pos);
				SoundMng.SetSoundObjectVolume(handle,4.0f);
			}
			return isFoundPlayer_;
		}},
		{STATE::ATTACK , [this] {
			if (isHitPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_ATTACK);
			}
			return (isHitPlayer_);
		}},
	};

	// 通常状態
	unique_ptr<State<STATE>> defaultState =
		make_unique<State<STATE>>(list);

	// 指定ルートを徘徊
	auto routeWalker = make_shared<RouteWalker>();
	routeWalker->SetTransform(transform_->get());
	routeWalker->SetSpeed(CHASE_LIMIT_DISTANCE_CHASE);
	routeWalker->SetRandomState(true);
	routeWalker->CreateStraightRoute(ENEMY_WALK_ROUTE);

	routeWalker->InitStart(0);
	routeWalker->SetIdleAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE); });
	routeWalker->SetWalkAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_WALK); });
	routeWalker->SetIdleTime(WALK_IDLE_TIME);
	defaultState->AddComponent(routeWalker);

	walker_ = *routeWalker;

	// 通常ステート追加
	stateMachine_->get().RegisterState(STATE::DEFAULT, move(defaultState));
}

void SlenderManEnemy::ChaseStateSetting()
{
	// ステート遷移条件設定
	unordered_map<STATE, function<bool()>> list = {
		{STATE::ATTACK , [this] {
			if (isHitPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_ATTACK);
			}
			return (isHitPlayer_);
		}},
		{STATE::RETURN , [this] {
			bool isChange = (!isFoundPlayer_ && !MainTimer.InProgress(CHASE_TIME_KEY.c_str()));

			if (isChange)
			{
				returnPos_.pos = walker_->get().GetNeaPosition(transform_->get().pos);
				EventMng.Notify(EVENT_ID::UNDETECTED_BY_ENEMY);
			}
			return isChange;
		}}
	};

	// 追従状態
	unique_ptr<State<STATE>> chaseState =
		make_unique<State<STATE>>(list);

	// A*追跡
	auto chaser = make_shared<AStarChaser>();
	chaser->SetMoveAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_RUN); });
	chaser->SetIdleAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE); });
	chaser->SetTransform(transform_->get());
	chaser->SetLimitDistance(CHASE_LIMIT_DISTANCE_RETURN);
	chaseState->AddComponent(chaser);

	chaser_ = *chaser;

	// 追従ステート追加
	stateMachine_->get().RegisterState(STATE::CHASE, move(chaseState));
}

void SlenderManEnemy::ReturnStateSetting()
{
	// ステート遷移条件設定
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
				int handle = SoundMng.PlaySoundObject(RES_ID::SOUND_ENEMY_SLENDERMAN_SCREAM, transform_->get().pos);
				SoundMng.SetSoundObjectVolume(handle, 4.0f);
			}
			return isFoundPlayer_;
		}},
		{STATE::ATTACK , [this] {
			if (isHitPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_ATTACK);
			}
			return (isHitPlayer_);
		}},
	};

	// 帰宅状態
	unique_ptr<State<STATE>> returnState =
		make_unique<State<STATE>>(list);

	// A*追跡
	auto chaser = make_shared<AStarChaser>();
	chaser->SetMoveAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_WALK); });
	chaser->SetIdleAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE); });

	chaser->SetTransform(transform_->get());
	chaser->SetTargetTransform(returnPos_);
	returnState->AddComponent(chaser);

	returnChaser_ = *chaser;

	// 追従ステート追加
	stateMachine_->get().RegisterState(STATE::RETURN, move(returnState));
}

void SlenderManEnemy::ScreamStateSetting()
{
	// ステート遷移条件設定
	unordered_map<STATE, function<bool()>> list = {
		{STATE::CHASE , [this] {
			bool isChange = (!MainTimer.InProgress(SCREAM_TIME_KEY.c_str()));
			if (isChange)
			{
				MainTimer.SetTimer(CHASE_TIME_KEY.c_str(), CHASE_TIME, true);

				EventMng.Notify(EVENT_ID::DETECTED_BY_ENEMY);
			}
			
			return isChange;
		}},
		{STATE::ATTACK , [this] {
			if (isHitPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_ATTACK);
			}
			return (isHitPlayer_);
		}},
	};

	// 叫び状態
	unique_ptr<State<STATE>> screamState =
		make_unique<State<STATE>>(list);

	screamState->onCollisionUpdateList_[COLLISION_FASE::FIRST] =
	[this]
	{
		Quaternion goal = Quaternion::LookRotation(chaser_->get().GetToTargetDir());
		rotateCon_->get().SetGoalQuaternion(goal);
	};

	// 叫びステート追加
	stateMachine_->get().RegisterState(STATE::SCREAM, move(screamState));
}

void SlenderManEnemy::AttackStateSetting()
{
	// ステート遷移条件設定
	unordered_map<STATE, function<bool()>> list = {
		{STATE::DEFAULT , [this] {
			return (animationController_->get().IsEndAnimation(ANIMATION_KEY_ATTACK));
		}}
	};

	// 攻撃状態
	unique_ptr<State<STATE>> attackState =
		make_unique<State<STATE>>(list);

	// 攻撃ステート追加
	stateMachine_->get().RegisterState(STATE::ATTACK, move(attackState));
}

void SlenderManEnemy::OnHitPlayer(COL_HIT_DATA data)
{
	isHitPlayer_ = true;
	MainCamera.ChangeState(Camera::STATE::LERP_FOLLOW);
	MainCamera.SetLerpFollowConfig(*head_, CAMERA_LERP_RATE);
	MainCamera.SetFollowRelativePos(CAMERA_RELATIVEPOS_DEAD);
	MainCamera.Shake(CAMERA_SHAKE_TIME, CAMERA_SHAKE_POW, CAMERA_SHAKE_SPEED);
}

void SlenderManEnemy::OnHitPlayerPoint(COL_HIT_DATA data)
{
	isFoundPlayer_ = true;
}
