#pragma once
#include "../../UIObject.h"
#include "../../../Collision/ColliderInfo.h"
#include "../../../Collision/CollisionHitData.h"
#include "../../../Game/Event/EventID.h"

class Rect2DCollider;
class Fader;


class LevelButtonUI final : public UIObject
{
public:

	[[nodiscard]] LevelButtonUI(int level,int defNo, int pushNo);
	~LevelButtonUI() = default;

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

	int level_;
};

