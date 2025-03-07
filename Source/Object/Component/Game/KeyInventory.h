#pragma once
#include "../Component.h"
#include "../../../Game/Event/EventID.h"


/// @brief �����Ǘ�����
class KeyInventory final : public Component
{
public:

	[[nodiscard]] KeyInventory();
	~KeyInventory() = default;

	/// @brief �����g�p����
	/// @return �g�p�ł������ǂ���
	[[nodiscard]] bool UseKey();

	/// @brief ���O�ɐݒ肳�ꂽ�����擾
	void GetKey();

	/// @brief �w��̌����擾
	/// @param keyName ���̃L�[
	void GetKey(const string& keyName);

	/// @brief �擾���錮�̃I�u�W�F�N�g���ʃL�[��ݒ�
	/// @param uniqueKey �擾���錮�̃I�u�W�F�N�g���ʃL�[
	void SetGetKeyObjectUniqueKey(const string& uniqueKey) { getKeyObjectUniqueKey_ = uniqueKey; }

	/// @brief ���ݎ擾���Ă��錮�̐����擾
	/// @return ���ݎ擾���Ă��錮�̐�
	[[nodiscard]] const int& GetKeyCount()const { return getKeyCount_; }


private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	virtual COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::KEY_INVENTORY; }

	/// @brief �X�V����
	void UpdateComponent() override {}

	/// @brief �������
	void ReleaseComponent() override {}


	int getKeyCount_;				/// @brief ���擾��

	string getKeyObjectUniqueKey_;	/// @brief �擾���錮�̃I�u�W�F�N�g���ʃL�[

	set<EVENT_ID> useIventID_;
	//vector<string> getKeyName_;
};

