#pragma once
#include "../Actor.h"
#include "../../Component/Actor/State/StateMachine.h"

class InputController;
class GravityController;
class LineCollider;
class CylinderCollider;
class SphereCollider;
class LightBulbPlacer;
class KeyInventory;
class PadLockUnlocker;
class RotateBoneController;
class FlashLightHolder;

struct COL_HIT_DATA;

/// @brief プレイヤー(操作対象)
class Player final : public Actor
{
public:

	[[nodiscard]] Player(const string& uniqueKey);
	~Player();

	[[nodiscard]] const Transform& GetHeadTransform()const { return *camera_; }
	[[nodiscard]] const Transform& GetHeadSubTransform()const { return *cameraSub_; }


	/// @brief 状態
	enum class STATE
	{
		DEFAULT,
		HIDE,
		DEAD,
	};


private:

	/// @brief 実行可能なアクション
	enum class AVAILABLE_ACTION
	{
		NONE,
		PLACE_LIGHT,
		GET_LIGHT,
		GET_KEY,
		UNLOCK_PADLOCK,
		HIDE,
	};

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::PLAYER; }

	/// @brief 繰り返し初期化処理
	void InitActor() override;

	/// @brief 描画処理
	void DrawActor() override;

	/// @brief 基本的な更新処理
	void UpdateActor() override;

	/// @brief 通常Update後に走る処理
	void LateUpdateActor() override;

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdateActor() override;

	/// @brief 衝突判定後に走る処理設定
	void OnCollisionUpdateSetting() override;

	/// @brief アニメーション設定
	void AnimationSetting() override;

	/// @brief ステート設定
	void StateSetting();

	/// @brief 入力情報設定
	void InputSetting();

	/// @brief コライダー設定
	void ColliderSetting();

	/// @brief 重力設定
	void GravitySetting();

	/// @brief カメラ追従対象更新
	void UpdateCameraTarget();

	// ステート設定
	void DefaultStateSetting();
	void HideStateSetting();
	void DeadStateSetting();

	// 衝突後処理
	void OnHitMap(COL_HIT_DATA data);
	void OnHitEnemy(COL_HIT_DATA data);
	void OnHitEyeMap(COL_HIT_DATA data);
	void OnHitEyeKey(COL_HIT_DATA data);
	void OnHitEyeLocker(COL_HIT_DATA data);
	void OnHitEyePadLock(COL_HIT_DATA data);
	void OnHitEyeLightBulb(COL_HIT_DATA data);

	optional<reference_wrapper<InputController>> input_;				/// @brief 入力コントローラ
	optional<reference_wrapper<RotateBoneController>> rotateBoneCon_;	/// @brief 入力コントローラ
	optional<reference_wrapper<GravityController>> gravity_;			/// @brief 重力コントローラ
	optional<reference_wrapper<StateMachine<STATE>>> stateMachine_;		/// @brief ステートマシン
	optional<reference_wrapper<CylinderCollider>> bodyCollider_;		/// @brief 体コライダー
	optional<reference_wrapper<LineCollider>> eyeCollider_;				/// @brief 目線コライダー
	optional<reference_wrapper<LineCollider>> eyeLongCollider_;			/// @brief 目線コライダー長い
	optional<reference_wrapper<SphereCollider>> pointCollider_;			/// @brief エネミー被探索用コライダー
	optional<reference_wrapper<LightBulbPlacer>> lightBulbPlacer_;		/// @brief 電球設置
	optional<reference_wrapper<KeyInventory>> keyInventory_;			/// @brief 鍵情報を管理
	optional<reference_wrapper<PadLockUnlocker>> padLockUnlocker_;		/// @brief 南京錠を管理
	optional<reference_wrapper<FlashLightHolder>> flashLight_;			/// @brief 懐中電灯

	unique_ptr<Transform> head_;		/// @brief 頭回転用トランスフォーム
	unique_ptr<Transform> camera_;		/// @brief カメラ追従位置
	unique_ptr<Transform> cameraSub_;	/// @brief カメラ追従位置

	unique_ptr<Transform> eyeA_;		/// @brief 目線元
	unique_ptr<Transform> eyeB_;		/// @brief 目線先

	unique_ptr<Transform> eyeLongA_;	/// @brief 目線元(エネミー観察用)
	unique_ptr<Transform> eyeLongB_;	/// @brief 目線先(エネミー観察用)

	Position3D neaEyeHitPos_;			/// @brief 最も近い衝突座標(目線コライダー)

	Position3D lockerPos_;				/// @brief 目線があったロッカー座標
	Position3D preLockerPos_;			/// @brief ロッカーに入る前の座標

	Quaternion lockerQua_;				/// @brief  目線があったロッカー回転

	AVAILABLE_ACTION availableAction_;		/// @brief 現在有効なアクション
	AVAILABLE_ACTION preAvailableAction_;	/// @brief 1フレーム前に有効だったアクション

	// 効果音ハンドル
	int walkSoundHandle_;
	int runSoundHandle_;
	int hideSoundHandle_;
};

