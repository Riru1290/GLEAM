#pragma once
#include "../../Common/ConvertCollider.h"

/// @brief �Փˌ㏈���֐��I�u�W�F�N�g���N���X
class CollisionResolveFunction
{
public:

	CollisionResolveFunction() = default;
	virtual ~CollisionResolveFunction() = default;

	virtual void Resolve(COL_HIT_DATA data) = 0;
};

