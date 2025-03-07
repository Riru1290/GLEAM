#pragma once
#include "../../Component.h"
#include "../../../../Collision/CollisionFase.h"

template<typename T>
class State
{
public:

	State(unordered_map<T, function<bool()>> transitableStates) :
		transitableStates_(transitableStates)
	{

	}
	virtual ~State() = default;

	/// @brief コンポーネント追加
	/// @param component コンポーネント
	/// @param overwrite 同一コンポーネントを上書きするか
	void AddComponent(shared_ptr<Component> component, bool overwrite = false)
	{
		// 追加予定のものがすでに含まれているかどうか確認
		if (components_.contains(component->GetComponentID()))
		{
			// 上書きするか
			if (overwrite)
			{
				// 上書き
				components_.insert_or_assign(
					component->GetComponentID(), move(component));

				return;
			}

			return;
		}

		// コンポーネント追加
		components_.emplace(
			component->GetComponentID(), move(component));
	}
	
	[[nodiscard]] bool IsPossibleChangeState(T state)
	{
		if (!transitableStates_.contains(state))return false;

		return transitableStates_.at(state)();
	}

	/// @brief 繰り返し初期化処理
	void Init()
	{
		for (const auto& [id, component] : components_)
		{
			component->Init();
		}
	}

	/// @brief 描画処理
	void Draw()
	{
		for (const auto& [id, component] : components_)
		{
			component->Draw();
		}
	}

	/// @brief 基本的な更新処理
	void Update()
	{
		for (const auto& [id, component] : components_)
		{
			component->Update();
		}
	}

	/// @brief 通常Update前に走る処理
	void PreUpdate()
	{
		for (const auto& [id, component] : components_)
		{
			component->PreUpdate();
		}
	}

	/// @brief 通常Update後に走る処理
	void LateUpdate()
	{
		for (const auto& [id, component] : components_)
		{
			component->LateUpdate();
		}
	}

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdate()
	{
		for (const auto& [id, component] : components_)
		{
			component->OnCameraUpdate();
		}
	}


	/// @brief 衝突判定後に走る処理
	void OnCollisionUpdate(COLLISION_FASE fase)
	{
		if (onCollisionUpdateList_.contains(fase))
		{
			onCollisionUpdateList_.at(fase)();
		}

		for (const auto& [id, component] : components_)
		{
			component->OnCollisionUpdate(fase);
		}
	}

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
		for (auto& [id, registerCom] : components_)
		{
			// クラス名を比較
			if (typeid(*registerCom) != typeid(T))continue;

			return *static_pointer_cast<T>(registerCom);
		}

		// 見つからなかった場合、例外をスロー
		throw runtime_error("指定したコンポーネントが存在しません");
	}

	[[nodiscard]] const unordered_map<T, function<bool()>>& GetTransitableStates()const { return transitableStates_; }

	unordered_map<COLLISION_FASE, function<void()>>onCollisionUpdateList_;	/// @brief 衝突判定後に走る処理

private:

	map<COMPONENT_ID, shared_ptr<Component>> components_;	/// @brief コンポーネント

	unordered_map<T, function<bool()>> transitableStates_;		/// @brief 遷移可能な状態とその遷移条件

};

