#include "../../ObjectManager.h"
#include "PadLockUnlocker.h"

PadLockUnlocker::PadLockUnlocker() : Component()
{
}

void PadLockUnlocker::UnLock() const
{
	// �w��̓싞�����t�B�[���h�ォ��폜
	ObjectMng.DeleteObjectLate(padLockObjectUniqueKey_);
}
