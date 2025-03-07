#pragma once
#include "../Component.h"
#include "../../../Utility/Math/Quaternion.h"

class Transform;

/// @brief �ݒ肳�ꂽ�I�u�W�F�N�g���w��̉�]�l�܂ł�������]������
class SmoothRotateController final : public Component
{
public:

	[[nodiscard]] SmoothRotateController();
	~SmoothRotateController() = default;

	/// @brief �g�����X�t�H�[���ݒ�
	/// @param transform �g�����X�t�H�[��
	void SetTransform(Transform& transform);

	/// @brief ��]�����Œ肷�邩��ݒ�
	/// @param fixedState ��]�����Œ肷�邩
	void SetFixedState(bool fixedState) { isFixedRotate_ = fixedState; }

	/// @brief �ڕW��]�l��ݒ�
	/// @param goalQua �ڕW��]�l
	void SetGoalQuaternion(Quaternion goalQua)
	{
		isGoal_ = false;
		goalQua_ = goalQua;
	}

	void SetRelativePosition(Position3D relativePos)
	{
		relativePos_ = relativePos;
	}

	void SetRotatePow(float pow)
	{
		rotPow_ = pow;
	}

	/// @brief �ڕW�l�ɓ��B���Ă��邩
	/// @return �ڕW�l�ɓ��B���Ă��邩
	[[nodiscard]] const bool& IsGoal()const { return isGoal_; }

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::SMOOTHROTATE_CONTROLLER; }

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �������
	void ReleaseComponent()override {}

	optional<reference_wrapper<Transform>> transform_;						/// @brief �g�����X�t�H�[�����

	Quaternion goalQua_;

	Position3D relativePos_;

	Position3D standardPos_;

	float rotPow_;

	bool isGoal_;

	bool isFixedRotate_;
};

