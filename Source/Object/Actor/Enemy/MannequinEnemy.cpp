#ifdef _DEBUG
#include "../../../../External/imgui/imgui.h"
#include "../../../../External/imgui/imgui_impl_win32.h"
#include "../../../../External/imgui/imgui_impl_dx12.h"
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
#include "MannequinEnemy.h"

namespace
{
	// ステート設定
	const string CHASE_TIME_KEY = "MANNEQUIN_CHASETIME";
	const float CHASE_TIME = (20.0f);

	// アニメーション
	const string ANIMATION_KEY_IDLE = "IDLE";
	const string ANIMATION_KEY_RUN = "RUN";
	const string ANIMATION_KEY_ATTACK = "ATTACK";

	// コライダー設定
	const float CYLINDER_COL_RADIUS = (90.0f);
	const float CYLINDER_COL_HEIGHT = (180.0f);
	const Position3D CYLINDER_COL_RELATIVEPOS = { 0.0f, 85.0f, 0.0f };

	const float SPHERE_COL_RADIUS = (500.0f);
	const Position3D SPHERE_COL_RELATIVEPOS = { 0.0f, 0.0f, 300.0f };

	const float SPHERE_COL_RADIUS_PLAYEREYE = (1000.0f);
	const Position3D SPHERE_COL_RELATIVEPOS_PLAYEREYE = { 0.0f, 85.0f, -1000.0f };

	// 追従
	const float CHASE_LIMIT_DISTANCE = (150.0f);

	// 頭のボーン番号
	const int BONE_NO_HEAD = (3);


	// カメラ設定
	const Position3D CAMERA_RELATIVEPOS_HEAD = { 0.0f,0.0f,50.0f };

	const float CAMERA_LERP_RATE = (0.5f);
	const Position3D CAMERA_RELATIVEPOS_DEAD = { 0.0f,0.0f,0.0f };
	const float CAMERA_SHAKE_TIME = (3.0f);
	const Vector3 CAMERA_SHAKE_POW = { 3.0f,0.0f,0.0f };
	const float CAMERA_SHAKE_SPEED = (30.0f);
}

MannequinEnemy::MannequinEnemy(const string& uniqueKey) :Actor(uniqueKey),
	returnPos_({}),
	isFoundPlayer_(false),
	isHitPlayer_(false),
	isFoundPlayerEye_(false)
{
	// モデル読み込み
	modelRenderer_->get().SetModel(ResourceMng.GetModel(RES_ID::MODEL_ENEMYMANNEQUIN));

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

void MannequinEnemy::InitActor()
{
	returnPos_ = transform_->get();
}

void MannequinEnemy::DrawActor()
{
}

void MannequinEnemy::UpdateActor()
{
#ifdef _DEBUG
	auto state = stateMachine_->get().GetStateKey();
	switch (state)
	{
	case MannequinEnemy::STATE::DEFAULT:
		ImGui::Text("NowState : DEFAULT");
		break;
	case MannequinEnemy::STATE::CHASE_IDLE:
		ImGui::Text("NowState : CHASE_IDLE");
		break;
	case MannequinEnemy::STATE::CHASE_MOVE:
		ImGui::Text("NowState : CHASE_MOVE");
		break;
	case MannequinEnemy::STATE::RETURN:
		ImGui::Text("NowState : RETURN");
		break;
	case MannequinEnemy::STATE::ATTACK:
		ImGui::Text("NowState : ATTACK");
		break;
	}
#endif
}

void MannequinEnemy::LateUpdateActor()
{
	auto dir = (transform_->get().pos - transform_->get().prePos).Normalized();

	if (dir != Vector3())
	{
		auto goal = Quaternion::LookRotation(dir);

		rotateCon_->get().SetGoalQuaternion(goal);
	}

	isFoundPlayer_ = false;
	isFoundPlayerEye_ = false;
	isHitPlayer_ = false;
}

void MannequinEnemy::OnCollisionUpdateSetting()
{
	onCollisionUpdateList_[COLLISION_FASE::FIRST] =
		[this] {

		UpdateCameraTarget();

		Position3D colPos = {};

		Position3D pos = transform_->get().pos;
		Position3D cPos = MainCamera.GetPos();
		pos.y = 0.0f;
		cPos.y = 0.0f;

		Vector3 toCameraVec = (cPos - pos).Normalized();

		colPos = transform_->get().pos +
			Quaternion::LookRotation(toCameraVec).PosAxis(SPHERE_COL_RELATIVEPOS_PLAYEREYE);
		sphereCollider_->get().SetPosition(colPos);

		};
}

void MannequinEnemy::AnimationSetting()
{
	// アニメーションを適用させるモデルセット
	animationController_->get().SetAnimationModel(modelRenderer_->get());

	// アニメーション読み込み・セット
	auto animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_MANNEQUIN_IDLE).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_IDLE);
	animationController_->get().SetIsStop(true, ANIMATION_KEY_IDLE);

	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_MANNEQUIN_RUN).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_RUN);


	animData = ResourceMng.GetAnimation(RES_ID::ANIMATION_ENEMY_MANNEQUIN_ATTACK).GetData();
	animationController_->get().AddAnimation(animData, ANIMATION_KEY_ATTACK);

	animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE);
}

