#include "../../Resource/ResourceManager.h"
#include "../../Graphics/RenderManager.h"
#include "../../Object/Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../../Collision/CollisionManager.h"
#include "../../Object/Component/Collider/Colliders/SphereCollider.h"
#include "../../Game/Event/EventManager.h"
#include "../../Object/Component/Common/Transform.h"
#include "../../Sound/SoundManager.h"
#include "Padlock.h"

Padlock::Padlock(const string& uniqueKey, EVENT_ID iventID) : Object(uniqueKey),
iventID_(iventID)
{
	auto renderer = RenderMng.CreateRenderer<ModelRenderer>();

	renderer->SetModel(ResourceMng.GetModel(RES_ID::MODEL_PADLOCK));
	renderer->SetTransform(transform_->get());

	AddComponent(renderer);

	ColliderSetting();

	EventMng.RegisterEventAction(iventID,
		[this] {SoundMng.PlaySoundObject(RES_ID::SOUND_GET_KEY, transform_->get().pos); });
}

Padlock::~Padlock()
{
	// 南京錠を解除した際のイベントを起こす
	EventMng.Notify(iventID_);
}

void Padlock::ColliderSetting()
{
	SPHERE_COL_INFO sphere;
	sphere.radius = 10.0f;

	auto collider = make_shared<SphereCollider>();
	collider->SetSphereInfo(sphere);
	collider->SetFollower(transform_->get());
	collider->SetRelativePosition({ 0.0f,5.0f,0.0f });

	collider->AddOnHitFunction(OBJECT_ID::PLAYER_EYE, bind(&Padlock::OnHitPlayerEye, this, placeholders::_1));

	AddComponent(collider);

	CollisionMng.RegisterCollider(collider, GetObjectUniqueKey(), OBJECT_ID::PADLOCK);
}

void Padlock::OnHitPlayerEye(COL_HIT_DATA data)
{
}
