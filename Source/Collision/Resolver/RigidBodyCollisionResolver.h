#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "../CollisionHitData.h"

#define ColResolver RigidBodyCollisionResolver::GetInstance()

class CollisionResolveFunction;

/// @brief 衝突後処理を管理
class RigidBodyCollisionResolver final : public StaticSingleton<RigidBodyCollisionResolver>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 衝突時データを元に衝突後処理を行う
	/// @param data 衝突時データ
	void Resolve(int index, COL_HIT_DATA data);

private:

	RigidBodyCollisionResolver();
	~RigidBodyCollisionResolver();
	PROOF_OF_STATIC_SINGLETON(RigidBodyCollisionResolver);

	map<int, unique_ptr<CollisionResolveFunction>> resolveFunctions_;	/// @brief 衝突後判定
};

