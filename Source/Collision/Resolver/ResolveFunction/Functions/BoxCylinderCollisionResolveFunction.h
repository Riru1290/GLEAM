#pragma once
#include "../CollisionResolveFunction.h"

/// @brief �{�b�N�X�ƃV�����_�[�̏Փˌ㏈��
class BoxCylinderCollisionResolveFunction final : public CollisionResolveFunction
{
public:

	[[nodiscard]] BoxCylinderCollisionResolveFunction() = default;
	~BoxCylinderCollisionResolveFunction() = default;

	/// @brief �Փˎ��f�[�^�����ɏՓˌ㏈�����s��
	/// @param data �Փˎ��f�[�^
	void Resolve(COL_HIT_DATA data) override;

private:

	/// @brief �{�b�N�X�����s����Ԃ̏ꍇ�̏Փˌ㏈��
	/// @param data �Փˎ��f�[�^
	void ResolveStaticBox(COL_HIT_DATA data);

	/// @brief �V�����_�[�����s����Ԃ̏ꍇ�̏Փˌ㏈��
	/// @param data �Փˎ��f�[�^
	void ResolveStaticCylinder(COL_HIT_DATA data);


};

