#pragma once
#include "../../../Resource/ResourceID.h"
#include "../Actor.h"

/// @brief �v���C���[(�\���p)
class VisualPlayer final : public Actor
{
public:

	[[nodiscard]] VisualPlayer(const string& uniqueKey,RES_ID animationID);
	~VisualPlayer();
private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::PLAYER; }

	/// @brief ��{�I�ȍX�V����
	void UpdateActor() override;

	/// @brief �A�j���[�V�����ݒ�
	void AnimationSetting() override;

	RES_ID animationID_;
};

