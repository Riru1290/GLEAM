#pragma once
#include "../Component.h"

/// @brief �ݒ肳�ꂽ�����𖞂������ꍇ�ɐݒ肳�ꂽ�֐������s����
class EventTrigger final : public Component
{
public:

	[[nodiscard]] EventTrigger();
	~EventTrigger() = default;



private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::EVENT_TRIGGER; }

	/// @brief �X�V����
	void UpdateComponent() override {}

	/// @brief �������
	void ReleaseComponent() override {}

	
};

