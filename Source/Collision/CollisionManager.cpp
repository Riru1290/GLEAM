#if defined(_DEBUG)
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_impl_win32.h"
#include "../../External/imgui/imgui_impl_dx12.h"
#endif
#include "../Object/Component/Collider/Collider.h"
#include "../Object/Component/Collider/Colliders/PointCollider.h"
#include "../Input/InputManager.h"
#include "Resolver/RigidBodyCollisionResolver.h"
#include "Detector/CollisionDetector.h"
#include "CollisionManager.h"

CollisionManager::CollisionManager():
	collisionCount_(0)
{
	CollisionListSetting();

	cursorCollider_ = make_shared<PointCollider>();
	RegisterCollider(cursorCollider_, "CURSOR", OBJECT_ID::CURSOR);
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::CollisionListSetting()
{
	collisionList_ = {
		{OBJECT_ID::CURSOR,		OBJECT_ID::UI,				COLLISION_FASE::FIRST},
		{OBJECT_ID::MAP,		OBJECT_ID::PLAYER,			COLLISION_FASE::FIRST},
		{OBJECT_ID::LOCKER,		OBJECT_ID::PLAYER,			COLLISION_FASE::FIRST},
		{OBJECT_ID::WAVE,		OBJECT_ID::PLAYER,			COLLISION_FASE::FIRST},
		{OBJECT_ID::DOOR,		OBJECT_ID::PLAYER,			COLLISION_FASE::FIRST},
		{OBJECT_ID::ENEMY,		OBJECT_ID::PLAYER,			COLLISION_FASE::SECOND},
		{OBJECT_ID::MAP,		OBJECT_ID::PLAYER_EYE,		COLLISION_FASE::SECOND},
		{OBJECT_ID::LOCKER,		OBJECT_ID::PLAYER_EYE,		COLLISION_FASE::SECOND},
		{OBJECT_ID::KEY,		OBJECT_ID::PLAYER_EYE,		COLLISION_FASE::SECOND},
		{OBJECT_ID::PADLOCK,	OBJECT_ID::PLAYER_EYE,		COLLISION_FASE::SECOND},
		{OBJECT_ID::LIGHTBULB,	OBJECT_ID::PLAYER_EYE,		COLLISION_FASE::SECOND},
		{OBJECT_ID::ENEMY_POINT,OBJECT_ID::PLAYER_EYE_LONG,	COLLISION_FASE::SECOND},
		{OBJECT_ID::ENEMY_EYE,	OBJECT_ID::PLAYER_POINT,	COLLISION_FASE::SECOND},
	};
}

void CollisionManager::Terminate()
{
}

void CollisionManager::RegisterCollider(shared_ptr<Collider> collider, const string& objectUniqueKey, OBJECT_ID objectID)
{
	colliders_[objectID].emplace_back(collider);

	collider->SetObjectUniqueKey(objectUniqueKey);
	collider->SetObjectID(objectID);
}

void CollisionManager::UnregisterCollider(shared_ptr<Component> collider)
{
	if (collider == nullptr)return;

	for (auto& [objID, renderers] : colliders_)
	{
		erase_if(renderers,
			[&collider](const weak_ptr<Collider>& weakCollider)
			{
				return weakCollider.lock() == collider;
			});
	}
}

void CollisionManager::Collision(COLLISION_FASE currentFase, int recursionCount)
{
	if (recursionCount == 0)return;

	UpdateCursorCollider();

	collisionCount_ = 0;

	// コリジョンリストに書いてある順番で衝突判定
	// 現在のフェーズのもののみ判定
	for (auto& [objA, objB, fase] : collisionList_) {

		// 選択されたフェーズ以外の場合処理を行わない
		if (fase != currentFase)continue;

		// 指定オブジェクト同士の衝突判定
		CollisionPerObject(objA, objB);

	}

	// 何かしら衝突していた場合
	if (collisionCount_ > 0)
	{

		// 再帰的に衝突判定
		Collision(currentFase, recursionCount - 1);
	}

}

void CollisionManager::CollisionPerObject(OBJECT_ID objA, OBJECT_ID objB)
{
    // コライダー配列から指定のオブジェクトのコライダーを取り出す
	if (colliders_.count(objA) == 0 || colliders_.count(objB) == 0)return;
    auto objAColliders = colliders_.at(objA);
    auto objBColliders = colliders_.at(objB);

    for (auto& objACol : objAColliders) {
		if (objACol.expired() || !objACol.lock()->IsActive())continue;

		int objAShape = static_cast<int>(objACol.lock()->GetShapeType());

        for (auto& objBCol : objBColliders) {
			if (objBCol.expired() || !objBCol.lock()->IsActive())continue;
           
			int objBShape = static_cast<int>(objBCol.lock()->GetShapeType());

			COL_HIT_DATA hitData;

			if (ColDetector.Detection(objAShape + objBShape, objACol, objBCol, hitData))
			{
				// 衝突後処理
				hitData.selfCollider = objACol;
				hitData.targetCollider = objBCol;
				objACol.lock()->OnHit(objB, hitData);

				hitData.selfCollider = objBCol;
				hitData.targetCollider = objACol;
				objBCol.lock()->OnHit(objA, hitData);

				// どちらも物理特性を持つオブジェクトであれば位置を更新する
				if (objACol.lock()->IsRigidBody() && objBCol.lock()->IsRigidBody())
				{
					ColResolver.Resolve(objAShape + objBShape,hitData);
				}

				collisionCount_++;
				
			}
        }
    }
}

void CollisionManager::UpdateCursorCollider()
{
	POINT_COL_INFO point;
	Position2D mousePos = InputMng.GetMousePosition();
	point.pos = { mousePos.x,mousePos.y,0.0f };
	cursorCollider_->SetPointInfo(point);
}
