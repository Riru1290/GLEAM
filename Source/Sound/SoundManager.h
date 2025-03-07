#pragma once
#include "../Resource/Sound/Sound.h"
#include "../Resource/ResourceID.h"

#define SoundMng SoundManager::GetInstance()

enum class PLAY_TYPE
{
	NONE,
	LOOP
};

class Transform;
class Quaternion;
class SoundObject;

class SoundManager
{
public:

	static SoundManager& GetInstance();

	void Update();

	int Play(RES_ID resID, PLAY_TYPE playType = PLAY_TYPE::NONE);

	int PlaySoundObject(RES_ID resID, Position3D pos, PLAY_TYPE playType = PLAY_TYPE::NONE);

	void StartSound(int key);
	void StopSound(int key);
	void DeleteSound(int key);
	void DeleteSoundObject(int key);

	void SetSoundVolume(int key, float volume);
	void SetSoundObjectVolume(int key, float volume);

	/// @brief 出し手を設定
	/// @param emitter 出し手
	void SetEmitterToSoundObject(int key, const Transform& emitter);

	void Terminate();

	/// @brief 聞き手解除
	void ResetListener();

	/// @brief 聞き手を設定
	/// @param listener 聞き手
	void SetListener(const Transform& listener);

	[[nodiscard]] Position3D GetListenerPosition()const;

	[[nodiscard]] Quaternion GetListenerQuaternion()const;

	[[nodiscard]] X3DAUDIO_LISTENER& GetListener() { return listener_; }


private:

	SoundManager();
	~SoundManager();
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	void UpdateListener();

	// ソースボイス
	unordered_map<UINT, IXAudio2SourceVoice*> sourceVoice_;

	unordered_map<UINT, shared_ptr<SoundObject>> soundObjects_;


	X3DAUDIO_LISTENER listener_ = {};


	optional<reference_wrapper<const Transform>> listenerTransform_;		/// @brief 音の聞き手の位置
	
	UINT voiceCreateCount_ = 0;
};

