#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Object/ObjectID.h"
#include "CollisionHitData.h"
#include "CollisionFase.h"

#define CollisionMng CollisionManager::GetInstance()

class Collider;
class PointCollider;
class Component;


namespace
{
	static constexpr int COLLISION_RECURSION_COUNT_DEFAULT = (1);
}

/// @brief 衝突判定管理
class CollisionManager final : public StaticSingleton<CollisionManager>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief コライダーを登録
	/// @param collider コライダー
	/// @param objectUniqueKey オブジェクト識別キー
	/// @param objectID オブジェクトID
	void RegisterCollider(shared_ptr<Collider> collider, const string& objectUniqueKey, OBJECT_ID objectID);

	/// @brief コライダーの登録解除
	/// @param collider コライダー
	void UnregisterCollider(shared_ptr<Component> collider);

	/// @brief 指定フェーズの衝突判定を行う
	/// @param fase 衝突判定フェーズ
	/// @param recursionCount 何度再帰的に衝突判定を行うか
	void Collision(COLLISION_FASE currentFase, int recursionCount = COLLISION_RECURSION_COUNT_DEFAULT);

private:

	CollisionManager();
	~CollisionManager();
	PROOF_OF_STATIC_SINGLETON(CollisionManager);

	/// @brief 衝突判定リスト設定
	void CollisionListSetting();

	/// @brief コライダー同士の衝突判定
	/// @param objA コライダーA
	/// @param objB コライダーB
	void CollisionPerObject(OBJECT_ID objA, OBJECT_ID objB);

	/// @brief マウスカーソル用コライダーを更新
	void UpdateCursorCollider();

	vector<tuple<OBJECT_ID, OBJECT_ID, COLLISION_FASE>> collisionList_;		/// @brief 衝突リスト

	map<OBJECT_ID, vector<weak_ptr<Collider>>> colliders_;					/// @brief オブジェクトIDごとのコライダー配列

	shared_ptr<PointCollider> cursorCollider_;								/// @brief マウスカーソル用コライダー

	UINT collisionCount_;													/// @brief オブジェクト同士が衝突した数
};


