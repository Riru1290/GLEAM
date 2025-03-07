#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

/// @brief �{�b�N�X�R���C�_�[
class BoxCollider final : public Collider
{
public:

	[[nodiscard]] BoxCollider() = default;
	~BoxCollider() = default;

	/// @brief �{�b�N�X�����Z�b�g
	/// @param info �{�b�N�X���
	void SetBoxInfo(BOX_COL_INFO info)
	{
		box_ = info;
		box_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] BOX_COL_INFO& GetBoxInfo() { return box_; }
	[[nodiscard]] BOX_COL_INFO& GetPreBoxInfo() { return preBox_; }

	/// @brief �R���C�_�[�̍X�V��Ǐ]�ΏۂɓK�p
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::BOX_COLLIDER; }

	/// @brief �R���C�_�[�̌`���擾
	/// @return �R���C�_�[�̌`
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::BOX; }

	/// @brief �X�V����
	void UpdateCollider() override;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateCollider() override;

	/// @brief �`�揈��
	void DrawCollider() override;

	BOX_COL_INFO box_;			/// @brief �{�b�N�X���
	BOX_COL_INFO preBox_;		/// @brief 1�t���[���O�{�b�N�X���
};

