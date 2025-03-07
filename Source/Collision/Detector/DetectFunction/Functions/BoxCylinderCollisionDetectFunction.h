#pragma once
#include "../CollisionDetectFunction.h"

/// @brief �{�b�N�X�ƃV�����_�[�̏Փ˔���
class BoxCylinderCollisionDetectFunction final : public CollisionDetectFunction
{
public:

	[[nodiscard]] BoxCylinderCollisionDetectFunction() = default;
	~BoxCylinderCollisionDetectFunction() = default;

	/// @brief �Փ˔���
	/// @param colA �R���C�_�[A
	/// @param colB �R���C�_�[B
	/// @param hitData �Փˎ��f�[�^
	/// @return �Փ˂��Ă��邩�ǂ���
	bool Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData) override;
};

