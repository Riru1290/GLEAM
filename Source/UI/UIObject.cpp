#include "../Object/Component/Common/Transform.h"
#include "UIObject.h"

UIObject::UIObject()
{
	AddComponent(make_shared<Transform>());

	transform_ = GetComponent<Transform>();
}

UIObject::~UIObject()
{
	for (const auto& [id, c] : components_)
	{
		c->Release();
	}
	components_.clear();
}

void UIObject::Init()
{
	InitUI();

	for (const auto& [id, c] : components_)
	{
		c->Init();
	}
}

void UIObject::Update()
{
	UpdateUI();

	for (const auto& [id, c] : components_)
	{
		c->Update();
	}
	for (const auto& [id, c] : components_)
	{
		c->LateUpdate();
	}
}

void UIObject::AddComponent(shared_ptr<Component> component, bool overwrite)
{
	// 追加予定のものがすでに含まれているかどうか確認
	if (components_.contains(component->GetComponentID()))
	{
		// 上書きするか
		if (overwrite)
		{
			// 上書き
			components_.insert_or_assign(
				component->GetComponentID(), move(component));

			return;
		}

		return;
	}

	// コンポーネント追加
	components_.emplace(
		component->GetComponentID(), move(component));
}

void UIObject::SetActiveStateToAllComponents(bool isActive)
{
	for (auto& [id, component] : components_)
	{
		component->SetActive(isActive);
	}
}

void UIObject::SetTransform(Transform transform)
{
	if (!transform_.has_value())return;

	auto& t = transform_->get();

	t = transform;
}

void UIObject::SetPosition(Position3D pos)
{
	if (!transform_.has_value())return;

	auto& t = transform_->get();

	t.pos = pos;
}

void UIObject::SetRotation(Quaternion qua)
{
	if (!transform_.has_value())return;

	auto& t = transform_->get();

	t.qua = qua;
}

void UIObject::SetScale(Scale3D scale)
{
	if (!transform_.has_value())return;

	auto& t = transform_->get();

	t.scale = scale;
}
