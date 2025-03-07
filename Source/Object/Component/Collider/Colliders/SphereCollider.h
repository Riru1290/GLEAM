#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

/// @brief ���̃R���C�_�[
class SphereCollider final : public Collider
{
public:

	[[nodiscard]] SphereCollider() = default;
	~SphereCollider() = default;

	/// @brief ���̏����Z�b�g
	/// @param info �T�[�N�����
	void SetSphereInfo(SPHERE_COL_INFO info)
	{
		sphere_ = info;
		sphere_.Init();
		PreUpdateCollider();
	}

	void SetPosition(Position3D pos);

	[[nodiscard]] SPHERE_COL_INFO& GetSphereInfo() { return sphere_; }
	[[nodiscard]] SPHERE_COL_INFO& GetPreSphereInfo() { return preSphere_; }

	/// @brief �R���C�_�[�̍X�V��Ǐ]�ΏۂɓK�p
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::SPHERE_COLLIDER; }

	/// @brief �R���C�_�[�̌`���擾
	/// @return �R���C�_�[�̌`
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::SPHERE; }

	/// @brief �X�V����
	void UpdateCollider() override;

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateCollider() override;

	/// @brief �`�揈��
	void DrawCollider() override;

	SPHERE_COL_INFO sphere_;			/// @brief ���̏��
	SPHERE_COL_INFO preSphere_;			/// @brief 1�t���[���O���̏��

};

