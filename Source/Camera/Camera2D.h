#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define MainCamera2D Camera2D::GetInstance()

/// @brief �J����2D
class Camera2D final : public StaticSingleton<Camera2D>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief �J�������
	enum class STATE
	{
		FREE = 0,	// �t���[�J����
		FIXED,		// �Œ�
	};


	/// @brief �J������Ԑ؂�ւ�
	/// @param state �؂�ւ����
	void ChangeState(STATE state);

	/// @brief �X�V����
	void Update();

	/// @brief �J���������V�F�[�_�ɃZ�b�g
	void SetBeforeDraw();

private:

	struct Camera2DInfo;

	Camera2D();
	~Camera2D();
	PROOF_OF_STATIC_SINGLETON(Camera2D);

	/// @brief �X�V����:�t���[
	void UpdateFree();

	/// @brief �X�V����:�Œ�
	void UpdateFixed();

	/// @brief �r���[�v���W�F�N�V�����s��o�b�t�@�쐬
	void CreateConstantBufferScreenMatrix();

	/// @brief �J�������萔�o�b�t�@�X�V
	void UpdateScreenMatrix();



	STATE state_;			/// @brief ���݂̏��

	Position2D pos_;		/// @brief ���W
	float angle_;			/// @brief �p�x	
	float lens_;			/// @brief �J�����{��

	bool isChanging_;		/// @brief �J�������ɕύX����������


	function<void()> StateUpdate_;								/// @brief ��ԕʍX�V���� 
	unordered_map<STATE, function<void()>> stateUpdateList_;	/// @brief ��ԕʍX�V�������X�g

	ComPtr<ID3D12Resource> cameraConstBuffer_;				/// @brief �J�������o�b�t�@
	int cameraHeapNo_;										/// @brief �J�������f�B�X�N���v�^�q�[�v�ԍ�
	Camera2DInfo* cameraMap_;								/// @brief �o�b�t�@�X�V�p�ϐ�

};

/// @brief �J�������
struct Camera2D::Camera2DInfo
{
	Position2D pos = {};	/// @brief ���W
	float angle = 0.0f;		/// @brief �p�x
	float lens = 1.0f;		/// @brief �g�嗦
	XMMATRIX screenMat = {};/// @brief �X�N���[���s��
};

