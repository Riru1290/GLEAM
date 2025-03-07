#include "../../Resource/ResourceManager.h"
#include "../../Graphics/RenderManager.h"
#include "../../Sound/SoundManager.h"
#include "../../Object/Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../../Resource/Model/Model.h"
#include "../../Resource/Texture/Texture.h"
#include "../../Common/Mesh/Mesh.h"
#include "../../Graphics/Material/Material.h"
#include "../../Object/Component/Common/Transform.h"
#include "TV.h"

TV::TV(const string& uniqueKey, RES_ID screenTexture, int xNum, int yNum) : Object(uniqueKey)
{

	auto renderer = RenderMng.CreateRenderer<ModelRenderer>();

	renderer->SetModel(ResourceMng.GetModel(RES_ID::MODEL_TV));
	renderer->SetTransform(transform_->get());

	const auto& meshes = renderer->GetModel().GetMeshes();

	auto& materialTV = meshes[0]->GetMaterial();
	materialTV.SetPSOID(PSO_ID::TV);

	tvScreen_ = meshes[1]->GetMaterial();

	tvScreen_->get().SetPSOID(PSO_ID::TVSCREEN);
	tvScreen_->get().SetDiffuseMap(ResourceMng.GetTexture(screenTexture));

	AddComponent(renderer);

	CreateConstantBuffer();

	tvMap_->screenDivideXNum = static_cast<float>(xNum);
	tvMap_->screenDivideYNum = static_cast<float>(yNum);
}

TV::~TV()
{
	const auto& meshes = GetComponent<ModelRenderer>().GetModel().GetMeshes();
	for (const auto& mesh : meshes)
	{
		mesh->ClearConstantBuffer();
	}
}

void TV::ChangeTVChannel(int no)
{
	float channel = static_cast<float>(no);
	if (channel == tvMap_->channel)return;

	tvMap_->preChannel = tvMap_->channel;
	tvMap_->channel = static_cast<float>(no);

	tvMap_->currentTime = 0.0f;

	SoundMng.Play(RES_ID::SOUND_CHANNEL);
}

void TV::UpdateObject()
{
	tvMap_->currentTime += MainTimer.GetMultDelta(1.0f);
	tvMap_->currentTime = min(tvMap_->currentTime, tvMap_->changeTime);
}

void TV::CreateConstantBuffer()
{
	auto& renderer = GetComponent<ModelRenderer>();
	const auto& mesh = renderer.GetModel().GetMeshes()[1];

	tvMap_ = 
		mesh->AddConstantBuffer<TV_BUFFER>(sizeof(TV_BUFFER), 13, tvBufferNo_);

	tvMap_->changeTime = 0.5f;
	tvMap_->currentTime = 0.0f;

	tvMap_->channel = 0.0f;
	tvMap_->preChannel = 0.0f;
}


