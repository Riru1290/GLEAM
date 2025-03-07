#pragma once
#include "../Component.h"

class ModelRenderer;
class AnimationModelRenderer;
class Object2DRenderer;
class MeshRenderer;

/// @brief �Ώۂ�_�ł�����
class ModelBlinker final : public Component
{
public:

	[[nodiscard]] ModelBlinker();
	~ModelBlinker() = default;

	void SetBlinkSpeed(float blinkSpeed)
	{
		blinkSpeed_ = blinkSpeed;
	}

	[[nodiscard]] const float& GetRate()const { return rate_; }

	/// @brief �t�F�[�h�����郌���_���[���Z�b�g
	/// @param renderer �����_���[
	void SetFadeRenderer(ModelRenderer& renderer);

	/// @brief �t�F�[�h�����郌���_���[���Z�b�g
	/// @param renderer �����_���[
	void SetFadeRenderer(AnimationModelRenderer& renderer);

	/// @brief �t�F�[�h�����郌���_���[���Z�b�g
	/// @param renderer �����_���[
	void SetFadeRenderer(Object2DRenderer& renderer);

	/// @brief �t�F�[�h�����郌���_���[���Z�b�g
	/// @param renderer �����_���[
	void SetFadeRenderer(MeshRenderer& renderer);

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::MODEL_BLINKER; }

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �������
	void ReleaseComponent() override {}

	float rate_;

	float timeCount_;

	float fadeTime_;

	float blinkSpeed_ = 1.0f;

	optional<reference_wrapper<ModelRenderer>> modelRenderer_;
	optional<reference_wrapper<AnimationModelRenderer>> animModelRenderer_;
	optional<reference_wrapper<Object2DRenderer>> object2DRenderer_;
	optional<reference_wrapper<MeshRenderer>> meshRenderer_;
};

