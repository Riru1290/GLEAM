#include "../../ObjectManager.h"
#include "../../../Game/Event/EventManager.h"
#include "../../Props/Key.h"
#include "KeyInventory.h"

KeyInventory::KeyInventory() : Component(),
getKeyCount_(0),
getKeyObjectUniqueKey_("")
{
	EventMng.RegisterEventAction(EVENT_ID::GET_KEY_A,
		[this] {useIventID_.insert(EVENT_ID::USE_KEY_A); 
				getKeyCount_++; });
	EventMng.RegisterEventAction(EVENT_ID::GET_KEY_B,
		[this] {useIventID_.insert(EVENT_ID::USE_KEY_B); 
				getKeyCount_++; });
	EventMng.RegisterEventAction(EVENT_ID::GET_KEY_C,
		[this] {useIventID_.insert(EVENT_ID::USE_KEY_C);
				getKeyCount_++; });
}

bool KeyInventory::UseKey()
{
	if (getKeyCount_ <= 0)return false;

	getKeyCount_--;

	//string key = *(getKeyName_.end() - 1);
	//getKeyName_.pop_back();
	if (!useIventID_.empty()) {
		EventMng.Notify(*useIventID_.begin());
		useIventID_.erase(useIventID_.begin());
	}

	return true;
}

void KeyInventory::GetKey()
{
	useIventID_.insert(static_pointer_cast<Key>(ObjectMng.GetSpecificObject(getKeyObjectUniqueKey_))->GetUseIventID());

	//// 指定の鍵をフィールド上から削除
	//ObjectMng.DeleteObjectLate(getKeyObjectUniqueKey_);

	EventMng.Notify(
		static_pointer_cast<Key>(ObjectMng.GetSpecificObject(getKeyObjectUniqueKey_))->GetGetIventID()
	);

	// 取得数を増加
	//getKeyCount_++;
	//getKeyName_.emplace_back(getKeyObjectUniqueKey_);

	getKeyObjectUniqueKey_ = "";

}

void KeyInventory::GetKey(const string& keyName)
{

	useIventID_.insert(static_pointer_cast<Key>(ObjectMng.GetSpecificObject(keyName))->GetUseIventID());

	//// 指定の鍵をフィールド上から削除
	//ObjectMng.DeleteObjectLate(keyName);

	EventMng.Notify(
		static_pointer_cast<Key>(ObjectMng.GetSpecificObject(getKeyObjectUniqueKey_))->GetGetIventID()
	);
	// 取得数を増加
	//getKeyCount_++;
	//getKeyName_.emplace_back(keyName);
}
