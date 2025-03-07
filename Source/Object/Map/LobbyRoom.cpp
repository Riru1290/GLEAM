#include "../../Resource/ResourceManager.h"
#include "../../Graphics/RenderManager.h"
#include "../../Object/Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "LobbyRoom.h"

LobbyRoom::LobbyRoom(const string& uniqueKey) : Object(uniqueKey)
{
	auto renderer = RenderMng.CreateRenderer<ModelRenderer>();

	renderer->SetModel(ResourceMng.GetModel(RES_ID::MODEL_LOBBY));
	renderer->SetTransform(transform_->get());

	AddComponent(renderer);
}
