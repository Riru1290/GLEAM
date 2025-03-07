#pragma once
#include "../Resource.h"

/// @brief サウンドリソース
class Sound final : public Resource
{
public:

	[[nodiscard]] Sound(const string& filename);
	~Sound();

	[[nodiscard]] const vector<BYTE>& GetMediaData() const { return mediaData_; };
	[[nodiscard]] const WAVEFORMATEX* GetWaveFormat() const { return waveFormat_; };

	void RegisterSourceVoiceID(int id) const;

private:

	/// @brief 解放処理
	void Release()override;

	vector<BYTE> mediaData_;
	WAVEFORMATEX* waveFormat_;

	mutable vector<int> sourceVoiceID_;
};

