#pragma once
#include "../../UIObject.h"
#include "../../../Collision/ColliderInfo.h"
#include "../../../Collision/CollisionHitData.h"
#include "../../../Game/Event/EventID.h"

class Rect2DCollider;
class Fader;

/// @brief �Q�[���X�^�[�gUI
class GameStartUI final : public UIObject
{
public:

	[[nodiscard]] GameStartUI();
	~GameStartUI() = default;

private:

	/// @brief ��{�I�ȍX�V����
	void UpdateUI() override;

	/// @brief �e�N�X�`���ݒ��̏�����
	void InitAfterSetTexture() override;

	void OnHitCursor(COL_HIT_DATA data);

	optional<reference_wrapper<Rect2DCollider>> collider_;
	optional<reference_wrapper<Fader>> fader_;

	bool onHit_;

};
