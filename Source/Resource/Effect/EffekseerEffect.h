#pragma once
#include "../Resource.h"
#include "../../Object/Effect/EffectController.h"

/// @brief Effekseer�G�t�F�N�g���\�[�X
class EffekseerEffect final : public Resource
{
public:

	[[nodiscard]] EffekseerEffect(const wstring& filename);
	~EffekseerEffect();

	/// @brief �G�t�F�N�g�{�̂��擾
	/// @return �G�t�F�N�g�{��
	[[nodiscard]] const Effekseer::EffectRef& GetEffect()const { return effect_; }

private:

	/// @brief �������
	void Release()override;

	Effekseer::EffectRef effect_;	/// @brief �G�t�F�N�g�{��
};

