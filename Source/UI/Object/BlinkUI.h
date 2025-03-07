#pragma once
#include "../../Graphics/Material/Material.h"
#include "../../Common/Mesh/Mesh.h"
#include "../UIObject.h"

class ModelBlinker;

class BlinkUI final : public UIObject
{
public:

	[[nodiscard]] BlinkUI();
	~BlinkUI() = default;

	void SetBlinkSpeed(float blinkSpeed);

private:


	/// @brief ��{�I�ȍX�V����
	void UpdateUI() override;

	/// @brief �e�N�X�`���ݒ��̏�����
	void InitAfterSetTexture() override;

	optional<reference_wrapper<ModelBlinker>> blinker_;
};