void MannequinEnemy::StateSetting()
{
	// ステート設定
	AddComponent(make_shared<StateMachine<STATE>>());

	stateMachine_ = GetComponent<StateMachine<STATE>>();

	DefaultStateSetting();
	ChaseIdleStateSetting();
	ChaseMoveStateSetting();
	ReturnStateSetting();
	AttackStateSetting();

	// 通常状態に設定
	stateMachine_->get().ChangeState(STATE::DEFAULT);
}

void MannequinEnemy::ColliderSetting()
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
	collider->AddOnHitFunction(OBJECT_ID::PLAYER, bind(&MannequinEnemy::OnHitPlayer, this, placeholders::_1));

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
	eye->AddOnHitFunction(OBJECT_ID::PLAYER_POINT, bind(&MannequinEnemy::OnHitPlayerPoint, this, placeholders::_1));

	AddComponent(eye);

	// コリジョンマネージャーに登録
	CollisionMng.RegisterCollider(eye, GetObjectUniqueKey(), OBJECT_ID::ENEMY_EYE);

	sphere = {};
	sphere.radius = SPHERE_COL_RADIUS_PLAYEREYE;
	auto point = make_shared<SphereCollider>();
	point->SetSphereInfo(sphere);
	point->AddOnHitFunction(OBJECT_ID::PLAYER_EYE_LONG, bind(&MannequinEnemy::OnHitPlayerEye, this, placeholders::_1));

	sphereCollider_ = *point;
	AddComponent(point);

	// コリジョンマネージャーに登録
	CollisionMng.RegisterCollider(point, GetObjectUniqueKey(), OBJECT_ID::ENEMY_POINT);
}

void MannequinEnemy::UpdateCameraTarget()
{
	// マップとの衝突判定後のプレイヤー位置情報を使用して
	// 目線情報を更新する

	auto mat = modelRenderer_->get().GetGlobalAnimBoneMatrix(BONE_NO_HEAD);

	XMFLOAT3 pos = GetTranslationFromMatrix(mat);

	Quaternion rot = Quaternion::ExtractQuaternion(mat);

	head_->qua = rot;
	head_->pos = { pos.x,pos.y,pos.z };
	head_->pos += rot.PosAxis(CAMERA_RELATIVEPOS_HEAD);

	head_->qua = Quaternion::LookRotation((Position3D(pos.x, pos.y, pos.z) - head_->pos).Normalized());

}

