#pragma once
#include "../../Object.h"

class Mesh;

/// @brief 直進波
struct STRAIGHT_WAVE
{
	Vector2 dir = {1.0f,0.0f};						/// @brief 周波数（波の間隔を調整）
	float frequency = 5.0f;							/// @brief 減衰スケール（影響範囲を小さくする）
	float speed = 0.0f;								/// @brief 波のスピード
	float duration = 0.0f;							/// @brief 継続させる時間
	float currentTime = 0.0f;						/// @brief 経過時間

	bool isLoop = false;							/// @brief 無限に発生させ続けるか
};

/// @brief 環状波
struct CYCLIC_WAVE
{
	Position3D occurrencePos = { 0.0f,0.0f,0.0f };	/// @brief 発生場所
	float frequency = 0.0f;							/// @brief 周波数（波の間隔を調整）
	float attenuationScale = 5.0;					/// @brief 減衰スケール（影響範囲を小さくする）
	float duration = 0.0f;							/// @brief 継続させる時間
	float currentTime = 0.0f;						/// @brief 経過時間

	bool isLoop = false;							/// @brief 無限に発生させ続けるか
};

/// @brief 波
class Wave final : public Object
{
public:

	[[nodiscard]] Wave(const string& uniqueKey, const vector<pair<Position3D, Size2D>>& waves);
	~Wave();


	/// @brief 直進波追加
	/// @param key キー
	/// @param wave 波の詳細
	void AddStraightWave(STRAIGHT_WAVE wave);

	/// @brief 環状波追加
	/// @param wave 波の詳細
	void AddCyclicWave(CYCLIC_WAVE wave);

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::MAP; }

	/// @brief 描画処理
	void DrawObject() override;

	/// @brief 基本的な更新処理
	void UpdateObject() override;

	/// @brief 通常Update後に走る処理
	void LateUpdateObject() override;

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdateObject() override;

	/// @brief 波を更新
	void UpdateWaves();

	/// @brief 環状波を更新
	void UpdateCyclicWaves();

	/// @brief 直進波を更新
	void UpdateStraightWaves();

	/// @brief ハイトマップの変更を適用
	void UpdateHeightMap();

	void CreateUploadResource();
	void CreateCopyResource();
	void MapUploadResourceToCopyResource();
	void CopyToGPUResource();

	struct WAVE_BUFFER;

	vector<CYCLIC_WAVE> cyclicWaves_;
	vector<STRAIGHT_WAVE> straightWaves_;

	unique_ptr<Mesh> mesh_;

	int waveBufferNo_;								/// @brief 波用定数バッファ管理番号
	WAVE_BUFFER* waveMap_;				/// @brief 波用定数更新用変数


	float count = 0.0f;

	static constexpr int HEIGHT_MAP_WIDTH  = (32);
	static constexpr int HEIGHT_MAP_HEIGHT = (32);

	float heightMap_[HEIGHT_MAP_WIDTH * HEIGHT_MAP_HEIGHT];

	ComPtr<ID3D12Resource> uploadResource_;
	ComPtr<ID3D12Resource> copyResource_;

	int resHeapNo_;

	mutex heightMapGuard_;
};

struct Wave::WAVE_BUFFER
{
	float time;
	float padding;
	float padding2;
	float padding3;
};

