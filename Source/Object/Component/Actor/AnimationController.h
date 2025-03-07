#pragma once
#include "../Component.h"
#include "../../../Resource/Model/ModelData.h"

class AnimationModelRenderer;
struct ANIM_DATA;

/// @brief ���f���A�j���[�V�����R���g���[��
class AnimationController final : public Component
{
public:

	[[nodiscard]] AnimationController();
	~AnimationController() = default;

	/// @brief �A�j���[�V���������郂�f�����Z�b�g
	/// @param modelRenderer ���f�������_���[
	void SetAnimationModel(AnimationModelRenderer& modelRenderer);

	/// @brief �A�j���[�V�����ǉ�
	/// @param animData �A�j���[�V�����f�[�^
	/// @param animKey �A�j���[�V�����L�[
	void AddAnimation(ANIMATION_DATA& animData,const string& animKey);

	/// @brief �A�j���[�V�����ύX
	/// @param animKey �A�j���[�V�����L�[
	void ChangeAnimation(const string& animKey);

	/// @brief �A�j���[�V�����I������
	/// @param animKey �A�j���[�V�����L�[
	/// @return �A�j���[�V�������I��������
	[[nodiscard]] bool IsEndAnimation(const string& animKey) const;
	
	/// @brief ���[�v�ݒ�
	/// @param animKey �A�j���[�V�����L�[
	/// @param isLoop ���[�v�����邩
	void SetIsLoop(bool isLoop, const string& animKey);

	/// @brief �X�g�b�v�ݒ�
	/// @param animKey �A�j���[�V�����L�[
	/// @param isStop �X�g�b�v�����邩
	void SetIsStop(bool isStop, const string& animKey);

	void SetAnimPlaySpeed(float speed, const string& animKey);
	void SetAnimPlaySpeed(float speed);

	/// @brief �A�j���[�V�����I�����ɌĂ΂��֐����Z�b�g
	/// @param animKey �A�j���[�V�����L�[
	/// @param isLoop �A�j���[�V�����I�����ɌĂ΂��֐�
	void SetOnAnimEndFunction(function<void()> OnAnimEnd, const string& animKey);


private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ANIMATION_CONTROLLER; }

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �`�揈��
	void DrawComponent() override;

	/// @brief �������
	void ReleaseComponent() override;

	/// @brief �A�j���[�V�������Ԃ��X�V
	void UpdateAnimationTime();

	/// @brief �A�j���[�V�����u�����h�����X�V
	void UpdateAnimationBlendRate();

	/// @brief �A�j���[�V�����s����X�V
	void UpdateAnimationMatrix();

	/// @brief �t���[����Ԍ�̃A�j���[�V�����s���Ԃ�
	/// @param data �s��z��
	/// @param frame �t���[��
	/// @return �t���[����Ԍ�̃A�j���[�V�����s��
	[[nodiscard]] XMMATRIX GetFrameInterpolatedAnimationMatrix(const vector<XMMATRIX>& data, float frame, bool isLoop);

	string currentKey_;									/// @brief ���ݗD�悳��Ă���A�j���[�V�����L�[
	unordered_map<string, ANIM_DATA> animations_;		/// @brief �A�j���[�V�����f�[�^
	bool isChangingAnimation_;							/// @brief �A�j���[�V�����J�ڒ���

	optional<reference_wrapper<AnimationModelRenderer>> modelRenderer_;	/// @brief �A�j���[�V�����K�p�惂�f��
};

struct ANIM_DATA
{
	// �ς��Ȃ�����------------------------------

	string animName = "";							/// @brief �A�j���[�V������

	map<string, vector<XMMATRIX>> animBoneMatrix;	/// @brief �{�[�����Ǝ��Ԃ��Ƃ̃A�j���[�V�����s��

	float start = 0.0f;				/// @brief �A�j���[�V�����J�n����(�����l)
	float end = 0.0f;				/// @brief �A�j���[�V�����I������(�����l)
	float total = 0.0f;				/// @brief �A�j���[�V�������Đ�����(�����l)

	float frameRate = 60.0f;		/// @brief �A�j���[�V�����t���[�����[�g

	// �ς��\���̂������----------------------

	bool isAnimation = false;		/// @brief �A�j���[�V���������ݓK�p����Ă��邩�ǂ���

	float animTime = 0.0f;			/// @brief �A�j���[�V�������ݎ���
	float animStartTime = 0.0f;		/// @brief �A�j���[�V�����J�n����
	float animEndTime = 0.0f;		/// @brief �A�j���[�V�����I������
	float animTotalTime = 0.0f;		/// @brief �A�j���[�V�������Đ�����

	float animSpeed = 1.0f;			/// @brief �A�j���[�V�����X�s�[�h

	bool isLoop = true;				/// @brief ���[�v�����邩
	bool isStop = false;			/// @brief �Đ����X�g�b�v�����邩

	float animBlendRate = 0.0f;		/// @brief �u�����h��
	float animAttachSpeed = 1.5f;	/// @brief �u�����h���������x
	float animDetachSpeed = 1.5f;	/// @brief �u�����h���������x

	function<void()> OnAnimEnd;		/// @brief �A�j���[�V�����I�����ɌĂԊ֐�
};
