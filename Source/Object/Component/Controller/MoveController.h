#pragma once
#include "../Component.h"

class Transform;

/// @brief �ݒ肳�ꂽ�I�u�W�F�N�g���ړ�������
class MoveController final : public Component
{
public:

	[[nodiscard]] MoveController();
	~MoveController() = default;

	/// @brief �g�����X�t�H�[���ݒ�
	/// @param transform �g�����X�t�H�[��
	void SetTransform(Transform& transform)
	{
		transform_ = transform;
	}

	/// @brief �~�܂����ۂ̃A�N�V�����ݒ�
	/// @param action �A�N�V����
	void SetIdleAction(function<void()> action)
	{
		IdleAction_ = action;
	}

	/// @brief �������ۂ̃A�N�V�����ݒ�
	/// @param action �A�N�V����
	void SetWalkAction(function<void()> action)
	{
		WalkAction_ = action;
	}

	/// @brief �������ۂ̃A�N�V�����ݒ�
	/// @param action �A�N�V����
	void SetRunAction(function<void()> action)
	{
		RunAction_ = action;
	}

	/// @brief ���������ݒ�
	/// @param speed ��������
	void SetWalkSpeed(float speed)
	{
		walkSpeed_ = speed;
	}

	/// @brief ���鑬���ݒ�
	/// @param speed ���鑬��
	void SetRunSpeed(float speed)
	{
		runSpeed_ = speed;
	}

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::MOVE_CONTROLLER; }

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �������
	void ReleaseComponent()override;

	optional<reference_wrapper<Transform>> transform_;						/// @brief �g�����X�t�H�[�����

	function<void()> IdleAction_;		/// @brief �~�܂����ۂ̒ǉ��A�N�V����
	function<void()> WalkAction_;		/// @brief �������ۂ̒ǉ��A�N�V����
	function<void()> RunAction_;		/// @brief �������ۂ̒ǉ��A�N�V����

	float walkSpeed_;					/// @brief ��������
	float runSpeed_;					/// @brief ���鑬��
};

