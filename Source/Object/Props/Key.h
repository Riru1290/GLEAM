#pragma once
#include "../../Game/Event/EventID.h"
#include "../Object.h"

struct COL_HIT_DATA;

/// @brief ゴールに必要な鍵
class Key final : public Object
{
public:

	[[nodiscard]] Key(const string& uniqueKey, EVENT_ID getID, EVENT_ID useID);
	~Key();

	[[nodiscard]] const EVENT_ID& GetUseIventID()const { return useIventID_; }
	[[nodiscard]] const EVENT_ID& GetGetIventID()const { return getIventID_; }

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::KEY; }

	/// @brief 描画処理
	void DrawObject() override {}

	/// @brief 基本的な更新処理
	void UpdateObject() override;

	/// @brief コライダー設定
	void ColliderSetting();

	void OnHitPlayerEye(COL_HIT_DATA data);

	EVENT_ID getIventID_;
	EVENT_ID useIventID_;
};

