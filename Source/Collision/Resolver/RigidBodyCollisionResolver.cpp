#include "ResolveFunction/Functions/BoxCylinderCollisionResolveFunction.h"
#include "ResolveFunction/Functions/CylinderModelCollisionResolveFunction.h"
#include "RigidBodyCollisionResolver.h"

RigidBodyCollisionResolver::RigidBodyCollisionResolver()
{
	// �{�b�N�X�ƃV�����_�[
	int index = static_cast<int>(COLLISION_SHAPE_TYPE::BOX) + static_cast<int>(COLLISION_SHAPE_TYPE::CYLINDER);
	resolveFunctions_[index] = move(make_unique<BoxCylinderCollisionResolveFunction>());

	// �V�����_�[�ƃ��f��
	index = static_cast<int>(COLLISION_SHAPE_TYPE::CYLINDER) + static_cast<int>(COLLISION_SHAPE_TYPE::MODEL);
	resolveFunctions_[index] = move(make_unique<CylinderModelCollisionResolveFunction>());
}

RigidBodyCollisionResolver::~RigidBodyCollisionResolver()
{
}

void RigidBodyCollisionResolver::Terminate()
{
}

void RigidBodyCollisionResolver::Resolve(int index, COL_HIT_DATA data)
{
	if (!resolveFunctions_.contains(index))return;

	resolveFunctions_.at(index)->Resolve(data);
}


