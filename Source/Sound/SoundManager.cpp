#include "../Object/Component/Common/Transform.h"
#include "../Resource/ResourceManager.h"
#include "SoundObject.h"
#include "SoundManager.h"

SoundManager& SoundManager::GetInstance()
{
	static SoundManager soundMng;
	return soundMng;
}

void SoundManager::Update()
{
	UpdateListener();

	erase_if(sourceVoice_, [](pair<UINT,IXAudio2SourceVoice*> temp) {
		
		XAUDIO2_VOICE_STATE state{ 0 };
		temp.second->GetState(&state);
		if (state.BuffersQueued == 0)
		{
			return true;
		}
		return false;
		});

	//for (auto& [key, obj] : soundObjects_)
	//{
	//	obj->Update();
	//}

	erase_if(soundObjects_, [](const pair<UINT, shared_ptr<SoundObject>>& temp) {
		temp.second->Update();
		return temp.second->IsEnd();
		}
	);
}

int SoundManager::Play(RES_ID resID, PLAY_TYPE playType)
{
	// サウンドデータ取得
	const auto& sound = ResourceMng.GetSound(resID);

	//if (sound == nullptr)return -1;

	IXAudio2SourceVoice* tempSourceVoice = nullptr;

	Audio->CreateSourceVoice(&tempSourceVoice, sound.GetWaveFormat());

	XAUDIO2_BUFFER buffer{ 0 };
	buffer.pAudioData = sound.GetMediaData().data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(sound.GetMediaData().size());

	if (playType == PLAY_TYPE::LOOP) {
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	tempSourceVoice->SubmitSourceBuffer(&buffer);

	tempSourceVoice->Start(0);

	sourceVoice_.emplace(voiceCreateCount_,tempSourceVoice);

	int ret = voiceCreateCount_;
	voiceCreateCount_++;

	sound.RegisterSourceVoiceID(ret);

	return ret;

}

int SoundManager::PlaySoundObject(RES_ID resID, Position3D pos, PLAY_TYPE playType)
{
	// サウンドデータ取得
	const auto& sound = ResourceMng.GetSound(resID);
	//if (sound == nullptr)return -1;

	IXAudio2SourceVoice* tempSourceVoice = nullptr;

	Audio->CreateSourceVoice(&tempSourceVoice, sound.GetWaveFormat());

	XAUDIO2_BUFFER buffer{ 0 };
	buffer.pAudioData = sound.GetMediaData().data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(sound.GetMediaData().size());

	if (playType == PLAY_TYPE::LOOP) {
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	tempSourceVoice->SubmitSourceBuffer(&buffer);

	tempSourceVoice->Start(0);

	shared_ptr<SoundObject> object = make_shared<SoundObject>(tempSourceVoice, pos);

	soundObjects_.emplace(voiceCreateCount_, object);

	int ret = voiceCreateCount_;
	voiceCreateCount_++;

	sound.RegisterSourceVoiceID(ret);

	return ret;
}

void SoundManager::StartSound(int key)
{
	if (!sourceVoice_.contains(key))return;

	sourceVoice_.at(key)->Start();
}

void SoundManager::StopSound(int key)
{
	if (!sourceVoice_.contains(key))return;

	sourceVoice_.at(key)->Stop();
}

void SoundManager::DeleteSound(int key)
{
	if (!sourceVoice_.contains(key))return;

	sourceVoice_.at(key)->DestroyVoice();

	sourceVoice_.erase(key);
}

void SoundManager::DeleteSoundObject(int key)
{
	if (!soundObjects_.contains(key))return;

	soundObjects_.at(key)->Release();

	soundObjects_.erase(key);
}

void SoundManager::SetSoundVolume(int key, float volume)
{
	if (!sourceVoice_.contains(key))return;

	sourceVoice_.at(key)->SetVolume(volume);
}

void SoundManager::SetSoundObjectVolume(int key, float volume)
{
	if (!soundObjects_.contains(key))return;

	soundObjects_.at(key)->SetSoundVolume(volume);
}

void SoundManager::SetEmitterToSoundObject(int key, const Transform& emitter)
{
	if (!soundObjects_.contains(key))return;

	soundObjects_.at(key)->SetEmitter(emitter);
}

void SoundManager::Terminate()
{
	for (auto& [key, voice] : sourceVoice_) {
		voice->Stop();
		voice->FlushSourceBuffers();
		voice->DestroyVoice();
	}
	sourceVoice_.clear();
	for (auto& [key, voiceObj] : soundObjects_) {
		voiceObj->Release();
	}
	soundObjects_.clear();
}

void SoundManager::ResetListener()
{
	listenerTransform_.reset();
}

void SoundManager::SetListener(const Transform& listener)
{
	listenerTransform_ = listener;
}

Position3D SoundManager::GetListenerPosition()const
{
	if (listenerTransform_.has_value())
	{
		return listenerTransform_->get().pos;
	}
	return Position3D();
}

Quaternion SoundManager::GetListenerQuaternion()const
{
	if (listenerTransform_.has_value())
	{
		return listenerTransform_->get().qua;
	}
	return Quaternion();
}

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{

}

void SoundManager::UpdateListener()
{
	if (!listenerTransform_.has_value())return;

	const auto& lt = listenerTransform_->get();

	Vector3 front = lt.qua.GetForward();
	Vector3 up = lt.qua.GetUp();

	listener_.Position = { lt.pos.x, lt.pos.y, lt.pos.z }; // リスナーの位置
	listener_.OrientFront = { front.x, front.y, front.z }; // 正面方向
	listener_.OrientTop = { up.x, up.y, up.z };   // 上方向
	listener_.Velocity = { 0.0f, 0.0f, 0.0f };    // 移動速度
}
