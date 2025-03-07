#include "Event/EventManager.h"
#include "GameProgressManager.h"

void GameProgressManager::Terminate()
{
}

void GameProgressManager::ResetGame()
{
	keyTriggeredEvents_.clear();
	detectedByEnemyNum_ = 0;
}

void GameProgressManager::ContinueGame()
{
	for (const auto& key : keyTriggeredEvents_)
	{
		EventMng.Notify(key);
	}
	EventMng.RegisterEventAction(EVENT_ID::GET_KEY_A,
		[this] {keyTriggeredEvents_.insert(EVENT_ID::GET_KEY_A); });
	EventMng.RegisterEventAction(EVENT_ID::GET_KEY_B,
		[this] {keyTriggeredEvents_.insert(EVENT_ID::GET_KEY_B); });
	EventMng.RegisterEventAction(EVENT_ID::GET_KEY_C,
		[this] {keyTriggeredEvents_.insert(EVENT_ID::GET_KEY_C); });
	EventMng.RegisterEventAction(EVENT_ID::DETECTED_BY_ENEMY,
		[this] {detectedByEnemyNum_++; });
	EventMng.RegisterEventAction(EVENT_ID::UNDETECTED_BY_ENEMY,
		[this] {
			detectedByEnemyNum_--;
			detectedByEnemyNum_ = max(0, detectedByEnemyNum_);
		});
	detectedByEnemyNum_ = 0;
}

GameProgressManager::GameProgressManager() : 
	detectedByEnemyNum_(0)
{
	EventMng.RegisterEventAction(EVENT_ID::GET_KEY_A,
		[this] {keyTriggeredEvents_.insert(EVENT_ID::GET_KEY_A); });
	EventMng.RegisterEventAction(EVENT_ID::GET_KEY_B,
		[this] {keyTriggeredEvents_.insert(EVENT_ID::GET_KEY_B); });
	EventMng.RegisterEventAction(EVENT_ID::GET_KEY_C,
		[this] {keyTriggeredEvents_.insert(EVENT_ID::GET_KEY_C); });
	EventMng.RegisterEventAction(EVENT_ID::DETECTED_BY_ENEMY,
		[this] {detectedByEnemyNum_++; });
	EventMng.RegisterEventAction(EVENT_ID::UNDETECTED_BY_ENEMY,
		[this] {
			detectedByEnemyNum_--;
			detectedByEnemyNum_ = max(0, detectedByEnemyNum_);
		});
}

GameProgressManager::~GameProgressManager()
{
}
