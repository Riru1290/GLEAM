#include "../RenderManager.h"
#include "../../Object/Component/Renderer/Renderers/Single/2D/Object2DRenderer.h"
#include "../../Object/Component/Common/Transform.h"
#include "../../Common/Mesh/Mesh.h"
#include "../../Graphics/Material/Material.h"
#include "../../Resource/Texture/Texture.h"
#include "../../Resource/ResourceManager.h"
#include "Fade.h"

Fade::Fade()
{
	render_ = RenderMng.CreateRenderer<Object2DRenderer>(LAYER::FADE);
	Transform t = {};
	t.pos = { SCREEN_CENTER.x, SCREEN_CENTER.y,0.0f };
	t.scale = Scale3D(1920.0f, 1080.0f, 0.0f);
	render_->SetTexture(ResourceMng.RegisterTextureResource(
		"Fade",
		move(make_unique<Texture>(XMFLOAT4(0.0f, 0.0f, 0.0f, 255.0f))))
	);
	render_->SetMeshTransformMatrixFromTransform(t);
	const auto& mesh = render_->GetMesh();
	auto& material = mesh.GetMaterial();
	material.SetPSOID(PSO_ID::FADE);

	CreateConstantBuffer();
}

Fade::~Fade()
{
	RenderMng.DeleteRenderer(render_);
}

void Fade::Update()
{
	if (fadeTime_ <= 0.0f)return;

	if (isFadeIn_) {
		timeCount_ += MainTimer.GetDelta();
	}
	else {
		timeCount_ -= MainTimer.GetDelta();
	}
	rate_ = timeCount_ / fadeTime_;

	rate_ = clamp(rate_, 0.0f, 1.0f);

	fadeMap_->rate = rate_;
}

void Fade::Draw()
{

}

bool Fade::IsEnd()
{
	return (rate_ == 1.0f);
}

bool Fade::IsFadeIn()
{
	return isFadeIn_;
}

void Fade::FadeIn(float fadeInTime)
{
	isFadeIn_ = true;
	fadeTime_ = fadeInTime;
	rate_ = 0.0f;
	timeCount_ = 0.0f;
}

void Fade::FadeOut(float fadeOutTime)
{
	isFadeIn_ = false;
	fadeTime_ = fadeOutTime;
	rate_ = 1.0f;
	timeCount_ = fadeTime_;
}

void Fade::CreateConstantBuffer()
{
	auto& mesh = render_->GetMesh();
	fadeMap_ = mesh.AddConstantBuffer<FadeData>(sizeof(FadeData), 7, fadeBufferNo_);
	fadeMap_->rate = rate_;
	fadeMap_->padding = {};
}
