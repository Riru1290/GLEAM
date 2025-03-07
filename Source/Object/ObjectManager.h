#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Collision/CollisionFase.h"
#include "ObjectID.h"

#define ObjectMng ObjectManager::GetInstance()

class Object;

/// @brief �I�u�W�F�N�g�Ǘ�
class ObjectManager final : public StaticSingleton<ObjectManager>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief �I�u�W�F�N�g�ǉ�
	/// @param object �I�u�W�F�N�g
	/// @param key �C�ӂ̃L�[
	void AddObject(shared_ptr<Object> object);

	/// @brief �I�u�W�F�N�g�ǉ�(1�t���[���x��)
	/// @param object �I�u�W�F�N�g
	/// @param key �C�ӂ̃L�[
	void AddObjectLate(shared_ptr<Object> object);

	/// @brief �w��I�u�W�F�N�g�폜
	/// @param key �I�u�W�F�N�g�L�[
	void DeleteObject(const string& key);

	/// @brief �w��I�u�W�F�N�g�폜(1�t���[���x��)
	/// @param key �I�u�W�F�N�g�L�[
	void DeleteObjectLate(const string& key);

	/// @brief �w��̃I�u�W�F�N�g�擾
	/// @param key �I�u�W�F�N�g�L�[
	/// @return �I�u�W�F�N�g
	[[nodiscard]] shared_ptr<Object> GetSpecificObject(const string& key) const;

	/// @brief �w��̃I�u�W�F�N�gID�̃I�u�W�F�N�g�z��擾
	/// @param key �I�u�W�F�N�gID
	/// @return �I�u�W�F�N�g�z��
	[[nodiscard]] vector<weak_ptr<Object>> GetSpecificObjectArray(OBJECT_ID id) const;

	/// @brief �J��Ԃ�����������
	void InitObjects();

	/// @brief �`�揈��
	void DrawObjects();

	/// @brief ��{�I�ȍX�V����
	void UpdateObjects();

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateObjects();

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdateObjects();

	/// @brief �Փ˔����ɑ��鏈��
	void OnCollisionUpdateObjects(COLLISION_FASE fase);

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdateObjects();

	/// @brief �������
	void ReleaseObjects();

private:

	ObjectManager();
	~ObjectManager();
	PROOF_OF_STATIC_SINGLETON(ObjectManager);

	/// @brief �ǉ�����I�u�W�F�N�g���X�g����I�u�W�F�N�g��ǉ�
	void AddObjectFromList();

	/// @brief �폜����I�u�W�F�N�g���X�g����I�u�W�F�N�g���폜
	void DeleteObjectFromList();

	vector<shared_ptr<Object>> objects_;									/// @brief �I�u�W�F�N�g�z��

	unordered_map<string, weak_ptr<Object>> objectListKey_;					/// @brief �I�u�W�F�N�g�L�[�Ǘ��z��
	unordered_map<OBJECT_ID, vector<weak_ptr<Object>>> objectListID_;		/// @brief �I�u�W�F�N�gID���Ƃ̊Ǘ��z��

	unordered_map<string, shared_ptr<Object>> addObjectList_;				/// @brief �ǉ�����I�u�W�F�N�g�z��
	vector<string> deleteObjectList_;										/// @brief �폜����I�u�W�F�N�g�z��
};

