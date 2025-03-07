#include "Event/EventManager.h"
#include "ClearChecker.h"

ClearChecker::ClearChecker()
{
	EventMng.RegisterFixedEventAction(EVENT_ID::OPEN_DOOR,
		[this] {isClear_ = true; });
	EventMng.RegisterFixedEventAction(EVENT_ID::DEAD_PLAYER,
		[this] {isClear_ = false; });
}
