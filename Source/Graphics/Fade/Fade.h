#pragma once
class Object2DRenderer;

class Fade
{
public:

	Fade();
	~Fade();

	void Update();
	void Draw();

	[[nodiscard]] bool IsEnd();
	[[nodiscard]] bool IsFadeIn();

	void FadeIn(float fadeInTime);
	void FadeOut(float fadeOutTime);

	float GetRate() { return rate_; };

private:

	struct FadeData;

	/// @brief �t�F�[�h���萔�o�b�t�@�쐬
	void CreateConstantBuffer();

	// �t�F�[�h�C���A�E�g�̊���(0�`1)
	// 0;�����Ȃ� 1:�^���� �݂����Ȋ���
	float rate_;
	bool isFadeIn_;

	float timeCount_ = 0.0f;
	float fadeTime_ = 0.0f;

	shared_ptr<Object2DRenderer> render_;

	int fadeBufferNo_;								/// @brief �t�F�[�h���Ǘ��ԍ�
	FadeData* fadeMap_;								/// @brief �o�b�t�@�X�V�p�ϐ�
};

struct Fade::FadeData
{
	float rate;
	Vector3 padding;
};
