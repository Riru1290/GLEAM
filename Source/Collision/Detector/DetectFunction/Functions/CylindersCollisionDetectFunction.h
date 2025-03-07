#pragma once
#include "../CollisionDetectFunction.h"

/// @brief �V�����_�[���m�̏Փ˔���
class CylindersCollisionDetectFunction final : public CollisionDetectFunction
{
public:

	[[nodiscard]] CylindersCollisionDetectFunction() = default;
	~CylindersCollisionDetectFunction() = default;

	/// @brief �Փ˔���
	/// @param colA �R���C�_�[A
	/// @param colB �R���C�_�[B
	/// @param hitData �Փˎ��f�[�^
	/// @return �Փ˂��Ă��邩�ǂ���
	bool Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData) override;
};

