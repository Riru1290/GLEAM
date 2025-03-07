#pragma once
#include "../Object.h"

/// @brief ���r�[�E���U���g�p�̕���
class LobbyRoom final : public Object
{
public:

	[[nodiscard]] LobbyRoom(const string& uniqueKey);
	~LobbyRoom() = default;

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::MAP; }

	/// @brief �`�揈��
	void DrawObject() override {}

	/// @brief ��{�I�ȍX�V����
	void UpdateObject() override {}
};

