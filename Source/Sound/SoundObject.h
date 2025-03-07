#pragma once
class SoundObject
{
public:

	SoundObject(IXAudio2SourceVoice* voice, Position3D pos, float distance = DEFAULT_DISTANCE);
	~SoundObject();

	void Update();

	void Release();

	[[nodiscard]] bool IsEnd();

	void SetSoundVolume(float volume);

	/// @brief �o�����ݒ�
	/// @param emitter �o����
	void SetEmitter(const Transform& emitter);

private:

	static constexpr float DEFAULT_DISTANCE = (100000.0f);

	IXAudio2SourceVoice* voice_;

	float distance_;


	Position3D pos_;
	Vector3 front_;

	X3DAUDIO_EMITTER emitter_ = {};

	optional<reference_wrapper<const Transform>> emitterTransform_;		/// @brief ���̏o����̈ʒu
};

