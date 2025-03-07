#pragma once
#include "../CollisionDetectFunction.h"

/// @brief �V�����_�[�ƃ��f���̏Փ˔���
class CylinderModelCollisionDetectFunction final : public CollisionDetectFunction
{
public:

	[[nodiscard]] CylinderModelCollisionDetectFunction() = default;
	~CylinderModelCollisionDetectFunction() = default;

	/// @brief �Փ˔���
	/// @param colA �R���C�_�[A
	/// @param colB �R���C�_�[B
	/// @param hitData �Փˎ��f�[�^
	/// @return �Փ˂��Ă��邩�ǂ���
	bool Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData) override;

};

