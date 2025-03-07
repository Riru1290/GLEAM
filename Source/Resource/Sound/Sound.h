#pragma once
#include "../Resource.h"

/// @brief �T�E���h���\�[�X
class Sound final : public Resource
{
public:

	[[nodiscard]] Sound(const string& filename);
	~Sound();

	[[nodiscard]] const vector<BYTE>& GetMediaData() const { return mediaData_; };
	[[nodiscard]] const WAVEFORMATEX* GetWaveFormat() const { return waveFormat_; };

	void RegisterSourceVoiceID(int id) const;

private:

	/// @brief �������
	void Release()override;

	vector<BYTE> mediaData_;
	WAVEFORMATEX* waveFormat_;

	mutable vector<int> sourceVoiceID_;
};

