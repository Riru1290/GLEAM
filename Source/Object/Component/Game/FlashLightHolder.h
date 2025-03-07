#pragma once
#include "../Component.h"
#include "../../../Graphics/Light/SpotLight.h"


class Transform;
class AnimationModelRenderer;

/// @brief �����d������������
class FlashLightHolder final : public Component
{
public:

	[[nodiscard]] FlashLightHolder();
	~FlashLightHolder();

	/// @brief ���f�������_���[�ݒ�
	/// @param transform ���f�������_���[
	void SetFollowerTransform(const Transform& follower)
	{
		transform_ = follower;
	}

	void SetGoalSpotLightColor(Vector3 color,float time);

	void SetRelativePos(Position3D relativePos,bool isLarp) {
		if (isLarp) {
			startPos_ = relativePos;
		}
		else
		{
			relativePos_ = relativePos;
			startPos_ = relativePos_;
		}
	}

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::FLASHLIGHT_HOLDER; }

	/// @brief �X�V����
	void UpdateComponent() override {}

	/// @brief �������
	void ReleaseComponent()override {}

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdateComponent() override;


	optional<reference_wrapper<const Transform>> transform_;		/// @brief �A�j���[�V�������f�������_���[

	SPOT_LIGHT light_;

	Vector3 goalColor_;
	Vector3 nowColor_;

	Position3D relativePos_;
	Position3D startPos_;
};

