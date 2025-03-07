#pragma once
#include "../../UIObject.h"
#include "../../../Collision/ColliderInfo.h"
#include "../../../Collision/CollisionHitData.h"
#include "../../../Game/Event/EventID.h"

class Rect2DCollider;
class Fader;

/// @brief �{�^��UI
class ButtonUI final : public UIObject
{
public:

	[[nodiscard]] ButtonUI(EVENT_ID eventID, int defNo, int pushNo);
	~ButtonUI() = default;

	void PressButton();
	void ReleaseButton();

private:

	/// @brief ��{�I�ȍX�V����
	void UpdateUI() override;

	/// @brief �e�N�X�`���ݒ��̏�����
	void InitAfterSetTexture() override;

	void OnHitCursor(COL_HIT_DATA data);

	optional<reference_wrapper<Rect2DCollider>> collider_;
	optional<reference_wrapper<Fader>> fader_;

	bool onClick_;

	bool preOnHit_;
	bool onHit_;

	int defNo_;
	int pushNo_;

	EVENT_ID eventID_;

};

