#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

class LineCollider final : public Collider
{
public:

	[[nodiscard]] LineCollider() = default;
	~LineCollider() = default;

	/// @brief �V�����_�[�����Z�b�g
	/// @param info �V�����_�[���
	void SetLineInfo(LINE_COL_INFO info)
	{
		line_ = info;
		line_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] LINE_COL_INFO& GetLineInfo() { return line_; }
	[[nodiscard]] LINE_COL_INFO& GetPreLineInfo() { return preLine_; }

	/// @brief �R���C�_�[�̍X�V��Ǐ]�ΏۂɓK�p
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::LINE_COLLIDER; }

	/// @brief �R���C�_�[�̌`���擾
	/// @return �R���C�_�[�̌`
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::LINE; }

	/// @brief �X�V����
	void UpdateCollider() override;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateCollider() override;

	/// @brief �`�揈��
	void DrawCollider() override;

	LINE_COL_INFO line_;		/// @brief �������
	LINE_COL_INFO preLine_;		/// @brief 1�t���[���O�������
};

