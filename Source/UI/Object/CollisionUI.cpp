#include "../../Object/Component/Collider/Colliders/Rect2DCollider.h"
#include "../../Collision/CollisionManager.h"
#include "CollisionUI.h"

CollisionUI::CollisionUI()
{
}

void CollisionUI::SetColliderRectInfo(RECT_COL_INFO rect)
{
	collider_->get().SetRectInfo(rect);
}

void CollisionUI::SetColliderActiveState(bool isActive)
{
	collider_->get().SetActive(isActive);
}

void CollisionUI::AddColliderOnHitFunction(OBJECT_ID objectID, function<void(COL_HIT_DATA)> onHit)
{
	collider_->get().AddOnHitFunction(objectID, onHit);
}

void CollisionUI::UpdateUI()
{
}

void CollisionUI::InitAfterSetTexture()
{
	shared_ptr<Rect2DCollider> collider = make_shared<Rect2DCollider>();
	CollisionMng.RegisterCollider(collider, "UI", OBJECT_ID::UI);

	if (object2DRenderer_.has_value())
	{
		const Size2D& texSize = object2DRenderer_->get().GetTextureSize();

		RECT_COL_INFO rect = {};
		rect.width = texSize.x;
		rect.height = texSize.y;
		collider->SetRectInfo(rect);
	}
	collider->SetFollower(transform_->get());

	collider_ = *collider;

	AddComponent(collider);
}
