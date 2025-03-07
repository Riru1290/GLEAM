#include "../../Resource/ResourceManager.h"
#include "../../Graphics/RenderManager.h"
#include "../../Object/Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../../Object/Component/Common/Transform.h"
#include "../../Collision/CollisionManager.h"
#include "../Component/Collider/Colliders/BoxCollider.h"
#include "../../Graphics/Material/Material.h"
#include "Locker.h"

Locker::Locker(const string& uniqueKey): Object(uniqueKey),
	model_(ResourceMng.GetModel(RES_ID::MODEL_LOCKER))
{
	auto renderer = RenderMng.CreateRenderer<ModelRenderer>();

	renderer->SetModel(ResourceMng.GetModel(RES_ID::MODEL_LOCKER));

	renderer->SetTransform(transform_->get());

	AddComponent(renderer);

	renderer->GetModel().GetMeshes()[0]->GetMaterial().SetPSOID(PSO_ID::LIMLIGHT);
}

void Locker::InitObject()
{
	ColliderSetting();
}

void Locker::DrawObject()
{
}

void Locker::UpdateObject()
{
}

void Locker::ColliderSetting()
{
	BOX_COL_INFO box;
	box.width = 70.0f;
	box.height = 180.0f;
	box.depth = 70.0f;

	auto collider = make_shared<BoxCollider>();
	collider->SetBoxInfo(box);
	collider->SetRelativePosition(Position3D(0.0f, 112.5f, 0.0f));
	collider->SetFollower(transform_->get());
	collider->SetRigidState(true);
	collider->SetStaticState(true);

	AddComponent(collider);

	CollisionMng.RegisterCollider(collider, GetObjectUniqueKey(), OBJECT_ID::LOCKER);
}