void MannequinEnemy::DefaultStateSetting()
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
		{STATE::CHASE_IDLE , [this]
		{
			if (isFoundPlayer_)
			{
				EventMng.Notify(EVENT_ID::DETECTED_BY_ENEMY);
				animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE);
				MainTimer.SetTimer(CHASE_TIME_KEY, CHASE_TIME, true);
			}
			return isFoundPlayer_;
		}},

	};

	// 通常状態
	unique_ptr<State<STATE>> defaultState =
		make_unique<State<STATE>>(list);


	defaultState->onCollisionUpdateList_[COLLISION_FASE::FIRST] = {
		[this]
		{
			rotateCon_->get().SetGoalQuaternion(returnPos_.qua);
		}
	};


	// 通常ステート追加
	stateMachine_->get().RegisterState(STATE::DEFAULT, move(defaultState));
}

void MannequinEnemy::ChaseIdleStateSetting()
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
		{STATE::CHASE_MOVE , [this] {
			if (!isFoundPlayerEye_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_RUN);
			}
			return (!isFoundPlayerEye_);
		}},
		{STATE::RETURN , [this] {
			bool isChange = (!isFoundPlayer_ && !MainTimer.InProgress(CHASE_TIME_KEY.c_str()));
			if (isChange)
			{
				EventMng.Notify(EVENT_ID::UNDETECTED_BY_ENEMY);
			}

			return isChange;
		}}
	};

	// 追従状態
	unique_ptr<State<STATE>> chaseState =
		make_unique<State<STATE>>(list);

	// 追従ステート追加
	stateMachine_->get().RegisterState(STATE::CHASE_IDLE, move(chaseState));
}

void MannequinEnemy::ChaseMoveStateSetting()
{
	// ステート遷移条件設定
	unordered_map<STATE, function<bool()>> list = {
		{STATE::CHASE_IDLE , [this] {
			if (isFoundPlayerEye_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE);
			}
			return (isFoundPlayerEye_);
		}},
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
	chaser->SetLimitDistance(CHASE_LIMIT_DISTANCE);
	chaseState->AddComponent(chaser);

	chaser_ = *chaser;

	// 追従ステート追加
	stateMachine_->get().RegisterState(STATE::CHASE_MOVE, move(chaseState));
}


void MannequinEnemy::ReturnStateSetting()
{
	// ステート遷移条件設定
	unordered_map<STATE, function<bool()>> list = {
		{STATE::DEFAULT , [this] {
			bool isGoal = returnChaser_->get().IsGoal();
			if (isGoal)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE);
			}
			return isGoal;
		}},
		{STATE::CHASE_IDLE , [this]
		{
			if (isFoundPlayer_)
			{
				animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE);
				MainTimer.SetTimer(CHASE_TIME_KEY, CHASE_TIME, true);
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
	chaser->SetMoveAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_RUN); });
	chaser->SetIdleAction([this] {animationController_->get().ChangeAnimation(ANIMATION_KEY_IDLE); });
	chaser->SetTransform(transform_->get());
	chaser->SetTargetTransform(returnPos_);
	returnState->AddComponent(chaser);

	returnChaser_ = *chaser;

	// 追従ステート追加
	stateMachine_->get().RegisterState(STATE::RETURN, move(returnState));
}

void MannequinEnemy::AttackStateSetting()
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

void MannequinEnemy::OnHitPlayer(COL_HIT_DATA data)
{
	isHitPlayer_ = true;
	MainCamera.ChangeState(Camera::STATE::LERP_FOLLOW);
	MainCamera.SetLerpFollowConfig(*head_, CAMERA_LERP_RATE);
	MainCamera.SetFollowRelativePos(CAMERA_RELATIVEPOS_DEAD);
	MainCamera.Shake(CAMERA_SHAKE_TIME, CAMERA_SHAKE_POW, CAMERA_SHAKE_SPEED);
}

void MannequinEnemy::OnHitPlayerPoint(COL_HIT_DATA data)
{
	isFoundPlayer_ = true;
}

void MannequinEnemy::OnHitPlayerEye(COL_HIT_DATA data)
{
	isFoundPlayerEye_ = true;
}
