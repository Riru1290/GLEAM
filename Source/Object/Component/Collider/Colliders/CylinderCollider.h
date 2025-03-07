#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

/// @brief �V�����_�[�R���C�_�[
class CylinderCollider final : public Collider
{
public:

	[[nodiscard]] CylinderCollider() = default;
	~CylinderCollider() = default;

	/// @brief �V�����_�[�����Z�b�g
	/// @param info �V�����_�[���
	void SetCylinderInfo(CYLINDER_COL_INFO info)
	{
		cylinder_ = info;
		cylinder_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] CYLINDER_COL_INFO& GetCylinderInfo() { return cylinder_; }
	[[nodiscard]] CYLINDER_COL_INFO& GetPreCylinderInfo() { return preCylinder_; }

	/// @brief �R���C�_�[�̍X�V��Ǐ]�ΏۂɓK�p
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::CYLINDER_COLLIDER; }

	/// @brief �R���C�_�[�̌`���擾
	/// @return �R���C�_�[�̌`
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::CYLINDER; }

	/// @brief �X�V����
	void UpdateCollider() override;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateCollider() override;

	/// @brief �`�揈��
	void DrawCollider() override;

	CYLINDER_COL_INFO cylinder_;		/// @brief �V�����_�[���
	CYLINDER_COL_INFO preCylinder_;		/// @brief 1�t���[���O�V�����_�[���

};

