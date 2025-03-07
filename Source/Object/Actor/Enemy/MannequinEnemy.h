#pragma once
#include "../../Component/Actor/State/StateMachine.h"
#include "../Actor.h"

class AStarChaser;
class RouteWalker;
class SmoothRotateController;
class SphereCollider;

/// @brief マネキンエネミー
class MannequinEnemy final : public Actor
{
public:

	[[nodiscard]] MannequinEnemy(const string& uniqueKey);
	virtual ~MannequinEnemy() = default;

	/// @brief 死亡時カメラ用Transform取得
	/// @return 死亡時カメラ用Transform
	[[nodiscard]] const Transform& GetHeadTransform()const { return *head_; }

	/// @brief 状態
	enum class STATE
	{
		DEFAULT,
		CHASE_IDLE,
		CHASE_MOVE,
		RETURN,
		ATTACK,
	};

private:


	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::ENEMY; }

	/// @brief 繰り返し初期化処理
	void InitActor() override;

	/// @brief 描画処理
	void DrawActor() override;

	/// @brief 基本的な更新処理
	void UpdateActor() override;

	/// @brief 通常Update後に走る処理
	void LateUpdateActor() override;

	/// @brief 衝突判定後に走る処理設定
	void OnCollisionUpdateSetting() override;

	/// @brief アニメーション設定
	void AnimationSetting() override;

	/// @brief ステート設定
	void StateSetting();

	/// @brief コライダー設定
	void ColliderSetting();

	/// @brief カメラ追従対象更新
	void UpdateCameraTarget();

	// ステート設定

	/// @brief デフォルトステート設定
	void DefaultStateSetting();

	/// @brief 追従(待機)ステート設定
	void ChaseIdleStateSetting();

	/// @brief 追従(移動)ステート設定
	void ChaseMoveStateSetting();

	/// @brief 帰宅ステート設定
	void ReturnStateSetting();

	/// @brief 攻撃ステート設定
	void AttackStateSetting();

	// 衝突後処理

	/// @brief プレイヤーとの衝突後処理
	/// @param data 衝突時データ
	void OnHitPlayer(COL_HIT_DATA data);

	/// @brief プレイヤー点との衝突後処理
	/// @param data 衝突時データ
	void OnHitPlayerPoint(COL_HIT_DATA data);

	/// @brief プレイヤー視点との衝突後処理
	/// @param data 衝突時データ
	void OnHitPlayerEye(COL_HIT_DATA data);

	unique_ptr<Transform> head_;		/// @brief カメラ用

	optional<reference_wrapper<AStarChaser>> chaser_;				/// @brief 経路探索
	optional<reference_wrapper<AStarChaser>> returnChaser_;			/// @brief 元の経路に帰るためのA*
	optional<reference_wrapper<RouteWalker>> walker_;				/// @brief 巡回
	optional<reference_wrapper<SmoothRotateController>> rotateCon_;	/// @brief 回転
	optional<reference_wrapper<SphereCollider>> sphereCollider_;	/// @brief プレイヤー視点用コライダー
	optional<reference_wrapper<StateMachine<STATE>>> stateMachine_;	/// @brief ステートマシン

	Transform returnPos_;	/// @brief 帰宅座標

	bool isFoundPlayer_;	/// @brief プレイヤーを発見しているか
	bool isHitPlayer_;		/// @brief プレイヤーに衝突したか
	bool isFoundPlayerEye_;	/// @brief プレイヤーに発見されているか
};

