#pragma once
#include "../Object.h"

class SmoothRotateController;
class BoxCollider;

/// @brief �S�[����j�ރh�A
class Door final : public Object
{
public:

	[[nodiscard]] Door(const string& uniqueKey, int padLockNum);
	~Door() = default;

	/// @brief �h�A�Ɍ���������
	/// @param padLockNum �싞���̐�
	void LockDoor(int padLockNum) { padLockNum_ = padLockNum; }

	/// @brief �싞������������
	void UnlockPadlockOnDoor();

	/// @brief �h�A���J����
	void OpenDoor();

	/// @brief �h�A��߂�
	void CloseDoor();

private:

	enum class STATE
	{
		OPEN,				/// @brief �J���Ă���
		CLOSE,				/// @brief �܂��Ă���
		ROTATE_TO_OPEN,		/// @brief ��]��
		ROTATE_TO_CLOSE,	/// @brief ��]��
	};

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::DOOR; }

	/// @brief ����������
	void InitObject() override;

	/// @brief �`�揈��
	void DrawObject() override {}

	/// @brief ��{�I�ȍX�V����
	void UpdateObject() override;

	/// @brief �R���C�_�[�ݒ�
	void ColliderSetting();

	optional<reference_wrapper<SmoothRotateController>> rotateCon_;
	optional<reference_wrapper<BoxCollider>> boxCollider_;

	int padLockNum_;	/// @brief �싞���̐�

	STATE state_;
};

