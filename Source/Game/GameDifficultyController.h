#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define GameDifficulty GameDifficultyController::GetInstance()

/// @brief ƒQ[ƒ€“ïˆÕ“xİ’è
class GameDifficultyController final : public StaticSingleton<GameDifficultyController>
{
public:

	/// @brief ‰ğ•úˆ—
	void Terminate() override;

	[[nodiscard]] const int& GetLevel()const { return level_; }

	void SetLevel(int level) { level_ = level; }

private:

	GameDifficultyController();
	~GameDifficultyController();
	PROOF_OF_STATIC_SINGLETON(GameDifficultyController);

	int level_;	/// @brief “ïˆÕ“x
};

