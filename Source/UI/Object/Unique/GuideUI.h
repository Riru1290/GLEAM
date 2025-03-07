#pragma once
#include "../../UIObject.h"
#include "../../../Game/Event/EventID.h"

class Fader;

/// @brief �K�C�hUI
class GuideUI final : public UIObject
{
public:

	[[nodiscard]] GuideUI(EVENT_ID eventID, EVENT_ID afterEventID);
	~GuideUI() = default;

private:

	/// @brief ��{�I�ȍX�V����
	void UpdateUI() override;

	/// @brief �e�N�X�`���ݒ��̏�����
	void InitAfterSetTexture() override;


	optional<reference_wrapper<Fader>> fader_;


	EVENT_ID eventID_;

};

