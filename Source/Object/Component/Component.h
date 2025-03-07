#pragma once
#include "ComponentID.h"
#include "../../Collision/CollisionFase.h"

/// @brief �R���|�[�l���g�C���^�[�t�F�[�X
class Component : public enable_shared_from_this<Component>
{
public:

	[[nodiscard]] Component();
	virtual ~Component() = default;

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	virtual COMPONENT_ID GetComponentID()const = 0;

	/// @brief �J��Ԃ�����������
	void Init();

	/// @brief �X�V����
	void Update();

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdate();

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdate();

	/// @brief �Փ˔����ɑ��鏈��
	void OnCollisionUpdate(COLLISION_FASE fase);

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdate();

	/// @brief �`�揈��
	void Draw();

	/// @brief �������
	void Release();

	/// @brief �L�����ǂ�����Ԃ�
	/// @return �L�����ǂ���
	[[nodiscard]] const bool& IsActive()const { return isActive_; }

	/// @brief �L�����ǂ������Z�b�g
	/// @param isActive �L�����ǂ���
	void SetActive(bool isActive) { isActive_ = isActive; }
 
	/// @brief �������g��shared_ptr��Ԃ�
	/// @return Component��shared_ptr
	shared_ptr<Component> GetPtr() { return shared_from_this(); }

protected:

	bool isActive_;		/// @brief �L�����ǂ���

	/// @brief �J��Ԃ�����������
	virtual void InitComponent() {}

	/// @brief �X�V����
	virtual void UpdateComponent() = 0;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	virtual void PreUpdateComponent() {}

	/// @brief �ʏ�Update��ɑ��鏈��
	virtual void LateUpdateComponent() {}

	/// @brief �J�����X�V��ɑ��鏈��
	virtual void OnCameraUpdateComponent() {}

	/// @brief �`�揈��
	virtual void DrawComponent() {}

	/// @brief �������
	virtual void ReleaseComponent() = 0;

	unordered_map<COLLISION_FASE, function<void()>>onCollisionUpdateList_;	/// @brief �Փ˔����ɑ��鏈��
};

