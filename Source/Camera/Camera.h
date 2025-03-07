#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Utility/Math/Quaternion.h"
#include "../Common/AABB/Frustum.h"
#include "../Common/AABB/BoundingSphere.h"
#include "../Object/Component/Common/Transform.h"

#define MainCamera Camera::GetInstance()

/// @brief �J����
class Camera final : public StaticSingleton<Camera>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief �J�������
	enum class STATE
	{
		FREE = 0,	// �t���[�J����
		FOLLOW,		// �Ǐ]
		RATE_FOLLOW,// �����Ǐ]
		FIXED,		// �Œ�
		LERP,		// ���[�v
		LERP_FOLLOW,// �Ǐ]���[�v
	};


	/// @brief �J������Ԑ؂�ւ�
	/// @param state �؂�ւ����
	/// @param isReset �J�����������������邩
	void ChangeState(STATE state, bool isReset = false);

	/// @brief �X�V����
	void Update();

	/// @brief �J���������V�F�[�_�ɃZ�b�g
	void SetBeforeDraw();

	/// @brief �J�������W�Z�b�g
	/// @param pos �J�������W
	void SetPosition(Position3D pos) { pos_ = pos; }

	/// @brief �J������]�Z�b�g
	/// @param pos �J������]
	void SetRotate(Quaternion rot);

	/// @brief �Ǐ]�ΏۃZ�b�g
	/// @param follower �Ǐ]�Ώ�
	void SetFollower(const Transform& follower) { follower_ = follower; }

	/// @brief �Ǐ]�����΍��W���Z�b�g
	/// @param relativePos �Ǐ]�����΍��W
	void SetFollowRelativePos(Position3D relativePos) { followRelativePos_ = relativePos; }

	/// @brief �Ǐ]�����Z�b�g
	/// @param rate �Ǐ]����(0.0f�`1.0f)
	void SetFollowRate(const float& rate) { followRate_ = rate; }

	/// @brief �Ǐ]�Ώ�(�T�u)�Z�b�g
	/// @param follower �Ǐ]�Ώ�(�T�u)
	void SetSubFollower(const Transform& follower) { subFollower_ = follower; }

	/// @brief �Ǐ](�T�u)�����΍��W���Z�b�g
	/// @param relativePos �Ǐ](�T�u)�����΍��W
	void SetSubFollowRelativePos(Position3D relativePos) { subFollowRelativePos_ = relativePos; }

	/// @brief ���[�v���ݒ�
	/// @param before �J�n�J�����ݒ�
	/// @param after  �I���J�����ݒ�
	/// @param time ���[�v����
	void SetLerpConfig(const Transform& before, const Transform& after, float time);

	/// @brief ���[�v���ݒ�
	/// @param after  �I���J�����ݒ�
	/// @param time ���[�v����
	void SetLerpConfig(const Transform& after, float time);

	/// @brief �Ǐ]���[�v���ݒ�
	/// @param after  �I���J�����ݒ�
	/// @param time ���[�v����
	void SetLerpFollowConfig(const Transform& after, float time);


	/// @brief ��ʗh��
	/// @param time �h�炷����
	/// @param pow �h���傫��
	/// @param speed �h��鑬��
	/// @param isPriority �ŗD�悳���邩
	void Shake(float time, Vector3 pow, float speed, bool isPriority = false);

	/// @brief �J�����r���[�v���W�F�N�V�����t���X�^�����擾
	/// @return �J�����r���[�v���W�F�N�V�����t���X�^��
	[[nodiscard]] const Frustum& GetViewProjectionFrustum()const { return frustum_; }

	[[nodiscard]] const XMMATRIX& GetViewMatrix()const { return viewMatrix_; }
	[[nodiscard]] const XMMATRIX& GetProjectionMatrix()const { return projMatrix_; }

	[[nodiscard]] const Transform& GetTransform()const { return cameraTransform_; }

	/// @brief �J�������W���擾
	/// @return �J�������W
	[[nodiscard]] const Position3D& GetPos() const { return pos_; }

	/// @brief �^�[�Q�b�g���W���擾
	/// @return �^�[�Q�b�g���W
	[[nodiscard]] const Position3D& GetTargetPos() const { return targetPos_; }

	/// @brief �A�b�v�x�N�g�����擾
	/// @return �A�b�v�x�N�g��
	[[nodiscard]] const Vector3& GetUpVector() const { return up_; }

	/// @brief ��]���擾
	/// @return ��]
	[[nodiscard]] const Quaternion& GetRotation() const { return rot_; }

	/// @brief �X�V��J�������W���擾
	/// @return �X�V��J�������W
	[[nodiscard]] Position3D GetLatestPos() const;

	/// @brief �X�V��^�[�Q�b�g���W���擾
	/// @return �X�V��^�[�Q�b�g���W
	[[nodiscard]] Position3D GetLatestTargetPos() const;

	/// @brief ���[�v���I��������
	/// @return ���[�v���I��������
	[[nodiscard]] bool IsEndLerp()const;

