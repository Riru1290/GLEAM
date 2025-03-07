#pragma once
#include "../Component.h"

class Transform;

/// @brief �ݒ肳�ꂽ�I�u�W�F�N�g����]������
class RotateController final : public Component
{
public:

	[[nodiscard]] RotateController();
	~RotateController() = default;

	/// @brief �g�����X�t�H�[���ݒ�
	/// @param transform �g�����X�t�H�[��
	void SetTransform(Transform& transform)
	{
		transform_ = transform;
	}

	/// @brief �}�E�X��X���ړ��ɑΉ�������]�ʂ�ݒ�
	/// @param rotatePowMouseX �}�E�X��X���ړ��ɑΉ�������]��
	void SetRotatePowMouseX(Vector3 rotatePowMouseX)
	{
		rotatePowMouseX_ = rotatePowMouseX;
	}

	/// @brief �}�E�X��Y���ړ��ɑΉ�������]�ʂ�ݒ�
	/// @param rotatePowMouseY �}�E�X��Y���ړ��ɑΉ�������]��
	void SetRotatePowMouseY(Vector3 rotatePowMouseY)
	{
		rotatePowMouseY_ = rotatePowMouseY;
	}

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ROTATE_CONTROLLER; }

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �������
	void ReleaseComponent()override;

	optional<reference_wrapper<Transform>> transform_;						/// @brief �g�����X�t�H�[�����

	Vector3 rotatePowMouseX_;		/// @brief �}�E�XX���ɑΉ�������]��
	Vector3 rotatePowMouseY_;		/// @brief �}�E�XY���ɑΉ�������]��


};

