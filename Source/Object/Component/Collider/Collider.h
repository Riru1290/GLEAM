#pragma once
#include "../Component.h"
#include "../../../Object/ObjectID.h"
#include "../../../Collision/ColliderInfo.h"
#include "../../../Collision/CollisionHitData.h"

class Transform;

/// @brief 衝突判定に使用する形状などを保持
class Collider : public Component
{
public:

	Collider();
	virtual ~Collider() = default;

	/// @brief コライダーの形を取得
	/// @return コライダーの形
	[[nodiscard]] virtual COLLISION_SHAPE_TYPE GetShapeType()const  = 0;

	/// @brief リジットボディか
	/// @return リジットボディか
	[[nodiscard]] const bool& IsRigidBody()const { return isRigidBody_; }

	/// @brief 不動のオブジェクトか
	/// @return 不動のオブジェクトか
	[[nodiscard]] const bool& IsStatic()const { return isStatic_; }

	/// @brief 重量取得
	/// @return 重量
	[[nodiscard]] const float& GetWeight()const { return weight_; }

	/// @brief 衝突後処理
	/// @param objectID オブジェクトID
	/// @param hitData 衝突時データ
	void OnHit(OBJECT_ID objectID, COL_HIT_DATA hitData);

	/// @brief 衝突後処理関数追加
	/// @param objectID 対象オブジェクト
	/// @param hitData 衝突後処理
	void AddOnHitFunction(OBJECT_ID objectID, function<void(COL_HIT_DATA)> onHit);

	/// @brief 追従対象セット
	/// @param transform 追従対象
	void SetFollower(Transform& transform);

	/// @brief 追従対象セット
	/// @param transform 追従対象
	void SetSubFollower(Transform& transform);

	/// @brief 相対座標セット
	/// @param relativePos 相対座標
	void SetRelativePosition(Position3D relativePos) { relativePos_ = relativePos; }

	/// @brief 物理特性状態セット
	/// @param isRigid 物理特性状態
	void SetRigidState(bool isRigid) { isRigidBody_ = isRigid; }

	/// @brief 不動状態セット
	/// @param isStatic 不動状態
	void SetStaticState(bool isStatic) { isStatic_ = isStatic; }

	/// @brief 重さセット
	/// @param weight 重さ
	void SetWeight(float weight) { weight_ = weight; }

	/// @brief コライダーの更新を追従対象に適用
	virtual void ApplyColliderUpdateToFollower() = 0;

	/// @brief オブジェクト識別キーセット
	/// @param uniqueKey オブジェクト識別キー
	void SetObjectUniqueKey(const string& uniqueKey) { objectUniqueKey_ = uniqueKey; }

	/// @brief オブジェクトIDセット
	/// @param objID オブジェクトID
	void SetObjectID(OBJECT_ID objID) { objectID_ = objID; }

	/// @brief 親オブジェクト識別キー取得
	/// @return 親オブジェクト識別キー
	[[nodiscard]] const string& GetObjectUniqueKey()const { return objectUniqueKey_; }

	/// @brief 親オブジェクトID取得
	/// @return 親オブジェクトID
	[[nodiscard]] const OBJECT_ID& GetObjectID()const { return objectID_; }

	/// @brief 追従対象トランスフォーム取得
	/// @return 追従対象トランスフォーム
	[[nodiscard]] const Transform& GetFollowerTransform()const { return follower_->get(); }

protected:

	/// @brief 更新処理
	virtual void UpdateCollider() = 0;

	/// @brief 通常Update前に走る処理
	virtual void PreUpdateCollider() = 0;

	/// @brief 描画処理
	virtual void DrawCollider() {}

	optional<reference_wrapper<Transform>> follower_;		/// @brief 追従対象
	Position3D relativePos_;								/// @brief 相対座標
	optional<reference_wrapper<Transform>> subFollower_;	/// @brief 追従対象(サブ)

	unordered_map<OBJECT_ID, vector<function<void(COL_HIT_DATA)>>> onHitList_;	/// @brief 衝突後処理リスト

	bool isRigidBody_;	/// @brief 物理特性を持つオブジェクトであるか
	bool isStatic_;		/// @brief 不動のオブジェクトであるか
	float weight_;		/// @brief 重さ(重力に関わる)

private:

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 通常Update前に走る処理
	void PreUpdateComponent() override;

	/// @brief 描画処理
	void DrawComponent() override;

	/// @brief 解放処理
	void ReleaseComponent() override;

	OBJECT_ID objectID_;
	string objectUniqueKey_;
};

