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

	/// @brief フェード情報定数バッファ作成
	void CreateConstantBuffer();

	// フェードインアウトの割合(0～1)
	// 0;何もなし 1:真っ黒 みたいな感じ
	float rate_;
	bool isFadeIn_;

	float timeCount_ = 0.0f;
	float fadeTime_ = 0.0f;

	shared_ptr<Object2DRenderer> render_;

	int fadeBufferNo_;								/// @brief フェード情報管理番号
	FadeData* fadeMap_;								/// @brief バッファ更新用変数
};

struct Fade::FadeData
{
	float rate;
	Vector3 padding;
};
