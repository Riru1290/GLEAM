#include "../../ObjectManager.h"
#include "PadLockUnlocker.h"

PadLockUnlocker::PadLockUnlocker() : Component()
{
}

void PadLockUnlocker::UnLock() const
{
	// 指定の南京錠をフィールド上から削除
	ObjectMng.DeleteObjectLate(padLockObjectUniqueKey_);
}
