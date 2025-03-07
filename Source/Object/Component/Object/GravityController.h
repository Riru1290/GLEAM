#pragma once
#include "../Component.h"

class Transform;

/// @brief �d�͂�K�p
class GravityController final : public Component
{
public:

	[[nodiscard]] GravityController();
	~GravityController() = default;

	/// @brief �d�͂�K�p������I�u�W�F�N�g���Z�b�g
	/// @param transform �d�͂�K�p������I�u�W�F�N�g
	void SetTransform(Transform& transform) { transform_ = transform; }

	// �d�͕ύX
	void ChangeGravity(float gravity) { gravity_ = gravity; }

	// �ő�d�͕ύX
	void ChangeMaxGravity(float maxGravity) { gravityMax_ = maxGravity; }

	// �d�͉����x�ύX
	void ChangeGravitySpeed(float gravitySpeed) { gravityAccel_ = gravitySpeed; }

	// �d�͎擾
	[[nodiscard]] const float& GetGravity()const { return gravity_; }

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::GRAVITY_CONTROLLER; }

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �������
	void ReleaseComponent() override;

	optional<reference_wrapper<Transform>> transform_;		/// @brief �d�͂�K�p������I�u�W�F�N�g

	float gravity_;			/// @brief �d��
	float gravityMax_;		/// @brief �ő�d��
	float gravityAccel_;	/// @brief �d�͉����x
	Vector3 gravityDir_;	/// @brief �d�͕���
};

