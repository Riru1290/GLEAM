#include "../../Sound/SoundManager.h"
#include "Sound.h"

Sound::Sound(const string& filename)
{
    IMFSourceReader* pMFSourceReader{ nullptr };
    MFCreateSourceReaderFromURL(cp_to_wide(filename).c_str(), NULL, &pMFSourceReader);

    IMFMediaType* pMFMediaType{ nullptr };
    MFCreateMediaType(&pMFMediaType);
    pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    pMFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

    pMFMediaType->Release();
    pMFMediaType = nullptr;
    pMFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);

    waveFormat_ = { nullptr };
    MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat_, nullptr);

    // XAudio‚É“n‚·‚½‚ß
    while (true)
    {
        IMFSample* pMFSample{ nullptr };
        DWORD dwStreamFlags{ 0 };
        pMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

        if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            break;
        }

        IMFMediaBuffer* pMFMediaBuffer{ nullptr };
        pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

        BYTE* pBuffer{ nullptr };
        DWORD cbCurrentLength{ 0 };
        pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

        mediaData_.resize(mediaData_.size() + cbCurrentLength);
        memcpy(mediaData_.data() + mediaData_.size() - cbCurrentLength, pBuffer, cbCurrentLength);

        pMFMediaBuffer->Unlock();

        pMFMediaBuffer->Release();
        pMFSample->Release();
    }
}

Sound::~Sound()
{
}

void Sound::RegisterSourceVoiceID(int id) const
{
    sourceVoiceID_.emplace_back(id);
}

void Sound::Release()
{
    for (const int& id : sourceVoiceID_)
    {
        SoundMng.DeleteSound(id);
        SoundMng.DeleteSoundObject(id);
    }
}
