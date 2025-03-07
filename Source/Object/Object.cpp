#include "Object.h"

Object::Object(const string& uniqueKey) :
objectUniqueKey_(uniqueKey)
{
	AddComponent(make_shared<Transform>());

	transform_ = GetComponent<Transform>();
}

Object::~Object()
{
	for (const auto& [id, c] : components_)
	{
		c->Release();
	}
	components_.clear();
}

void Object::Init()
{
	InitObject();

	OnCollisionUpdateSetting();

	for (const auto& [id, c] : components_)
	{
		c->Init();
	}
}

void Object::Draw()
{
	DrawObject();

	for (const auto& [id, c] : components_)
	{
		c->Draw();
	}
}

void Object::Update()
{
	UpdateObject();

	for (const auto& [id,c] : components_)
	{
		c->Update();
	}
}

void Object::PreUpdate()
{
	PreUpdateObject();

	for (const auto& [id, c] : components_)
	{
		c->PreUpdate();
	}
}

void Object::LateUpdate()
{
	LateUpdateObject();

	for (const auto& [id, c] : components_)
	{
		c->LateUpdate();
	}
}

void Object::OnCollisionUpdate(COLLISION_FASE fase)
{
	if (onCollisionUpdateList_.contains(fase))
	{
		onCollisionUpdateList_.at(fase)();
	}

	for (const auto& [id, c] : components_)
	{
		c->OnCollisionUpdate(fase);
	}
}

void Object::OnCameraUpdate()
{
	OnCameraUpdateObject();

	for (const auto& [id, c] : components_)
	{
		c->OnCameraUpdate();
	}
}

void Object::AddComponent(shared_ptr<Component> component, bool overwrite)
{
	//// 追加予定のものがすでに含まれているかどうか確認
	//if (components_.contains(component->GetComponentID()))
	//{
	//	// 上書きするか
	//	if (overwrite)
	//	{
	//		//// 上書き
	//		//components_.insert(
	//		//	component->GetComponentID(), move(component));

	//		return;
	//	}
	//	
	//	return;
	//}
	
	// コンポーネント追加
	components_.emplace(
		component->GetComponentID(), move(component));
}

void Object::SetActiveStateToAllComponents(bool isActive)
{
	for (auto& [id,component] : components_)
	{
		component->SetActive(isActive);
	}
}

void Object::SetTransform(Transform transform)
{
	if (!transform_.has_value())return;

	auto& t = transform_->get();

	t = transform;
	t.PreUpdate();
}

void Object::SetPosition(Position3D pos)
{
	if (!transform_.has_value())return;

	auto& t = transform_->get();

	t.pos = pos;
}

void Object::SetRotation(Quaternion qua)
{
	if (!transform_.has_value())return;

	auto& t = transform_->get();

	t.qua = qua;
}

void Object::SetScale(Scale3D scale)
{
	if (!transform_.has_value())return;

	auto& t = transform_->get();

	t.scale = scale;
}



