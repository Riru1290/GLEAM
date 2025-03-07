#pragma once
#include "../../../Resource/ResourceID.h"
#include "../Actor.h"

/// @brief プレイヤー(表示用)
class VisualPlayer final : public Actor
{
public:

	[[nodiscard]] VisualPlayer(const string& uniqueKey,RES_ID animationID);
	~VisualPlayer();
private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::PLAYER; }

	/// @brief 基本的な更新処理
	void UpdateActor() override;

	/// @brief アニメーション設定
	void AnimationSetting() override;

	RES_ID animationID_;
};

