#include "EventManager.h"


EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void EventManager::Terminate()
{
	eventActions_.clear();
}

void EventManager::Execute()
{
}

void EventManager::ResetEventActions()
{
	eventActions_.clear();
}

void EventManager::Notify(EVENT_ID id)
{
	if (eventActions_.contains(id))
	{
		// 設定されたアクションを実行
		for (const auto& action : eventActions_.at(id))
		{
			action();
		}
	}


	if (fixedEventActions_.contains(id))
	{
		// 設定されたアクションを実行
		for (const auto& action : fixedEventActions_.at(id))
		{
			action();
		}
	}
}

void EventManager::RegisterEventAction(EVENT_ID id, function<void()> func)
{
	eventActions_[id].emplace_back(func);
}

void EventManager::RegisterFixedEventAction(EVENT_ID id, function<void()> func)
{
	fixedEventActions_[id].emplace_back(func);
}

