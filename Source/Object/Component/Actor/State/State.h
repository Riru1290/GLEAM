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

	/// @brief �R���|�[�l���g�ǉ�
	/// @param component �R���|�[�l���g
	/// @param overwrite ����R���|�[�l���g���㏑�����邩
	void AddComponent(shared_ptr<Component> component, bool overwrite = false)
	{
		// �ǉ��\��̂��̂����łɊ܂܂�Ă��邩�ǂ����m�F
		if (components_.contains(component->GetComponentID()))
		{
			// �㏑�����邩
			if (overwrite)
			{
				// �㏑��
				components_.insert_or_assign(
					component->GetComponentID(), move(component));

				return;
			}

			return;
		}

		// �R���|�[�l���g�ǉ�
		components_.emplace(
			component->GetComponentID(), move(component));
	}
	
	[[nodiscard]] bool IsPossibleChangeState(T state)
	{
		if (!transitableStates_.contains(state))return false;

		return transitableStates_.at(state)();
	}

	/// @brief �J��Ԃ�����������
	void Init()
	{
		for (const auto& [id, component] : components_)
		{
			component->Init();
		}
	}

	/// @brief �`�揈��
	void Draw()
	{
		for (const auto& [id, component] : components_)
		{
			component->Draw();
		}
	}

	/// @brief ��{�I�ȍX�V����
	void Update()
	{
		for (const auto& [id, component] : components_)
		{
			component->Update();
		}
	}

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdate()
	{
		for (const auto& [id, component] : components_)
		{
			component->PreUpdate();
		}
	}

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdate()
	{
		for (const auto& [id, component] : components_)
		{
			component->LateUpdate();
		}
	}

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdate()
	{
		for (const auto& [id, component] : components_)
		{
			component->OnCameraUpdate();
		}
	}


	/// @brief �Փ˔����ɑ��鏈��
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

	/// @brief �w��R���|�[�l���g���擾
	/// @tparam T �R���|�[�l���g�N���X
	/// @return �w�肳�ꂽ�R���|�[�l���g
	template<class T,
		enable_if_t<
		is_base_of_v<const Component, T>,
		nullptr_t> = nullptr>
	[[nodiscard]] T& GetComponent() const
	{
		// �w�肳�ꂽ�R���|�[�l���g���܂܂�Ă��邩�m�F
		for (auto& [id, registerCom] : components_)
		{
			// �N���X�����r
			if (typeid(*registerCom) != typeid(T))continue;

			return *static_pointer_cast<T>(registerCom);
		}

		// ������Ȃ������ꍇ�A��O���X���[
		throw runtime_error("�w�肵���R���|�[�l���g�����݂��܂���");
	}

	[[nodiscard]] const unordered_map<T, function<bool()>>& GetTransitableStates()const { return transitableStates_; }

	unordered_map<COLLISION_FASE, function<void()>>onCollisionUpdateList_;	/// @brief �Փ˔����ɑ��鏈��

private:

	map<COMPONENT_ID, shared_ptr<Component>> components_;	/// @brief �R���|�[�l���g

	unordered_map<T, function<bool()>> transitableStates_;		/// @brief �J�ډ\�ȏ�ԂƂ��̑J�ڏ���

};

