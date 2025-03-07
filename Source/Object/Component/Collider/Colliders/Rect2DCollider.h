#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

/// @brief ��`(2D)�R���C�_�[
class Rect2DCollider final : public Collider
{
public:

	[[nodiscard]] Rect2DCollider() = default;
	~Rect2DCollider() = default;

	/// @brief ��`(2D)�����Z�b�g
	/// @param info ��`(2D)���
	void SetRectInfo(RECT_COL_INFO info)
	{
		rect_ = info;
		rect_.Init();
		PreUpdateCollider();
	}

	[[nodiscard]] RECT_COL_INFO& GetRectInfo() { return rect_; }
	[[nodiscard]] RECT_COL_INFO& GetPreRectInfo() { return preRect_; }

	/// @brief �R���C�_�[�̍X�V��Ǐ]�ΏۂɓK�p
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::RECT2D_COLLIDER; }

	/// @brief �R���C�_�[�̌`���擾
	/// @return �R���C�_�[�̌`
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::RECT; }

	/// @brief �X�V����
	void UpdateCollider() override;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateCollider() override;

	/// @brief �`�揈��
	void DrawCollider() override;

	RECT_COL_INFO rect_;			/// @brief ��`(2D)���
	RECT_COL_INFO preRect_;		/// @brief 1�t���[���O��`(2D)���
};

