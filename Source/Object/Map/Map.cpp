#ifdef _DEBUG
#include "../../Graphics/DebugRender/DebugRenderManager.h"
#include "../../Graphics/Material/Material.h"
#include "../../Resource/Texture/Texture.h"
#endif
#include "../../Resource/ResourceManager.h"
#include "../Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../Component/Collider/Colliders/ModelCollider.h"
#include "../../Resource/Model/Model.h"
#include "../../Graphics/RenderManager.h"
#include "../../Collision/CollisionManager.h"
#include "Map.h"


Map::Map(const string& uniqueKey) : Object(uniqueKey)
{
	AddComponent(RenderMng.CreateRenderer<ModelRenderer>());
	// モデル読み込み
	GetComponent<ModelRenderer>().SetModel(ResourceMng.GetModel(RES_ID::MODEL_MAP));

	//GetComponent<ModelRenderer>().GetModel().GetMeshes()[0]->GetMaterial().SetDiffuseMap(
	//	ResourceMng.RegisterTextureResource(
	//		"DebugMap", move(make_unique<Texture>("Resource/Model/Map/Texture/BASE COLOR_CarpetFloor.png")
	//		)
	//	));

	navMesh_ = MakeNavMesh(ResourceMng.GetModel(RES_ID::NAVIMESH_MAP));

	//renderer = RenderMng.CreateRenderer<ModelRenderer>();
	//renderer->SetModel(ResourceMng.GetModel(RES_ID::NAVIMESH_MAP));
	//renderer->GetModel().GetMeshes()[0]->GetMaterial().SetDiffuseMap(
	//	ResourceMng.RegisterTextureResource(
	//		"DebugMap", move(make_unique<Texture>(XMFLOAT4(75.0f, 75.0f, 0.0f, 255.0f))
	//		)
	//	));

	// コライダー設定
	ColliderSetting();
}

Map::~Map()
{
	//RenderMng.DeleteRenderer(renderer);
}

void Map::DrawObject()
{
#ifdef _DEBUG
	//for (const auto& p : navMesh_.polygons)
	//{
	//	for (const auto& pp : p.vertices)
	//	{

	//		DebugRender.DrawSphere(pp, 10.0f, 0xffff00, 255.0f, true);
	//	}
	//}
#endif
}

void Map::UpdateObject()
{
}

void Map::LateUpdateObject()
{
}

void Map::OnCameraUpdateObject()
{
}

void Map::ColliderSetting()
{
	MODEL_COL_INFO model;
	model.CreatePolygons(GetComponent<ModelRenderer>().GetModel());

	auto collider = make_shared<ModelCollider>();
	collider->SetModelInfo(model);
	collider->SetRigidState(true);
	collider->SetStaticState(true);

	AddComponent(collider);

	CollisionMng.RegisterCollider(collider,GetObjectUniqueKey(), OBJECT_ID::MAP);

}
