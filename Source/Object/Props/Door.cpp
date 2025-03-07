#include "../../Resource/ResourceManager.h"
#include "../../Graphics/RenderManager.h"
#include "../../Object/Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../../Collision/CollisionManager.h"
#include "../Component/Collider/Colliders/BoxCollider.h"
#include "../Component/Controller/SmoothRotateController.h"
#include "../../Game/Event/EventManager.h"
#include "Door.h"

Door::Door(const string& uniqueKey,int padLockNum) : Object(uniqueKey),
padLockNum_(padLockNum),
state_(STATE::CLOSE)
{
	auto renderer = RenderMng.CreateRenderer<ModelRenderer>();

	renderer->SetModel(ResourceMng.GetModel(RES_ID::MODEL_DOOR));
	renderer->SetTransform(transform_->get());

	AddComponent(renderer);

	EventMng.RegisterEventAction(EVENT_ID::UNLOCK_PADLOCK_A, [this] {UnlockPadlockOnDoor(); });
	EventMng.RegisterEventAction(EVENT_ID::UNLOCK_PADLOCK_B, [this] {UnlockPadlockOnDoor(); });
	EventMng.RegisterEventAction(EVENT_ID::UNLOCK_PADLOCK_C, [this] {UnlockPadlockOnDoor(); });
	EventMng.RegisterEventAction(EVENT_ID::UNLOCK_ALL_PADLOCK, [this] {OpenDoor(); });

	onCollisionUpdateList_[COLLISION_FASE::FIRST] =
		[this] {
		if (rotateCon_->get().IsGoal())
		{
			if (state_ == STATE::ROTATE_TO_CLOSE) {
				state_ = STATE::CLOSE;
			}
			else if (state_ == STATE::ROTATE_TO_OPEN) {
				state_ = STATE::OPEN;
				EventMng.Notify(EVENT_ID::OPEN_DOOR);
			}
		}
		};
}

void Door::UnlockPadlockOnDoor()
{
	padLockNum_--;

	padLockNum_ = max(0, padLockNum_);

	if (padLockNum_ == 0)
	{
		EventMng.Notify(EVENT_ID::UNLOCK_ALL_PADLOCK);
	}
}

void Door::OpenDoor()
{
	if (padLockNum_ > 0)return;

	rotateCon_->get().SetGoalQuaternion(Quaternion::Euler(0.0f,Deg2Radian(270.0f),0.0f));

	state_ = STATE::ROTATE_TO_OPEN;
}

void Door::CloseDoor()
{
	rotateCon_->get().SetGoalQuaternion(Quaternion::Euler(0.0f, 0.0f, 0.0f));

	state_ = STATE::ROTATE_TO_CLOSE;
}

void Door::InitObject()
{
	ColliderSetting();

	auto rotateCon = make_shared<SmoothRotateController>();
	rotateCon->SetTransform(transform_->get());
	rotateCon->SetRelativePosition({ -55.0f,0.0f,-18.0f });
	rotateCon->SetFixedState(true);
	rotateCon->SetRotatePow(100.0f);
	AddComponent(rotateCon);
	rotateCon_ = *rotateCon;
}

void Door::UpdateObject()
{

}

void Door::ColliderSetting()
{
	BOX_COL_INFO box;
	box.width = 120.0f;
	box.height = 255.0f;
	box.depth = 10.0f;

	auto collider = make_shared<BoxCollider>();
	collider->SetBoxInfo(box);
	collider->SetRelativePosition(Position3D(0.0f, 140.0f, 18.0f));
	collider->SetFollower(transform_->get());
	collider->SetRigidState(true);
	collider->SetStaticState(true);

	AddComponent(collider);

	CollisionMng.RegisterCollider(collider, GetObjectUniqueKey(), OBJECT_ID::DOOR);
	boxCollider_ = *collider;
}
