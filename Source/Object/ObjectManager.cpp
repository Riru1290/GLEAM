#include "Object.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::AddObjectFromList()
{
	if (addObjectList_.size() <= 0)return;

	for (auto& [key, object] : addObjectList_)
	{
		object->Init();
		object->SetActiveStateToAllComponents(true);

		AddObject(object);
	}
	addObjectList_.clear();
}

void ObjectManager::DeleteObjectFromList()
{
	if (deleteObjectList_.size() <= 0)return;

	for (auto& key : deleteObjectList_)
	{
		DeleteObject(key);
	}
	deleteObjectList_.clear();
}

void ObjectManager::Terminate()
{
	objects_.clear();
	objectListKey_.clear();
	objectListID_.clear();
}

void ObjectManager::AddObject(shared_ptr<Object> object)
{
	const string& key = object->GetObjectUniqueKey();

	objects_.emplace_back(object);

	objectListKey_[key] = object;
	objectListID_[object->GetObjectID()].emplace_back(object);
}

void ObjectManager::AddObjectLate(shared_ptr<Object> object)
{
	const string& key = object->GetObjectUniqueKey();

	addObjectList_[key] = object;
}

void ObjectManager::DeleteObject(const string& key)
{
	if (!objectListKey_.contains(key))return;

	shared_ptr<Object> object = objectListKey_.at(key).lock();

	erase_if(objects_, [this, &object](shared_ptr<Object> valObj)
		{
			return (valObj == object);
		});

	OBJECT_ID id = object->GetObjectID();
	if (objectListID_.contains(id))
	{
		auto& objVec = objectListID_.at(id);
		erase_if(objVec, [this, &object](weak_ptr<Object> valObj)
			{
				return (valObj.lock() == object);
			});
	}

	objectListKey_.erase(key);
}

void ObjectManager::DeleteObjectLate(const string& key)
{
	deleteObjectList_.emplace_back(key);
}

shared_ptr<Object> ObjectManager::GetSpecificObject(const string& key)const
{
	if (!objectListKey_.contains(key))return nullptr;

	return objectListKey_.at(key).lock();
}

vector<weak_ptr<Object>> ObjectManager::GetSpecificObjectArray(OBJECT_ID id) const
{
	vector<weak_ptr<Object>> ret;

	if (objectListID_.contains(id))
	{
		ret = objectListID_.at(id);
	}

	return ret;
}

void ObjectManager::InitObjects()
{
	for (auto& object : objects_)
	{
		object->Init();
	}
}

void ObjectManager::DrawObjects()
{
	for (auto& object : objects_)
	{
		object->Draw();
	}
}

void ObjectManager::UpdateObjects()
{
	for (auto& object : objects_)
	{
		object->Update();
	}
}

void ObjectManager::PreUpdateObjects()
{
	DeleteObjectFromList();
	AddObjectFromList();

	for (auto& object : objects_)
	{
		object->PreUpdate();
	}
}

void ObjectManager::LateUpdateObjects()
{
	for (auto& object : objects_)
	{
		object->LateUpdate();
	}
}

void ObjectManager::OnCollisionUpdateObjects(COLLISION_FASE fase)
{
	for (auto& object : objects_)
	{
		object->OnCollisionUpdate(fase);
	}
}

void ObjectManager::OnCameraUpdateObjects()
{
	for (auto& object : objects_)
	{
		object->OnCameraUpdate();
	}
}

void ObjectManager::ReleaseObjects()
{
	objects_.clear();
	objectListKey_.clear();
	objectListID_.clear();
	addObjectList_.clear();
	deleteObjectList_.clear();
}


