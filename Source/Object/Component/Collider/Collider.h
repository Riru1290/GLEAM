#pragma once
#include "../Component.h"
#include "../../../Object/ObjectID.h"
#include "../../../Collision/ColliderInfo.h"
#include "../../../Collision/CollisionHitData.h"

class Transform;

/// @brief �Փ˔���Ɏg�p����`��Ȃǂ�ێ�
class Collider : public Component
{
public:

	Collider();
	virtual ~Collider() = default;

	/// @brief �R���C�_�[�̌`���擾
	/// @return �R���C�_�[�̌`
	[[nodiscard]] virtual COLLISION_SHAPE_TYPE GetShapeType()const  = 0;

	/// @brief ���W�b�g�{�f�B��
	/// @return ���W�b�g�{�f�B��
	[[nodiscard]] const bool& IsRigidBody()const { return isRigidBody_; }

	/// @brief �s���̃I�u�W�F�N�g��
	/// @return �s���̃I�u�W�F�N�g��
	[[nodiscard]] const bool& IsStatic()const { return isStatic_; }

	/// @brief �d�ʎ擾
	/// @return �d��
	[[nodiscard]] const float& GetWeight()const { return weight_; }

	/// @brief �Փˌ㏈��
	/// @param objectID �I�u�W�F�N�gID
	/// @param hitData �Փˎ��f�[�^
	void OnHit(OBJECT_ID objectID, COL_HIT_DATA hitData);

	/// @brief �Փˌ㏈���֐��ǉ�
	/// @param objectID �ΏۃI�u�W�F�N�g
	/// @param hitData �Փˌ㏈��
	void AddOnHitFunction(OBJECT_ID objectID, function<void(COL_HIT_DATA)> onHit);

	/// @brief �Ǐ]�ΏۃZ�b�g
	/// @param transform �Ǐ]�Ώ�
	void SetFollower(Transform& transform);

	/// @brief �Ǐ]�ΏۃZ�b�g
	/// @param transform �Ǐ]�Ώ�
	void SetSubFollower(Transform& transform);

	/// @brief ���΍��W�Z�b�g
	/// @param relativePos ���΍��W
	void SetRelativePosition(Position3D relativePos) { relativePos_ = relativePos; }

	/// @brief ����������ԃZ�b�g
	/// @param isRigid �����������
	void SetRigidState(bool isRigid) { isRigidBody_ = isRigid; }

	/// @brief �s����ԃZ�b�g
	/// @param isStatic �s�����
	void SetStaticState(bool isStatic) { isStatic_ = isStatic; }

	/// @brief �d���Z�b�g
	/// @param weight �d��
	void SetWeight(float weight) { weight_ = weight; }

	/// @brief �R���C�_�[�̍X�V��Ǐ]�ΏۂɓK�p
	virtual void ApplyColliderUpdateToFollower() = 0;

	/// @brief �I�u�W�F�N�g���ʃL�[�Z�b�g
	/// @param uniqueKey �I�u�W�F�N�g���ʃL�[
	void SetObjectUniqueKey(const string& uniqueKey) { objectUniqueKey_ = uniqueKey; }

	/// @brief �I�u�W�F�N�gID�Z�b�g
	/// @param objID �I�u�W�F�N�gID
	void SetObjectID(OBJECT_ID objID) { objectID_ = objID; }

	/// @brief �e�I�u�W�F�N�g���ʃL�[�擾
	/// @return �e�I�u�W�F�N�g���ʃL�[
	[[nodiscard]] const string& GetObjectUniqueKey()const { return objectUniqueKey_; }

	/// @brief �e�I�u�W�F�N�gID�擾
	/// @return �e�I�u�W�F�N�gID
	[[nodiscard]] const OBJECT_ID& GetObjectID()const { return objectID_; }

	/// @brief �Ǐ]�Ώۃg�����X�t�H�[���擾
	/// @return �Ǐ]�Ώۃg�����X�t�H�[��
	[[nodiscard]] const Transform& GetFollowerTransform()const { return follower_->get(); }

protected:

	/// @brief �X�V����
	virtual void UpdateCollider() = 0;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	virtual void PreUpdateCollider() = 0;

	/// @brief �`�揈��
	virtual void DrawCollider() {}

	optional<reference_wrapper<Transform>> follower_;		/// @brief �Ǐ]�Ώ�
	Position3D relativePos_;								/// @brief ���΍��W
	optional<reference_wrapper<Transform>> subFollower_;	/// @brief �Ǐ]�Ώ�(�T�u)

	unordered_map<OBJECT_ID, vector<function<void(COL_HIT_DATA)>>> onHitList_;	/// @brief �Փˌ㏈�����X�g

	bool isRigidBody_;	/// @brief �������������I�u�W�F�N�g�ł��邩
	bool isStatic_;		/// @brief �s���̃I�u�W�F�N�g�ł��邩
	float weight_;		/// @brief �d��(�d�͂Ɋւ��)

private:

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateComponent() override;

	/// @brief �`�揈��
	void DrawComponent() override;

	/// @brief �������
	void ReleaseComponent() override;

	OBJECT_ID objectID_;
	string objectUniqueKey_;
};

