#pragma once
#include "../../Component/Actor/State/StateMachine.h"
#include "../Actor.h"

class AStarChaser;
class RouteWalker;
class SmoothRotateController;

/// @brief �K�[���G�l�~�[
class GirlEnemy final : public Actor
{
public:

	[[nodiscard]] GirlEnemy(const string& uniqueKey);
	virtual ~GirlEnemy() = default;

	/// @brief ���S���J�����pTransform�擾
	/// @return ���S���J�����pTransform
	[[nodiscard]] const Transform& GetHeadTransform()const { return *head_; }

	/// @brief ���
	enum class STATE
	{
		DEFAULT,
		CHASE,
		RETURN,
		SCREAM,
		ATTACK,
		STOP,
	};

private:


	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::ENEMY; }

	/// @brief �`�揈��
	void DrawActor() override;

	/// @brief ��{�I�ȍX�V����
	void UpdateActor() override;

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdateActor() override;

	/// @brief �Փ˔����ɑ��鏈���ݒ�
	void OnCollisionUpdateSetting() override;

	/// @brief �A�j���[�V�����ݒ�
	void AnimationSetting() override;

	/// @brief �X�e�[�g�ݒ�
	void StateSetting();

	/// @brief �R���C�_�[�ݒ�
	void ColliderSetting();

	/// @brief �J�����Ǐ]�ΏۍX�V
	void UpdateCameraTarget();

	// �X�e�[�g�ݒ�
	
	/// @brief �f�t�H���g�X�e�[�g�ݒ�
	void DefaultStateSetting();

	/// @brief �Ǐ]�X�e�[�g�ݒ�
	void ChaseStateSetting();

	/// @brief �A��X�e�[�g�ݒ�
	void ReturnStateSetting();

	/// @brief ���уX�e�[�g�ݒ�
	void ScreamStateSetting();

	/// @brief �U���X�e�[�g�ݒ�
	void AttackStateSetting();

	/// @brief �X�g�b�v�X�e�[�g�ݒ�
	void StopStateSetting();

	// �Փˌ㏈��

	/// @brief �v���C���[�Ƃ̏Փˌ㏈��
	/// @param data �Փˎ��f�[�^
	void OnHitPlayer(COL_HIT_DATA data);

	/// @brief �v���C���[�_�Ƃ̏Փˌ㏈��
	/// @param data �Փˎ��f�[�^
	void OnHitPlayerPoint(COL_HIT_DATA data);

	unique_ptr<Transform> head_;		/// @brief �J�����p

	optional<reference_wrapper<AStarChaser>> chaser_;				/// @brief �o�H�T��
	optional<reference_wrapper<AStarChaser>> returnChaser_;			/// @brief ���̌o�H�ɋA�邽�߂�A*
	optional<reference_wrapper<RouteWalker>> walker_;				/// @brief ����
	optional<reference_wrapper<SmoothRotateController>> rotateCon_;	/// @brief ��]
	optional<reference_wrapper<StateMachine<STATE>>> stateMachine_;	/// @brief �X�e�[�g�}�V��

	Transform returnPos_;	/// @brief �A����W


	bool isFoundPlayer_;	/// @brief �v���C���[�𔭌����Ă��邩
	bool isHitPlayer_;		/// @brief �v���C���[�ɏՓ˂�����
};

