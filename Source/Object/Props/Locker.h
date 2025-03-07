#pragma once
#include "../Object.h"
#include "../../Resource/Model/Model.h"


/// @brief プレイヤーが隠れるロッカー
class Locker final : public Object
{
public:

	[[nodiscard]] Locker(const string& uniqueKey);
	~Locker() = default;

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::LOCKER; }

	/// @brief 初期化処理
	void InitObject() override;

	/// @brief 描画処理
	void DrawObject() override;

	/// @brief 基本的な更新処理
	void UpdateObject() override;

	/// @brief コライダー設定
	void ColliderSetting();

	optional<reference_wrapper<const Model>> model_;
};

