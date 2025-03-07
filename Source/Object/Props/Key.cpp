#include "../../Resource/ResourceManager.h"
#include "../../Graphics/RenderManager.h"
#include "../../Collision/CollisionManager.h"
#include "../../Object/Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../../Object/Component/Collider/Colliders/SphereCollider.h"
#include "../../Object/Component/Common/Transform.h"
#include "../../Object/ObjectManager.h"
#include "../../Resource/Texture/Texture.h"
#include "../../Common/Mesh/Mesh.h"
#include "../../Graphics/Material/Material.h"
#include "../../Resource/Model/Model.h"
#include "../../Game/Event/EventManager.h"
#include "../../Sound/SoundManager.h"
#include "Key.h"

Key::Key(const string& uniqueKey, EVENT_ID getID, EVENT_ID useID) : Object(uniqueKey),
getIventID_(getID),
useIventID_(useID)
{
	auto renderer = RenderMng.CreateRenderer<ModelRenderer>();

	renderer->SetModel(ResourceMng.GetModel(RES_ID::MODEL_KEY));
	renderer->SetTransform(transform_->get());

	AddComponent(renderer);

	ColliderSetting();

	EventMng.RegisterEventAction(getIventID_,
		[this] {SoundMng.PlaySoundObject(RES_ID::SOUND_GET_KEY, transform_->get().pos);
				ObjectMng.DeleteObjectLate(GetObjectUniqueKey()); });

}


Key::~Key()
{
	//// 鍵を取得した際のイベントを起こす
	//EventMng.Notify(getIventID_);
}

void Key::UpdateObject()
{
}

void Key::ColliderSetting()
{
	SPHERE_COL_INFO sphere;
	sphere.radius = 20.0f;

	auto collider = make_shared<SphereCollider>();
	collider->SetSphereInfo(sphere);
	collider->SetFollower(transform_->get());

	collider->AddOnHitFunction(OBJECT_ID::PLAYER_EYE, bind(&Key::OnHitPlayerEye, this, placeholders::_1));

	AddComponent(collider);

	CollisionMng.RegisterCollider(collider, GetObjectUniqueKey(), OBJECT_ID::KEY);
}

void Key::OnHitPlayerEye(COL_HIT_DATA data)
{
}