private:

	struct CameraInfo;

	Camera();
	~Camera();
	PROOF_OF_STATIC_SINGLETON(Camera);

	/// @brief �J�������͐ݒ�
	void InputSetting();

	/// @brief �J������񃊃Z�b�g
	void ResetCamera();

	/// @brief �X�V����:�t���[
	void UpdateFree();

	/// @brief �X�V����:�Ǐ]
	void UpdateFollow();

	/// @brief �X�V����:�����Ǐ]
	void UpdateRateFollow();

	/// @brief �X�V����:�Œ�
	void UpdateFixed();

	/// @brief �X�V����:���[�v
	void UpdateLerp();

	/// @brief �X�V����:�Ǐ]���[�v
	void UpdateLerpFollow();

	/// @brief �X�V����:�t���[
	void LatestUpdateFree(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief �X�V����:�Ǐ]
	void LatestUpdateFollow(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief �X�V����:�����Ǐ]
	void LatestUpdateRateFollow(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief �X�V����:�Œ�
	void LatestUpdateFixed(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief �X�V����:���[�v
	void LatestUpdateLerp(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief �X�V����:�Ǐ]���[�v
	void LatestUpdateLerpFollow(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief �J�������萔�o�b�t�@�쐬
	void CreateConstantBuffer();

	/// @brief �r���[�v���W�F�N�V�����s��o�b�t�@�쐬
	void CreateConstantBufferViewProjectionMatrix();

	/// @brief �J�������萔�o�b�t�@�쐬
	void CreateConstantBufferCameraInfo();
	
	/// @brief �J�������萔�o�b�t�@�X�V
	void UpdateViewProjectionMatrix();

	/// @brief �r���[�v���W�F�N�V�����t���X�^���X�V
	void UpdateFrustum(const XMMATRIX& viewProj);


	/// @brief ��ʗh��
	/// @param pos ����W
	void ShakeCamera(Position3D& pos);


	STATE state_;					/// @brief ���݂̏��

	Position3D pos_;				/// @brief ���W
	Position3D targetPos_;			/// @brief �^�[�Q�b�g���W
	Vector3 up_;					/// @brief �����
	Quaternion rot_;				/// @brief ��]

	float fov_;						/// @brief ��p
	float near_;					/// @brief �`���
	float far_;						/// @brief �`�打

	Transform cameraTransform_;		/// @brief �J�������

	bool isChanging_;				/// @brief �J�������ɕύX����������

	Frustum frustum_;				/// @brief �r���[�v���W�F�N�V�����t���X�^��
	BoundingSphere sphere_;			/// @brief �r���[�v���W�F�N�V�����t���X�^��


	optional<reference_wrapper<const Transform>> follower_;			/// @brief �Ǐ]�Ώ�
	Position3D followRelativePos_;									/// @brief �Ǐ]�����΍��W

	float followRate_;												/// @brief �Ǐ]���[�g
	optional<reference_wrapper<const Transform>> subFollower_;		/// @brief �Ǐ]�Ώ�(�T�u)
	Position3D subFollowRelativePos_;								/// @brief �Ǐ]�����΍��W(�T�u)

	Transform lerpStartTransform_;									/// @brief ���[�v�J�n���g�����X�t�H�[��
	Transform lerpEndTransform_;									/// @brief ���[�v�J�n���g�����X�t�H�[��

	optional<reference_wrapper<const Transform>> lerpEndFollower_;	/// @brief ���[�v�J�n���g�����X�t�H�[��

	Vector3 shakePow_;												/// @brief �V�F�C�N�����
	float shakeSpeed_;												/// @brief �V�F�C�N���鑬��
	bool isPriorityShake_;											/// @brief �D��I�ɃV�F�C�N���邩�ǂ���

	function<void()> StateUpdate_;									/// @brief ��ԕʍX�V���� 
	unordered_map<STATE, function<void()>> stateUpdateList_;		/// @brief ��ԕʍX�V�������X�g

	function<void(Position3D&, Position3D&)> StateLatestUpdate_;							/// @brief ��ԕʍX�V����
	unordered_map<STATE, function<void(Position3D&, Position3D&)>> stateLatestUpdateList_;	/// @brief ��ԕʍX�V�������X�g

	ComPtr<ID3D12Resource> viewProjMatConstBuffer_;	/// @brief �J�������o�b�t�@
	int viewProjHeapNo_;							/// @brief �J�������f�B�X�N���v�^�q�[�v�ԍ�
	XMMATRIX* viewProjMap_;							/// @brief �o�b�t�@�X�V�p�ϐ�

	ComPtr<ID3D12Resource> cameraConstBuffer_;		/// @brief �r���[�v���W�F�N�V�����s��o�b�t�@
	int cameraHeapNo_;								/// @brief �r���[�v���W�F�N�V�����s��f�B�X�N���v�^�q�[�v�ԍ�
	CameraInfo* cameraMap_;							/// @brief �o�b�t�@�X�V�p�ϐ�

	XMMATRIX viewMatrix_;		/// @brief �r���[�s��
	XMMATRIX projMatrix_;		/// @brief �v���W�F�N�V�����s��
};

/// @brief �J�������
struct Camera::CameraInfo
{
	Position3D pos = {};	/// @brief �J�������W
	float padding1 = 0.0f;	/// @brief �p�f�B���O
	Vector3 dir = {};		/// @brief �J��������
	float padding2 = 0.0f;	/// @brief �p�f�B���O
};
