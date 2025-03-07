#pragma once
#include "../CollisionResolveFunction.h"

/// @brief �V�����_�[�ƃ��f���̏Փˌ㏈��
class CylinderModelCollisionResolveFunction final : public CollisionResolveFunction
{
public:

	[[nodiscard]] CylinderModelCollisionResolveFunction() = default;
	~CylinderModelCollisionResolveFunction() = default;

	/// @brief �Փˎ��f�[�^�����ɏՓˌ㏈�����s��
	/// @param data �Փˎ��f�[�^
	void Resolve(COL_HIT_DATA data) override;

private:

	/// @brief �V�����_�[�����s����Ԃ̏ꍇ�̏Փˌ㏈��
	/// @param data �Փˎ��f�[�^
	void ResolveStaticCylinder(COL_HIT_DATA data);

	/// @brief ���f�������s����Ԃ̏ꍇ�̏Փˌ㏈��
	/// @param data �Փˎ��f�[�^
	void ResolveStaticModel(COL_HIT_DATA data);


};

