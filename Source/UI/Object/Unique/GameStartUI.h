#pragma once
#include "../../UIObject.h"
#include "../../../Collision/ColliderInfo.h"
#include "../../../Collision/CollisionHitData.h"
#include "../../../Game/Event/EventID.h"

class Rect2DCollider;
class Fader;

/// @brief ゲームスタートUI
class GameStartUI final : public UIObject
{
public:

	[[nodiscard]] GameStartUI();
	~GameStartUI() = default;

private:

	/// @brief 基本的な更新処理
	void UpdateUI() override;

	/// @brief テクスチャ設定後の初期化
	void InitAfterSetTexture() override;

	void OnHitCursor(COL_HIT_DATA data);

	optional<reference_wrapper<Rect2DCollider>> collider_;
	optional<reference_wrapper<Fader>> fader_;

	bool onHit_;

};
