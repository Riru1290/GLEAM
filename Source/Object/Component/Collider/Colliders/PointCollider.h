#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

/// @brief �_�R���C�_�[
class PointCollider final : public Collider
{
public:

	[[nodiscard]] PointCollider() = default;
	~PointCollider() = default;

	/// @brief �_�����Z�b�g
	/// @param info �_���
	void SetPointInfo(POINT_COL_INFO info)
	{
		point_ = info;
		point_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] POINT_COL_INFO& GetPointInfo() { return point_; }
	[[nodiscard]] POINT_COL_INFO& GetPrePointInfo() { return prePoint_; }

	/// @brief �R���C�_�[�̍X�V��Ǐ]�ΏۂɓK�p
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::BOX_COLLIDER; }

	/// @brief �R���C�_�[�̌`���擾
	/// @return �R���C�_�[�̌`
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::POINT; }

	/// @brief �X�V����
	void UpdateCollider() override;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateCollider() override;

	/// @brief �`�揈��
	void DrawCollider() override;

	POINT_COL_INFO point_;			/// @brief �_���
	POINT_COL_INFO prePoint_;		/// @brief 1�t���[���O�_���
};

