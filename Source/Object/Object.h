#pragma once
#include "ObjectID.h"
#include "Component/Component.h"
#include "Component/Common/Transform.h"
#include "../Collision/CollisionFase.h"

class Component;
class Quaternion;


/// @brief �Q�[���ɓo�ꂷ��S�I�u�W�F�N�g(UI������)�̊���N���X
class Object : public enable_shared_from_this<Object>
{
public:

	/// @param uniqueKey �I�u�W�F�N�g�����ʂ��邽�߂̔C�ӂ̃L�[
	Object(const string& uniqueKey);
	virtual ~Object();

	/// @brief �I�u�W�F�N�g���ʗp�̃L�[�擾
	/// @return �I�u�W�F�N�g���ʗp�̃L�[
	[[nodiscard]] const string& GetObjectUniqueKey()const { return objectUniqueKey_; }

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] virtual OBJECT_ID GetObjectID()const = 0;

	/// @brief �J��Ԃ�����������
	void Init();

	/// @brief �`�揈��
	void Draw();

	/// @brief ��{�I�ȍX�V����
	void Update();

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdate();

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdate();

	/// @brief �Փ˔����ɑ��鏈��
	void OnCollisionUpdate(COLLISION_FASE fase);

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdate();

	/// @brief �������g��shared_ptr��Ԃ�
	/// @return Object��shared_ptr
	shared_ptr<Object> GetPtr() { return shared_from_this(); }

	/// @brief �R���|�[�l���g�ǉ�
	/// @param component �R���|�[�l���g
	/// @param overwrite ����R���|�[�l���g���㏑�����邩
	void AddComponent(shared_ptr<Component> component, bool overwrite = false);

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
		for (auto& [id,registerCom] : components_)
		{
			// �N���X�����r
			if (typeid(*registerCom) != typeid(T))continue;
			 
			return *static_pointer_cast<T>(registerCom);
		}

		// ������Ȃ������ꍇ�A��O���X���[
		throw runtime_error("�w�肵���R���|�[�l���g�����݂��܂���");
	}

	/// @brief ���ׂẴR���|�[�l���g�̃A�N�e�B�u��Ԃ��Z�b�g
	/// @param isActive �A�N�e�B�u���
	void SetActiveStateToAllComponents(bool isActive);

	/// @brief �g�����X�t�H�[���擾
	/// @return �g�����X�t�H�[��
	[[nodiscard]] const Transform& GetTransform()const { return transform_->get(); }

	/// @brief �g�����X�t�H�[���ݒ�
	/// @param transform �g�����X�t�H�[��
	void SetTransform(Transform transform);

	/// @brief ���W�ݒ�
	/// @param pos ���W
	void SetPosition(Position3D pos);

	/// @brief ��]�ݒ�
	/// @param qua ��]
	void SetRotation(Quaternion qua);

	/// @brief �傫���ݒ�
	/// @param scale �傫��
	void SetScale(Scale3D scale);

protected:


	/// @brief �J��Ԃ�����������
	virtual void InitObject() {}

	/// @brief �`�揈��
	virtual void DrawObject() = 0;

	/// @brief ��{�I�ȍX�V����
	virtual void UpdateObject() = 0;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	virtual void PreUpdateObject() {}

	/// @brief �ʏ�Update��ɑ��鏈��
	virtual void LateUpdateObject() {}

	/// @brief �J�����X�V��ɑ��鏈��
	virtual void OnCameraUpdateObject() {}

	/// @brief �Փ˔����ɑ��鏈���ݒ�
	virtual void OnCollisionUpdateSetting() {}

	unordered_map<COLLISION_FASE, function<void()>>onCollisionUpdateList_;	/// @brief �Փ˔����ɑ��鏈��

	
	multimap<COMPONENT_ID, shared_ptr<Component>> components_;	/// @brief �R���|�[�l���g

	// ��{�R���|�[�l���g--------------------

	optional<reference_wrapper<Transform>> transform_;		/// @brief �g�����X�t�H�[�����

	//---------------------------------------

private:

	string objectUniqueKey_;	/// @brief �I�u�W�F�N�g�����ʂ���B��̃L�[

};

