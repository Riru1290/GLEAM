#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "../CollisionHitData.h"

#define ColResolver RigidBodyCollisionResolver::GetInstance()

class CollisionResolveFunction;

/// @brief �Փˌ㏈�����Ǘ�
class RigidBodyCollisionResolver final : public StaticSingleton<RigidBodyCollisionResolver>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief �Փˎ��f�[�^�����ɏՓˌ㏈�����s��
	/// @param data �Փˎ��f�[�^
	void Resolve(int index, COL_HIT_DATA data);

private:

	RigidBodyCollisionResolver();
	~RigidBodyCollisionResolver();
	PROOF_OF_STATIC_SINGLETON(RigidBodyCollisionResolver);

	map<int, unique_ptr<CollisionResolveFunction>> resolveFunctions_;	/// @brief �Փˌ㔻��
};

