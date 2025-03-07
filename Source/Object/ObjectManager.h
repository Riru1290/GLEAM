#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Collision/CollisionFase.h"
#include "ObjectID.h"

#define ObjectMng ObjectManager::GetInstance()

class Object;

/// @brief オブジェクト管理
class ObjectManager final : public StaticSingleton<ObjectManager>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief オブジェクト追加
	/// @param object オブジェクト
	/// @param key 任意のキー
	void AddObject(shared_ptr<Object> object);

	/// @brief オブジェクト追加(1フレーム遅れ)
	/// @param object オブジェクト
	/// @param key 任意のキー
	void AddObjectLate(shared_ptr<Object> object);

	/// @brief 指定オブジェクト削除
	/// @param key オブジェクトキー
	void DeleteObject(const string& key);

	/// @brief 指定オブジェクト削除(1フレーム遅れ)
	/// @param key オブジェクトキー
	void DeleteObjectLate(const string& key);

	/// @brief 指定のオブジェクト取得
	/// @param key オブジェクトキー
	/// @return オブジェクト
	[[nodiscard]] shared_ptr<Object> GetSpecificObject(const string& key) const;

	/// @brief 指定のオブジェクトIDのオブジェクト配列取得
	/// @param key オブジェクトID
	/// @return オブジェクト配列
	[[nodiscard]] vector<weak_ptr<Object>> GetSpecificObjectArray(OBJECT_ID id) const;

	/// @brief 繰り返し初期化処理
	void InitObjects();

	/// @brief 描画処理
	void DrawObjects();

	/// @brief 基本的な更新処理
	void UpdateObjects();

	/// @brief 通常Update前に走る処理
	void PreUpdateObjects();

	/// @brief 通常Update後に走る処理
	void LateUpdateObjects();

	/// @brief 衝突判定後に走る処理
	void OnCollisionUpdateObjects(COLLISION_FASE fase);

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdateObjects();

	/// @brief 解放処理
	void ReleaseObjects();

private:

	ObjectManager();
	~ObjectManager();
	PROOF_OF_STATIC_SINGLETON(ObjectManager);

	/// @brief 追加するオブジェクトリストからオブジェクトを追加
	void AddObjectFromList();

	/// @brief 削除するオブジェクトリストからオブジェクトを削除
	void DeleteObjectFromList();

	vector<shared_ptr<Object>> objects_;									/// @brief オブジェクト配列

	unordered_map<string, weak_ptr<Object>> objectListKey_;					/// @brief オブジェクトキー管理配列
	unordered_map<OBJECT_ID, vector<weak_ptr<Object>>> objectListID_;		/// @brief オブジェクトIDごとの管理配列

	unordered_map<string, shared_ptr<Object>> addObjectList_;				/// @brief 追加するオブジェクト配列
	vector<string> deleteObjectList_;										/// @brief 削除するオブジェクト配列
};

