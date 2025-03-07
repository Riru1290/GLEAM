#pragma once
#include "../../Game/Event/EventID.h"
#include "../Object.h"

/// @brief �S�[����j�ޓ싞��
class Padlock final : public Object
{
public:

	[[nodiscard]] Padlock(const string& uniqueKey, EVENT_ID iventID);
	~Padlock();

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::PADLOCK; }

	/// @brief �`�揈��
	void DrawObject() override {}

	/// @brief ��{�I�ȍX�V����
	void UpdateObject() override {}

	/// @brief �R���C�_�[�ݒ�
	void ColliderSetting();

	void OnHitPlayerEye(COL_HIT_DATA data);

	EVENT_ID iventID_;
};

