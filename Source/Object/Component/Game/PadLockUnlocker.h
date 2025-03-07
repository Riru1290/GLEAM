#pragma once
#include "../Component.h"

/// @brief �싞������������
class PadLockUnlocker final : public Component
{
public:

	[[nodiscard]] PadLockUnlocker();
	~PadLockUnlocker() = default;

	/// @brief ���O�ɐݒ肳�ꂽ�싞��������
	void UnLock() const;

	/// @brief ��������싞���̃I�u�W�F�N�g���ʃL�[��ݒ�
	/// @param uniqueKey ��������싞���̃I�u�W�F�N�g���ʃL�[
	void SetGetKeyObjectUniqueKey(const string& uniqueKey) { padLockObjectUniqueKey_ = uniqueKey; }

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	virtual COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::PADLOCK_UNLOCKER; }

	/// @brief �X�V����
	void UpdateComponent() override {}

	/// @brief �������
	void ReleaseComponent() override {}


	string padLockObjectUniqueKey_;	/// @brief ��������싞���̃I�u�W�F�N�g���ʃL�[

};

