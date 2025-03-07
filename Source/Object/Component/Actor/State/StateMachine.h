#pragma once
#include "../../Component.h"
#include "State.h"


/// @brief �A�N�^�[�̃X�e�[�g�Ǘ�
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

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::STATE_MACHINE; }


	/// @brief �X�V����
	void InitComponent() override
	{
		currentState_->get().Init();
	}

	/// @brief �X�V����
	void UpdateComponent() override
	{
		CheckChangeState();
		currentState_->get().Update();
	}

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateComponent() override
	{
		currentState_->get().PreUpdate();
	}

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdateComponent() override
	{
		currentState_->get().LateUpdate();
	}

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdateComponent() override
	{
		currentState_->get().OnCameraUpdate();
	}

	/// @brief �`�揈��
	void DrawComponent() override
	{
		currentState_->get().Draw();
	}

	/// @brief �������
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
	optional<reference_wrapper<State<T>>> currentState_;	/// @brief ���݂̏��
	unordered_map<T, unique_ptr<State<T>>> stateList_;		/// @brief ��ԃ��X�g
};
