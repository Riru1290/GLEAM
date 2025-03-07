#pragma once
#include "../Component.h"
#include "../../../Utility/Math/Quaternion.h"

class Transform;
class AnimationModelRenderer;

/// @brief �ݒ肳�ꂽ�I�u�W�F�N�g�̎w��{�[������]������
class RotateBoneController final : public Component
{
public:

	[[nodiscard]] RotateBoneController();
	~RotateBoneController() = default;

	/// @brief ���f�������_���[�ݒ�
	/// @param transform ���f�������_���[
	void SetAnimationModelRenderer(AnimationModelRenderer& modelRenderer)
	{
		modelRenderer_ = modelRenderer;
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

	/// @brief ��]����ݒ�
	/// @param min �ŏ��l
	/// @param max �ő�l
	void SetRotateLimit(Vector3 min, Vector3 max, bool isLimitX, bool isLimitY, bool isLimitZ)
	{
		rotateMin_ = min;
		rotateMax_ = max;
		isLimitRotateX_ = isLimitX;
		isLimitRotateY_ = isLimitY;
		isLimitRotateZ_ = isLimitZ;
	}

	/// @brief ��]�����Z�b�g
	void ResetRotate();

	/// @brief ��]���Z�b�g
	/// @param rotate ��]
	void SetRotate(Quaternion rotate);

	/// @brief ��]������{�[���ԍ���ݒ�
	/// @param no ��]������{�[���ԍ�
	void SetBoneNo(int no)
	{
		boneNo_ = no;
	}

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ROTATEBONE_CONTROLLER; }

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �������
	void ReleaseComponent()override;

	Quaternion boneRot_;

	int boneNo_;
	optional<reference_wrapper<AnimationModelRenderer>> modelRenderer_;		/// @brief �A�j���[�V�������f�������_���[

	Vector3 rotatePowMouseX_;		/// @brief �}�E�XX���ɑΉ�������]��
	Vector3 rotatePowMouseY_;		/// @brief �}�E�XY���ɑΉ�������]��

	Vector3 rotateMin_;				/// @brief ��]�ʐ����ŏ��l
	Vector3 rotateMax_;				/// @brief ��]�ʐ����ő�l

	bool isLimitRotateX_;
	bool isLimitRotateY_;
	bool isLimitRotateZ_;

};

