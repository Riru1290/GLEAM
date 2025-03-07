#pragma once
#include "../Actor.h"
#include "../../Component/Actor/State/StateMachine.h"

class InputController;
class GravityController;
class LineCollider;
class CylinderCollider;
class SphereCollider;
class LightBulbPlacer;
class KeyInventory;
class PadLockUnlocker;
class RotateBoneController;
class FlashLightHolder;

struct COL_HIT_DATA;

/// @brief �v���C���[(����Ώ�)
class Player final : public Actor
{
public:

	[[nodiscard]] Player(const string& uniqueKey);
	~Player();

	[[nodiscard]] const Transform& GetHeadTransform()const { return *camera_; }
	[[nodiscard]] const Transform& GetHeadSubTransform()const { return *cameraSub_; }


	/// @brief ���
	enum class STATE
	{
		DEFAULT,
		HIDE,
		DEAD,
	};


private:

	/// @brief ���s�\�ȃA�N�V����
	enum class AVAILABLE_ACTION
	{
		NONE,
		PLACE_LIGHT,
		GET_LIGHT,
		GET_KEY,
		UNLOCK_PADLOCK,
		HIDE,
	};

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::PLAYER; }

	/// @brief �J��Ԃ�����������
	void InitActor() override;

	/// @brief �`�揈��
	void DrawActor() override;

	/// @brief ��{�I�ȍX�V����
	void UpdateActor() override;

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdateActor() override;

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdateActor() override;

	/// @brief �Փ˔����ɑ��鏈���ݒ�
	void OnCollisionUpdateSetting() override;

	/// @brief �A�j���[�V�����ݒ�
	void AnimationSetting() override;

	/// @brief �X�e�[�g�ݒ�
	void StateSetting();

	/// @brief ���͏��ݒ�
	void InputSetting();

	/// @brief �R���C�_�[�ݒ�
	void ColliderSetting();

	/// @brief �d�͐ݒ�
	void GravitySetting();

	/// @brief �J�����Ǐ]�ΏۍX�V
	void UpdateCameraTarget();

	// �X�e�[�g�ݒ�
	void DefaultStateSetting();
	void HideStateSetting();
	void DeadStateSetting();

	// �Փˌ㏈��
	void OnHitMap(COL_HIT_DATA data);
	void OnHitEnemy(COL_HIT_DATA data);
	void OnHitEyeMap(COL_HIT_DATA data);
	void OnHitEyeKey(COL_HIT_DATA data);
	void OnHitEyeLocker(COL_HIT_DATA data);
	void OnHitEyePadLock(COL_HIT_DATA data);
	void OnHitEyeLightBulb(COL_HIT_DATA data);

	optional<reference_wrapper<InputController>> input_;				/// @brief ���̓R���g���[��
	optional<reference_wrapper<RotateBoneController>> rotateBoneCon_;	/// @brief ���̓R���g���[��
	optional<reference_wrapper<GravityController>> gravity_;			/// @brief �d�̓R���g���[��
	optional<reference_wrapper<StateMachine<STATE>>> stateMachine_;		/// @brief �X�e�[�g�}�V��
	optional<reference_wrapper<CylinderCollider>> bodyCollider_;		/// @brief �̃R���C�_�[
	optional<reference_wrapper<LineCollider>> eyeCollider_;				/// @brief �ڐ��R���C�_�[
	optional<reference_wrapper<LineCollider>> eyeLongCollider_;			/// @brief �ڐ��R���C�_�[����
	optional<reference_wrapper<SphereCollider>> pointCollider_;			/// @brief �G�l�~�[��T���p�R���C�_�[
	optional<reference_wrapper<LightBulbPlacer>> lightBulbPlacer_;		/// @brief �d���ݒu
	optional<reference_wrapper<KeyInventory>> keyInventory_;			/// @brief �������Ǘ�
	optional<reference_wrapper<PadLockUnlocker>> padLockUnlocker_;		/// @brief �싞�����Ǘ�
	optional<reference_wrapper<FlashLightHolder>> flashLight_;			/// @brief �����d��

	unique_ptr<Transform> head_;		/// @brief ����]�p�g�����X�t�H�[��
	unique_ptr<Transform> camera_;		/// @brief �J�����Ǐ]�ʒu
	unique_ptr<Transform> cameraSub_;	/// @brief �J�����Ǐ]�ʒu

	unique_ptr<Transform> eyeA_;		/// @brief �ڐ���
	unique_ptr<Transform> eyeB_;		/// @brief �ڐ���

	unique_ptr<Transform> eyeLongA_;	/// @brief �ڐ���(�G�l�~�[�ώ@�p)
	unique_ptr<Transform> eyeLongB_;	/// @brief �ڐ���(�G�l�~�[�ώ@�p)

	Position3D neaEyeHitPos_;			/// @brief �ł��߂��Փˍ��W(�ڐ��R���C�_�[)

	Position3D lockerPos_;				/// @brief �ڐ������������b�J�[���W
	Position3D preLockerPos_;			/// @brief ���b�J�[�ɓ���O�̍��W

	Quaternion lockerQua_;				/// @brief  �ڐ������������b�J�[��]

	AVAILABLE_ACTION availableAction_;		/// @brief ���ݗL���ȃA�N�V����
	AVAILABLE_ACTION preAvailableAction_;	/// @brief 1�t���[���O�ɗL���������A�N�V����

	// ���ʉ��n���h��
	int walkSoundHandle_;
	int runSoundHandle_;
	int hideSoundHandle_;
};

