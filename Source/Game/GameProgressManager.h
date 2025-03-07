#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "Event/EventID.h"

#define GameProgress GameProgressManager::GetInstance()

/// @brief �Q�[���i�s�󋵊Ǘ�
class GameProgressManager final : public StaticSingleton<GameProgressManager>
{
public:

	/// @brief �������
	void Terminate() override;

	void ResetGame();

	void ContinueGame();

	void ResetDetectedByEnemyNum() { detectedByEnemyNum_ = 0; }

	[[nodiscard]] bool IsDetectedByEnemy()const { return (detectedByEnemyNum_ > 0); }

private:

	GameProgressManager();
	~GameProgressManager();
	PROOF_OF_STATIC_SINGLETON(GameProgressManager);

	set<EVENT_ID> keyTriggeredEvents_;

	int detectedByEnemyNum_;
};

