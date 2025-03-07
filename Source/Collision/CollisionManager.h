#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Object/ObjectID.h"
#include "CollisionHitData.h"
#include "CollisionFase.h"

#define CollisionMng CollisionManager::GetInstance()

class Collider;
class PointCollider;
class Component;


namespace
{
	static constexpr int COLLISION_RECURSION_COUNT_DEFAULT = (1);
}

/// @brief �Փ˔���Ǘ�
class CollisionManager final : public StaticSingleton<CollisionManager>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief �R���C�_�[��o�^
	/// @param collider �R���C�_�[
	/// @param objectUniqueKey �I�u�W�F�N�g���ʃL�[
	/// @param objectID �I�u�W�F�N�gID
	void RegisterCollider(shared_ptr<Collider> collider, const string& objectUniqueKey, OBJECT_ID objectID);

	/// @brief �R���C�_�[�̓o�^����
	/// @param collider �R���C�_�[
	void UnregisterCollider(shared_ptr<Component> collider);

	/// @brief �w��t�F�[�Y�̏Փ˔�����s��
	/// @param fase �Փ˔���t�F�[�Y
	/// @param recursionCount ���x�ċA�I�ɏՓ˔�����s����
	void Collision(COLLISION_FASE currentFase, int recursionCount = COLLISION_RECURSION_COUNT_DEFAULT);

private:

	CollisionManager();
	~CollisionManager();
	PROOF_OF_STATIC_SINGLETON(CollisionManager);

	/// @brief �Փ˔��胊�X�g�ݒ�
	void CollisionListSetting();

	/// @brief �R���C�_�[���m�̏Փ˔���
	/// @param objA �R���C�_�[A
	/// @param objB �R���C�_�[B
	void CollisionPerObject(OBJECT_ID objA, OBJECT_ID objB);

	/// @brief �}�E�X�J�[�\���p�R���C�_�[���X�V
	void UpdateCursorCollider();

	vector<tuple<OBJECT_ID, OBJECT_ID, COLLISION_FASE>> collisionList_;		/// @brief �Փ˃��X�g

	map<OBJECT_ID, vector<weak_ptr<Collider>>> colliders_;					/// @brief �I�u�W�F�N�gID���Ƃ̃R���C�_�[�z��

	shared_ptr<PointCollider> cursorCollider_;								/// @brief �}�E�X�J�[�\���p�R���C�_�[

	UINT collisionCount_;													/// @brief �I�u�W�F�N�g���m���Փ˂�����
};


