#pragma once
#include "../../Game/Event/EventID.h"
#include "../Object.h"

struct COL_HIT_DATA;

/// @brief �S�[���ɕK�v�Ȍ�
class Key final : public Object
{
public:

	[[nodiscard]] Key(const string& uniqueKey, EVENT_ID getID, EVENT_ID useID);
	~Key();

	[[nodiscard]] const EVENT_ID& GetUseIventID()const { return useIventID_; }
	[[nodiscard]] const EVENT_ID& GetGetIventID()const { return getIventID_; }

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::KEY; }

	/// @brief �`�揈��
	void DrawObject() override {}

	/// @brief ��{�I�ȍX�V����
	void UpdateObject() override;

	/// @brief �R���C�_�[�ݒ�
	void ColliderSetting();

	void OnHitPlayerEye(COL_HIT_DATA data);

	EVENT_ID getIventID_;
	EVENT_ID useIventID_;
};

