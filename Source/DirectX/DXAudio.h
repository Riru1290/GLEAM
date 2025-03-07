#pragma once

// XAudio2
#include <xaudio2.h>
#include <X3DAudio.h>
#pragma comment(lib, "xaudio2.lib")


// MediaFoundation
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

#define Audio DXAudio::GetInstance().GetAudioDevice()

struct WaveData
{
	WAVEFORMATEX waveFormat;
	BYTE* audioData;
	DWORD dataSize;
};

class DXAudio
{
public:

	static DXAudio& GetInstance();

	void Update();

	//void TestPlay();

	void Destroy();

	bool Create();
	//void TestLoad();

	void AudioCalculate3D(IXAudio2SourceVoice* sourceVoice,X3DAUDIO_EMITTER& emitter, X3DAUDIO_LISTENER& listener);

	const ComPtr<IXAudio2>& GetAudioDevice();

private:

	//WaveData waveData;

	DXAudio() = default;
	~DXAudio() = default;
	DXAudio(const DXAudio&) = delete;
	DXAudio& operator=(const DXAudio&) = delete;

	ComPtr<IXAudio2> audioDevice_;
	IXAudio2MasteringVoice* masteringVoice_;


	X3DAUDIO_HANDLE x3DAudioHandle_;

	//// çƒê∂éûÇ…àÍéûìIÇ…
	//IXAudio2SourceVoice* sourceVoice_;

	//vector<BYTE> mediaData;

};

