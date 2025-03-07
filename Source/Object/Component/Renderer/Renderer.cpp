#include "../../../Graphics/RenderManager.h"
#include "Renderer.h"

Renderer::Renderer() : Component()
{
}

void Renderer::LateUpdateComponent()
{
	UpdateRenderer();
}

void Renderer::ReleaseComponent()
{
	ReleaseRenderer();
	RenderMng.DeleteRenderer(GetPtr());
}
