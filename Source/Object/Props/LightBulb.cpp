#include "../../Resource/ResourceManager.h"
#include "../../Graphics/RenderManager.h"
#include "../../Object/Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../../Collision/CollisionManager.h"
#include "../../Object/Component/Collider/Colliders/SphereCollider.h"
#include "../../Object/Component/Common/DistanceChecker.h"
#include "../../Object/Component/Common/Transform.h"
#include "../../Resource/Model/Model.h"
#include "../../Common/Mesh/Mesh.h"
#include "LightBulb.h"

namespace
{
	const float BASE_DISTANCE = (1750.0f);
	const Vector3 BASE_COLOR = { 0.0f,1.0f,0.0f };
	const Vector3 DEN_COLOR = { 1.0f,0.0f,0.0f };
}

LightBulb::LightBulb(const string& uniqueKey, const string& lightKey) : Object(uniqueKey),
lightKey_(lightKey),
isCreateLight_(true)
{
	auto renderer = RenderMng.CreateRenderer<ModelRenderer>();

	renderer->SetModel(ResourceMng.GetModel(RES_ID::MODEL_LIGHTBULB));
	renderer->SetTransform(transform_->get());
	//Transform t = {};
	//t.pos = { 100.0f,15.0f,-20.0f };
	//t.scale = { 5.0f,5.0f,5.0f };
	//t.qua = Quaternion::Euler({ 0.0f,Deg2Radian(0.0f),0.0f });
	//renderer->SetModelTransformMatrixFromTransform(t);

	const auto& meshes = renderer->GetModel().GetMeshes();
	meshes[1]->SetTransState(true);

	AddComponent(renderer);

	auto checker = make_shared<DistanceChecker>();
	checker->SetTransform(transform_->get());
	distanceChecker_ = *checker;

	AddComponent(checker);


	ColliderSetting();
}

LightBulb::LightBulb(const string& uniqueKey) : Object(uniqueKey),
isCreateLight_(false)
{
	auto renderer = RenderMng.CreateRenderer<ModelRenderer>();

	renderer->SetModel(ResourceMng.GetModel(RES_ID::MODEL_LIGHTBULB));
	renderer->SetTransform(transform_->get());
	//Transform t = {};
	//t.pos = { 100.0f,15.0f,-20.0f };
	//t.scale = { 5.0f,5.0f,5.0f };
	//t.qua = Quaternion::Euler({ 0.0f,Deg2Radian(0.0f),0.0f });
	//renderer->SetModelTransformMatrixFromTransform(t);

	const auto& meshes = renderer->GetModel().GetMeshes();
	meshes[1]->SetTransState(true);

	AddComponent(renderer);

	auto checker = make_shared<DistanceChecker>();
	checker->SetTransform(transform_->get());
	distanceChecker_ = *checker;

	AddComponent(checker);


	//ColliderSetting();
}

LightBulb::~LightBulb()
{
	Light.DeletePointLightInfo(lightKey_);
}

void LightBulb::InitObject()
{
	if (!isCreateLight_)return;
	lightInfo_ = {};
	lightInfo_.pos = transform_->get().pos + transform_->get().qua.PosAxis({ 0.0f,0.0f,30.0f });
	lightInfo_.color = { 0.0f,1.0f,0.0f };
	lightInfo_.radius = 2000.0f;

	Light.AddPointLight(lightInfo_, lightKey_);
}

void LightBulb::LateUpdateObject()
{
	float rate =
		(BASE_DISTANCE - distanceChecker_->get().GetClosestTargetDistance()) / BASE_DISTANCE;

	rate = clamp(rate, 0.0f, 1.0f);

	lightInfo_.color = Lerp(BASE_COLOR, DEN_COLOR, rate);

	Light.SetPointLightInfo(lightInfo_, lightKey_);
}

void LightBulb::ColliderSetting()
{
	SPHERE_COL_INFO sphere;
	sphere.radius = 5.0f;

	auto collider = make_shared<SphereCollider>();
	collider->SetSphereInfo(sphere);
	collider->SetFollower(transform_->get());

	collider->AddOnHitFunction(OBJECT_ID::PLAYER_EYE, bind(&LightBulb::OnHitPlayerEye, this, placeholders::_1));

	AddComponent(collider);

	CollisionMng.RegisterCollider(collider, GetObjectUniqueKey(), OBJECT_ID::LIGHTBULB);
}

void LightBulb::OnHitPlayerEye(COL_HIT_DATA data)
{
}
