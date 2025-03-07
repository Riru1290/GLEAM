#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

class ModelCollider final : public Collider
{
public:

	[[nodiscard]] ModelCollider() = default;
	~ModelCollider() = default;

	/// @brief ���f�������Z�b�g
	/// @param info ���f�����
	void SetModelInfo(MODEL_COL_INFO info)
	{
		model_ = info;
		model_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] MODEL_COL_INFO& GetModelInfo() { return model_; }
	[[nodiscard]] MODEL_COL_INFO& GetPreModelInfo() { return preModel_; }

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::MODEL_COLLIDER; }

	/// @brief �R���C�_�[�̌`���擾
	/// @return �R���C�_�[�̌`
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::MODEL; }

	/// @brief �X�V����
	void UpdateCollider() override;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateCollider() override;

	/// @brief �R���C�_�[�̍X�V��Ǐ]�ΏۂɓK�p
	void ApplyColliderUpdateToFollower() override;

	MODEL_COL_INFO model_;			/// @brief ���f�����
	MODEL_COL_INFO preModel_;		/// @brief 1�t���[���O���f�����
};

