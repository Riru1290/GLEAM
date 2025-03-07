#pragma once
#include "../CollisionHitData.h"

class CollisionDetectFunction;

#define ColDetector CollisionDetector::GetInstance()

/// @brief �Փ˔�����s��
class CollisionDetector final : public StaticSingleton<CollisionDetector>
{
public:

	/// @brief �������
	void Terminate() override {};

	/// @brief �Փ˔���
	/// @param index 2�̃R���C�_�[�̌`�̑g�ݍ��킹�ɂ���ĕς��C���f�b�N�X
	/// @param colA �R���C�_�[A
	/// @param colB �R���C�_�[B
	/// @param hitData �Փˎ��f�[�^
	/// @return �Փ˂��Ă��邩�ǂ���
	[[nodiscard]] bool Detection(int index,
		weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData);

protected:

	CollisionDetector();
	~CollisionDetector();
	PROOF_OF_STATIC_SINGLETON(CollisionDetector);
	
	map<int, unique_ptr<CollisionDetectFunction>> detectFunctions_;		/// @brief �Փ˔���

};

