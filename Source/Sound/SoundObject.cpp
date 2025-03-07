#include "../Object/Component/Common/Transform.h"
#include "SoundManager.h"
#include "SoundObject.h"


SoundObject::SoundObject(IXAudio2SourceVoice* voice, Position3D pos, float distance):
pos_(pos),
voice_(voice),
distance_(distance)
{
	emitter_.Position = { pos.x, pos.y, pos.z }; // 音源の位置
	emitter_.Velocity = { 0.0f, 0.0f, 0.0f };  // 音源の速度
	emitter_.ChannelCount = 1;               // モノラルの場合
	emitter_.CurveDistanceScaler = 1.0f;     // 距離減衰スケール
	emitter_.OrientFront = { 0.0f,0.0f,1.0f };
	emitter_.OrientTop = { 0.0f,1.0f,1.0f };
}

SoundObject::~SoundObject()
{

}

void SoundObject::Update()
{
	if (emitterTransform_.has_value())
	{
		pos_ = emitterTransform_->get().pos;
	}

	float volume = 0.0f;

	Position3D listenerPos = SoundMng.GetListenerPosition();

	Vector3 dir = (listenerPos - pos_);

	float dis = dir.Length();

	float disRate = 1.0f - (dis / distance_);
	disRate = max(0.0f, disRate);

	volume += disRate;

	Quaternion listenerQua = SoundMng.GetListenerQuaternion();

	float dif = static_cast<float>
		(Radian2Deg(Quaternion::AngleBetween(Quaternion::LookRotation(dir.Normalized()), listenerQua)));

	float difRate = (fabsf(dif) / 180.0f) + 0.1f;
	difRate = max(0.0f, difRate);

	volume *= difRate * 0.5f;

	voice_->SetVolume(volume);


	// うまくいかない
	//DXAudio::GetInstance().AudioCalculate3D(voice_, emitter_, SoundMng.GetListener());

}

void SoundObject::Release()
{
	voice_->Stop();
	voice_->FlushSourceBuffers();
	voice_->DestroyVoice();
}

bool SoundObject::IsEnd()
{
	XAUDIO2_VOICE_STATE state{ 0 };
	voice_->GetState(&state);
	if (state.BuffersQueued == 0)
	{
		return true;
	}
	return false;
}

void SoundObject::SetSoundVolume(float volume)
{
	voice_->SetVolume(volume);
}

void SoundObject::SetEmitter(const Transform& emitter)
{
	emitterTransform_ = emitter;
}
