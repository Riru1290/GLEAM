#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define GameDifficulty GameDifficultyController::GetInstance()

/// @brief �Q�[����Փx�ݒ�
class GameDifficultyController final : public StaticSingleton<GameDifficultyController>
{
public:

	/// @brief �������
	void Terminate() override;

	[[nodiscard]] const int& GetLevel()const { return level_; }

	void SetLevel(int level) { level_ = level; }

private:

	GameDifficultyController();
	~GameDifficultyController();
	PROOF_OF_STATIC_SINGLETON(GameDifficultyController);

	int level_;	/// @brief ��Փx
};

