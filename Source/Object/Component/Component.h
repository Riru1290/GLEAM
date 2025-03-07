#pragma once
#include "ComponentID.h"
#include "../../Collision/CollisionFase.h"

/// @brief コンポーネントインターフェース
class Component : public enable_shared_from_this<Component>
{
public:

	[[nodiscard]] Component();
	virtual ~Component() = default;

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	virtual COMPONENT_ID GetComponentID()const = 0;

	/// @brief 繰り返し初期化処理
	void Init();

	/// @brief 更新処理
	void Update();

	/// @brief 通常Update前に走る処理
	void PreUpdate();

	/// @brief 通常Update後に走る処理
	void LateUpdate();

	/// @brief 衝突判定後に走る処理
	void OnCollisionUpdate(COLLISION_FASE fase);

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdate();

	/// @brief 描画処理
	void Draw();

	/// @brief 解放処理
	void Release();

	/// @brief 有効かどうかを返す
	/// @return 有効かどうか
	[[nodiscard]] const bool& IsActive()const { return isActive_; }

	/// @brief 有効かどうかをセット
	/// @param isActive 有効かどうか
	void SetActive(bool isActive) { isActive_ = isActive; }
 
	/// @brief 自分自身のshared_ptrを返す
	/// @return Componentのshared_ptr
	shared_ptr<Component> GetPtr() { return shared_from_this(); }

protected:

	bool isActive_;		/// @brief 有効かどうか

	/// @brief 繰り返し初期化処理
	virtual void InitComponent() {}

	/// @brief 更新処理
	virtual void UpdateComponent() = 0;

	/// @brief 通常Update前に走る処理
	virtual void PreUpdateComponent() {}

	/// @brief 通常Update後に走る処理
	virtual void LateUpdateComponent() {}

	/// @brief カメラ更新後に走る処理
	virtual void OnCameraUpdateComponent() {}

	/// @brief 描画処理
	virtual void DrawComponent() {}

	/// @brief 解放処理
	virtual void ReleaseComponent() = 0;

	unordered_map<COLLISION_FASE, function<void()>>onCollisionUpdateList_;	/// @brief 衝突判定後に走る処理
};

