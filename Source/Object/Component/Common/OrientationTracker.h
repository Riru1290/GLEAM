#pragma once
#include "../Component.h"

class Transform;
class SmoothRotateController;

/// @brief �w��I�u�W�F�N�g�̕���������������
class OrientationTracker final : public Component
{
public:

	[[nodiscard]] OrientationTracker();
	~OrientationTracker() = default;

	/// @brief �g�����X�t�H�[���ݒ�
	/// @param transform �g�����X�t�H�[��
	void SetTransform(Transform& transform);


	/// @brief �Ǐ]�Ώېݒ�
	/// @param target �Ǐ]�Ώ�
	/// @param rotateOnlyOnY Y����]�݂̂��s����
	void SetTargetTransform(const Transform& target, bool rotateOnlyOnY = false);


private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ORIENTATION_TRACKER; }

	/// @brief �X�V����
	void UpdateComponent() override {}

	/// @brief �X�V����
	void LateUpdateComponent() override;

	/// @brief �������
	void ReleaseComponent() override {}

	/// @brief �����Ǐ]
	void Track();

	optional<reference_wrapper<Transform>> transform_;	/// @brief ��I�u�W�F�N�g
	optional<reference_wrapper<const Transform>> target_;		/// @brief �Ǐ]�Ώ�

	unique_ptr<SmoothRotateController> rotateCon_;				/// @brief ��]�R���g���[��

	bool rotateOnlyOnY_;	/// @brief Y����]�݂̂��s����
};

