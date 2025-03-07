#pragma once
#include "../CollisionDetectFunction.h"

/// @brief ���̓��m�̏Փ˔���
class SpheresCollisionDetectFunction final : public CollisionDetectFunction
{
public:

	[[nodiscard]] SpheresCollisionDetectFunction() = default;
	~SpheresCollisionDetectFunction() = default;

	/// @brief �Փ˔���
	/// @param colA �R���C�_�[A
	/// @param colB �R���C�_�[B
	/// @param hitData �Փˎ��f�[�^
	/// @return �Փ˂��Ă��邩�ǂ���
	bool Detection(weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData) override;
};

