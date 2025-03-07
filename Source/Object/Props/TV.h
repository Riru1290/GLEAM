#pragma once
#include "../Object.h"

class Material;

/// @brief TVオブジェクト
class TV final : public Object
{
public:

	[[nodiscard]] TV(const string& uniqueKey, RES_ID screenTexture, int xNum, int yNum);
	~TV();

	void ChangeTVChannel(int no);



private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::TV; }

	/// @brief 描画処理
	void DrawObject() override {}

	/// @brief 基本的な更新処理
	void UpdateObject() override;

	/// @brief 定数バッファ作成
	void CreateConstantBuffer();

	struct TV_BUFFER;

	optional<reference_wrapper<Material>> tvScreen_;

	int tvBufferNo_;			/// @brief TVScreen用定数バッファ管理番号
	TV_BUFFER* tvMap_;			/// @brief TVScreen用定数更新用変数
};

struct TV::TV_BUFFER
{
	float channel;
	float preChannel;
	float currentTime;
	float changeTime;

	float screenDivideXNum;
	float screenDivideYNum;
	float padding;
	float padding2;
};
