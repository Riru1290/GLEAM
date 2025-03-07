#pragma once
#include "ObjectID.h"
#include "Component/Component.h"
#include "Component/Common/Transform.h"
#include "../Collision/CollisionFase.h"

class Component;
class Quaternion;


/// @brief ゲームに登場する全オブジェクト(UIを除く)の既定クラス
class Object : public enable_shared_from_this<Object>
{
public:

	/// @param uniqueKey オブジェクトを識別するための任意のキー
	Object(const string& uniqueKey);
	virtual ~Object();

	/// @brief オブジェクト識別用のキー取得
	/// @return オブジェクト識別用のキー
	[[nodiscard]] const string& GetObjectUniqueKey()const { return objectUniqueKey_; }

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] virtual OBJECT_ID GetObjectID()const = 0;

	/// @brief 繰り返し初期化処理
	void Init();

	/// @brief 描画処理
	void Draw();

	/// @brief 基本的な更新処理
	void Update();

	/// @brief 通常Update前に走る処理
	void PreUpdate();

	/// @brief 通常Update後に走る処理
	void LateUpdate();

	/// @brief 衝突判定後に走る処理
	void OnCollisionUpdate(COLLISION_FASE fase);

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdate();

	/// @brief 自分自身のshared_ptrを返す
	/// @return Objectのshared_ptr
	shared_ptr<Object> GetPtr() { return shared_from_this(); }

	/// @brief コンポーネント追加
	/// @param component コンポーネント
	/// @param overwrite 同一コンポーネントを上書きするか
	void AddComponent(shared_ptr<Component> component, bool overwrite = false);

	/// @brief 指定コンポーネントを取得
	/// @tparam T コンポーネントクラス
	/// @return 指定されたコンポーネント
	template<class T, 
		enable_if_t<
		is_base_of_v<const Component, T>,
		nullptr_t> = nullptr>
	[[nodiscard]] T& GetComponent() const
	{
		// 指定されたコンポーネントが含まれているか確認
		for (auto& [id,registerCom] : components_)
		{
			// クラス名を比較
			if (typeid(*registerCom) != typeid(T))continue;
			 
			return *static_pointer_cast<T>(registerCom);
		}

		// 見つからなかった場合、例外をスロー
		throw runtime_error("指定したコンポーネントが存在しません");
	}

	/// @brief すべてのコンポーネントのアクティブ状態をセット
	/// @param isActive アクティブ状態
	void SetActiveStateToAllComponents(bool isActive);

	/// @brief トランスフォーム取得
	/// @return トランスフォーム
	[[nodiscard]] const Transform& GetTransform()const { return transform_->get(); }

	/// @brief トランスフォーム設定
	/// @param transform トランスフォーム
	void SetTransform(Transform transform);

	/// @brief 座標設定
	/// @param pos 座標
	void SetPosition(Position3D pos);

	/// @brief 回転設定
	/// @param qua 回転
	void SetRotation(Quaternion qua);

	/// @brief 大きさ設定
	/// @param scale 大きさ
	void SetScale(Scale3D scale);

protected:


	/// @brief 繰り返し初期化処理
	virtual void InitObject() {}

	/// @brief 描画処理
	virtual void DrawObject() = 0;

	/// @brief 基本的な更新処理
	virtual void UpdateObject() = 0;

	/// @brief 通常Update前に走る処理
	virtual void PreUpdateObject() {}

	/// @brief 通常Update後に走る処理
	virtual void LateUpdateObject() {}

	/// @brief カメラ更新後に走る処理
	virtual void OnCameraUpdateObject() {}

	/// @brief 衝突判定後に走る処理設定
	virtual void OnCollisionUpdateSetting() {}

	unordered_map<COLLISION_FASE, function<void()>>onCollisionUpdateList_;	/// @brief 衝突判定後に走る処理

	
	multimap<COMPONENT_ID, shared_ptr<Component>> components_;	/// @brief コンポーネント

	// 基本コンポーネント--------------------

	optional<reference_wrapper<Transform>> transform_;		/// @brief トランスフォーム情報

	//---------------------------------------

private:

	string objectUniqueKey_;	/// @brief オブジェクトを識別する唯一のキー

};

