#include "../../Object/Component/Renderer/Renderers/Single/2D/Object2DRenderer.h"
#include "../../Graphics/RenderManager.h"
#include "../../Graphics/Material/Material.h"
#include "../../Common/Mesh/Mesh.h"
#include "DefaultUI.h"

//DefaultUI::DefaultUI(const Texture& texture) :UIObject(texture)
//{
//	AddComponent(RenderMng.CreateRenderer<Object2DRenderer>(LAYER::TEXTURE_2D));
//
//	renderer_ = GetComponent<Object2DRenderer>();
//
//	renderer_->get().SetTexture(texture);
//	renderer_->get().SetTransform(transform_->get());
//
//	renderer_->get().GetMesh().GetMaterial().SetPSOID(PSO_ID::DEFAULT_2D);
//
//}

DefaultUI::DefaultUI() : UIObject()
{
}

void DefaultUI::UpdateUI()
{
}
