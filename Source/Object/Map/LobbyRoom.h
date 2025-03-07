#pragma once
#include "../Object.h"

/// @brief ロビー・リザルト用の部屋
class LobbyRoom final : public Object
{
public:

	[[nodiscard]] LobbyRoom(const string& uniqueKey);
	~LobbyRoom() = default;

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::MAP; }

	/// @brief 描画処理
	void DrawObject() override {}

	/// @brief 基本的な更新処理
	void UpdateObject() override {}
};

