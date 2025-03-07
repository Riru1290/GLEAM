#pragma once
#include "../../Component.h"
#include "State.h"


/// @brief アクターのステート管理
template<class T>
class StateMachine final : public Component
{
public:

	[[nodiscard]] StateMachine() : Component()
	{

		onCollisionUpdateList_[COLLISION_FASE::FIRST] =
			[this] {currentState_->get().OnCollisionUpdate(COLLISION_FASE::FIRST); };
		onCollisionUpdateList_[COLLISION_FASE::SECOND] =
			[this] {currentState_->get().OnCollisionUpdate(COLLISION_FASE::SECOND); };

	}
	~StateMachine() = default;

	void ChangeState(T key)
	{
		if (!stateList_.contains(key))return;

		currentState_ = *stateList_.at(key);

		stateKey_ = key;
	}

	void RegisterState(T key,unique_ptr<State<T>> state)
	{
		if (stateList_.contains(key))return;

		stateList_.emplace(key, move(state));

		currentState_ = *stateList_.at(key);

		stateKey_ = key;
	}

	[[nodiscard]] State<T>& GetState(T key)
	{
		return *stateList_.at(key);
	}

	[[nodiscard]] const T& GetStateKey()
	{
		return stateKey_;
	}

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::STATE_MACHINE; }


	/// @brief 更新処理
	void InitComponent() override
	{
		currentState_->get().Init();
	}

	/// @brief 更新処理
	void UpdateComponent() override
	{
		CheckChangeState();
		currentState_->get().Update();
	}

	/// @brief 通常Update前に走る処理
	void PreUpdateComponent() override
	{
		currentState_->get().PreUpdate();
	}

	/// @brief 通常Update後に走る処理
	void LateUpdateComponent() override
	{
		currentState_->get().LateUpdate();
	}

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdateComponent() override
	{
		currentState_->get().OnCameraUpdate();
	}

	/// @brief 描画処理
	void DrawComponent() override
	{
		currentState_->get().Draw();
	}

	/// @brief 解放処理
	void ReleaseComponent() override
	{
		stateList_.clear();
	}

	void CheckChangeState()
	{
		unordered_map<T, function<bool()>> transitableStates =
			currentState_->get().GetTransitableStates();

		for (const auto& [state, func] : transitableStates)
		{
			if (func())
			{
				ChangeState(state);
				break;
			}
		}

	}

	T stateKey_;
	optional<reference_wrapper<State<T>>> currentState_;	/// @brief 現在の状態
	unordered_map<T, unique_ptr<State<T>>> stateList_;		/// @brief 状態リスト
};
