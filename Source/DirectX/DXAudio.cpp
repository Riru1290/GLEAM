#include "DXAudio.h"

inline UINT32 XAudio2GetChannelCount(DWORD ChannelMask) {
    return __popcnt(ChannelMask);
}

bool LoadWaveFile(const char* filename, WaveData& waveData)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        assert(0 && "WAV�t�@�C���I�[�v�����s");
        return false;
    }

    // RIFF�w�b�_��ǂݍ���
    char riff[4];
    file.read(riff, 4);
    if (strncmp(riff, "RIFF", 4) != 0)
    {
        assert(0 && "������WAV�t�@�C��");
        return false;
    }

    // �t�@�C���T�C�Y��ǂݔ�΂�
    file.seekg(4, std::ios::cur);

    // WAVE�w�b�_��ǂݍ���
    char wave[4];
    file.read(wave, 4);
    if (strncmp(wave, "WAVE", 4) != 0)
    {
        assert(0 && "������WAV�t�@�C��");
        return false;
    }

    // �t�H�[�}�b�g�`�����N��T��
    char fmt[4];
    while (file.read(fmt, 4))
    {
        DWORD chunkSize;
        file.read(reinterpret_cast<char*>(&chunkSize), 4);

        if (strncmp(fmt, "fmt ", 4) == 0)
        {
            file.read(reinterpret_cast<char*>(&waveData.waveFormat), sizeof(WAVEFORMATEX));
            file.seekg(chunkSize - sizeof(WAVEFORMATEX), ios::cur);
        }
        else if (strncmp(fmt, "data", 4) == 0)
        {
            waveData.dataSize = chunkSize;
            waveData.audioData = new BYTE[chunkSize];
            file.read(reinterpret_cast<char*>(waveData.audioData), chunkSize);
            break;
        }
        else
        {
            file.seekg(chunkSize, ios::cur);
        }
    }

    file.close();
    return true;
}

void FreeWaveData(WaveData& waveData)
{
    delete[] waveData.audioData;
}

DXAudio& DXAudio::GetInstance()
{
    static DXAudio audioDevice;
    return audioDevice;
}

void DXAudio::Update()
{

}

void DXAudio::Destroy()
{
    if (masteringVoice_) {
        masteringVoice_->DestroyVoice();
        masteringVoice_ = nullptr;
    }

    audioDevice_->Release();
}

bool DXAudio::Create()
{
    if (FAILED(XAudio2Create(&audioDevice_)))
    {
        return false;
    }
    if (FAILED(audioDevice_->CreateMasteringVoice(&masteringVoice_)))
    {
        return false;
    }

    MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

    DWORD speakerChannelMask;

    audioDevice_->CreateMasteringVoice(&masteringVoice_);
    masteringVoice_->GetChannelMask(&speakerChannelMask);

    if (FAILED(X3DAudioInitialize(speakerChannelMask, X3DAUDIO_HANDLE_BYTESIZE, x3DAudioHandle_)))
    {
        return false;
    }

    return true;
}

const ComPtr<IXAudio2>& DXAudio::GetAudioDevice()
{
    if (audioDevice_ == nullptr)
    {
        Create();
    }
    return audioDevice_;
}

void DXAudio::AudioCalculate3D(IXAudio2SourceVoice* sourceVoice, X3DAUDIO_EMITTER& emitter, X3DAUDIO_LISTENER& listener)
{
    // �X�s�[�J�[�}�X�N���擾
    DWORD speakerChannelMask = 0;
    masteringVoice_->GetChannelMask(&speakerChannelMask);

    // �o�̓X�s�[�J�[�̃`�����l�������擾
    UINT32 dstChannelCount = XAudio2GetChannelCount(speakerChannelMask);

    // DSP �ݒ�̏���
    X3DAUDIO_DSP_SETTINGS dspSettings = {};
    dspSettings.SrcChannelCount = 1; // ���m���������i1�`�����l���j������
    dspSettings.DstChannelCount = dstChannelCount; // �o�̓X�s�[�J�[�̃`�����l����

    // �}�g���b�N�X�̃��������m��
    vector<FLOAT32> matrix(dstChannelCount); // �K�v�ȃ`���l�������̃��������m��
    dspSettings.pMatrixCoefficients = matrix.data();

    // X3DAudio �̌v�Z
    X3DAudioCalculate(x3DAudioHandle_, &listener, &emitter,
        X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER,
        &dspSettings);

    // �o�̓}�g���b�N�X���X�V
    HRESULT hr = sourceVoice->SetOutputMatrix(masteringVoice_, dspSettings.SrcChannelCount, dspSettings.DstChannelCount, dspSettings.pMatrixCoefficients);
    if (FAILED(hr))
    {
        assert(0 && "�I�[�f�B�I�}�g���b�N�X�X�V���s");
    }
}



