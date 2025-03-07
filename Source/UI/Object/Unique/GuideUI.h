#pragma once
#include "../../UIObject.h"
#include "../../../Game/Event/EventID.h"

class Fader;

/// @brief ガイドUI
class GuideUI final : public UIObject
{
public:

	[[nodiscard]] GuideUI(EVENT_ID eventID, EVENT_ID afterEventID);
	~GuideUI() = default;

private:

	/// @brief 基本的な更新処理
	void UpdateUI() override;

	/// @brief テクスチャ設定後の初期化
	void InitAfterSetTexture() override;


	optional<reference_wrapper<Fader>> fader_;


	EVENT_ID eventID_;

};

