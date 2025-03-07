#pragma once
#include "../../Game/Event/EventID.h"
#include "../Object.h"

/// @brief ゴールを阻む南京錠
class Padlock final : public Object
{
public:

	[[nodiscard]] Padlock(const string& uniqueKey, EVENT_ID iventID);
	~Padlock();

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::PADLOCK; }

	/// @brief 描画処理
	void DrawObject() override {}

	/// @brief 基本的な更新処理
	void UpdateObject() override {}

	/// @brief コライダー設定
	void ColliderSetting();

	void OnHitPlayerEye(COL_HIT_DATA data);

	EVENT_ID iventID_;
};

