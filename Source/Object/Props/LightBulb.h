#pragma once
#include "../../Graphics/Light/LightController.h"
#include "../Object.h"

struct COL_HIT_DATA;

class DistanceChecker;

/// @brief 電球オブジェクト
class LightBulb final : public Object
{
public:

	[[nodiscard]] LightBulb(const string& uniqueKey,const string& lightKey);
	[[nodiscard]] LightBulb(const string& uniqueKey);
	~LightBulb();

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::LIGHTBULB; }

	/// @brief 繰り返し初期化処理
	void InitObject();

	/// @brief 描画処理
	void DrawObject() override {}

	/// @brief 基本的な更新処理
	void UpdateObject() override {}

	/// @brief 通常Update後に走る処理
	void LateUpdateObject() override;

	/// @brief コライダー設定
	void ColliderSetting();

	void OnHitPlayerEye(COL_HIT_DATA data);

	string lightKey_;

	POINT_LIGHT lightInfo_;

	optional<reference_wrapper<DistanceChecker>> distanceChecker_;			/// @brief 距離測定

	bool isCreateLight_;
};

