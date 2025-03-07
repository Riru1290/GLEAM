#include "../Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "../Renderer/Renderers/Single/2D/Object2DRenderer.h"
#include "../Renderer/Renderers/Single/MeshRenderer.h"
#include "ModelBlinker.h"

ModelBlinker::ModelBlinker() : Component(),
timeCount_(0.0f)
{
}

void ModelBlinker::SetFadeRenderer(ModelRenderer& renderer)
{
	modelRenderer_ = renderer;
}

void ModelBlinker::SetFadeRenderer(AnimationModelRenderer& renderer)
{
	animModelRenderer_ = renderer;
}

void ModelBlinker::SetFadeRenderer(Object2DRenderer& renderer)
{
	object2DRenderer_ = renderer;
}

void ModelBlinker::SetFadeRenderer(MeshRenderer& renderer)
{
	meshRenderer_ = renderer;
}

void ModelBlinker::UpdateComponent()
{
	timeCount_ += MainTimer.GetMultDelta(blinkSpeed_);

	rate_ = (sinf(timeCount_) + 1.0f) / 2.0f;
	//rate_ = clamp(rate_, 0.0f, 1.0f);

	if (modelRenderer_.has_value()) {
		modelRenderer_->get().SetModelAlpha(rate_);
	}
	else if (animModelRenderer_.has_value()) {
		animModelRenderer_->get().SetModelAlpha(rate_);
	}
	else if (object2DRenderer_.has_value()) {
		object2DRenderer_->get().SetMeshAlpha(rate_);
	}
	else if (meshRenderer_.has_value()) {
		meshRenderer_->get().SetMeshAlpha(rate_);
	}
}
