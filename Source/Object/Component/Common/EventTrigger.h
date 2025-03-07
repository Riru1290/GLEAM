#pragma once
#include "../Component.h"

/// @brief 設定された条件を満たした場合に設定された関数を実行する
class EventTrigger final : public Component
{
public:

	[[nodiscard]] EventTrigger();
	~EventTrigger() = default;



private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::EVENT_TRIGGER; }

	/// @brief 更新処理
	void UpdateComponent() override {}

	/// @brief 解放処理
	void ReleaseComponent() override {}

	
};

