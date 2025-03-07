#pragma once
#include "../../Common/ConvertCollider.h"

/// @brief 衝突後処理関数オブジェクト基底クラス
class CollisionResolveFunction
{
public:

	CollisionResolveFunction() = default;
	virtual ~CollisionResolveFunction() = default;

	virtual void Resolve(COL_HIT_DATA data) = 0;
};

