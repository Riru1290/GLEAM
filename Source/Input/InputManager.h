#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "PadID.h"

#define InputMng InputManager::GetInstance()

class KeyboardState;
class PadState;

/// @brief ���̓^�C�v
enum class INPUT_TYPE
{
	Pad,
	Keyboard
};

/// @brief ���̓f�[�^
struct INPUT_DATA
{
	INPUT_TYPE type_;
	unsigned int key_;
};

/// @brief �v���C���[ID
enum class PLAYER_ID 
{
	P1,
	P2,
	P3,
	P4,
	MAX
};

/// @brief ���͏��Ǘ�
class InputManager final : public StaticSingleton<InputManager>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief �X�V����
	void Update();

	/// @brief �R���t�B�O�ǉ�
	/// @param key �L�[
	/// @param list �L�[�ɑΉ���������̓f�[�^�Q
	void AddKey(const string& key, vector<INPUT_DATA> list);

	/// @brief ��������
	/// @param key �L�[
	/// @param padNo �p�b�h�ԍ�(�w�肵�Ȃ���΃L�[�{�[�h����)
	/// @return  �������ꂽ��
	[[nodiscard]] bool Pressed(const string& key,UINT padNo = UINT_MAX);

	/// @brief �L�[�������ꂽ�u�Ԃ̔���
	/// @param key �L�[
	/// @param padNo �p�b�h�ԍ�(�w�肵�Ȃ���΃L�[�{�[�h����)
	/// @return �����ꂽ�u�Ԃ�
	[[nodiscard]] bool Triggered(const string& key, UINT padNo = UINT_MAX);

	/// @brief �����ꂽ�u�Ԃ̔���
	/// @param key �L�[
	/// @param padNo �p�b�h�ԍ�(�w�肵�Ȃ���΃L�[�{�[�h����)
	/// @return �����ꂽ�u�Ԃ�
	[[nodiscard]] bool Released(const string& key, UINT padNo = UINT_MAX);

	/// @brief ����������
	/// @param key �L�[
	/// @param padNo �p�b�h�ԍ�(�w�肵�Ȃ���΃L�[�{�[�h����)
	/// @return  �w�莞�Ԓ��������Ă�����
	[[nodiscard]] bool Hold(float time, const string& key, UINT padNo = UINT_MAX);

	/// @brief ���g���K�[�̉������ݗʎ擾
	/// @param padNo �p�b�h�ԍ� 
	/// @return ���g���K�[�̉������ݗ�0(min)~255(max)
	[[nodiscard]] int GetLeftTriggerState(UINT padNo = PAD_1);

	/// @brief �E�g���K�[�̉������ݗʎ擾
	/// @param padNo �p�b�h�ԍ� 
	/// @return �E�g���K�[�̉������ݗ�0(min)~255(max)
	[[nodiscard]] int GetRightTriggerState(UINT padNo = PAD_1);


	/// @brief ���X�e�B�b�N�̈ړ��ʗʎ擾
	/// @param padNo �p�b�h�ԍ� 
	/// @return ���X�e�B�b�N�̈ړ���  -32768(min)~32767(max)
	[[nodiscard]] Vector2 GetLeftStickState(UINT padNo = PAD_1);

	/// @brief �E�X�e�B�b�N�̈ړ��ʗʎ擾
	/// @param padNo �p�b�h�ԍ� 
	/// @return �E�X�e�B�b�N�̈ړ���  -32768(min)~32767(max)
	[[nodiscard]] Vector2 GetRightStickState(UINT padNo = PAD_1);

	/// @brief ��������L�[(�{�^��)�������ꂽ
	/// @return ��������L�[(�{�^��)�������ꂽ��
	[[nodiscard]] bool AnyPressed(UINT padNo = UINT_MAX);

	/// @brief ��������L�[(�{�^��)�������ꂽ
	/// @return ��������L�[(�{�^��)�������ꂽ
	[[nodiscard]] bool AnyTriggered(UINT padNo = UINT_MAX);

	/// @brief ���ݐڑ�����Ă���p�b�h�̐��擾
	/// @return ���ݐڑ�����Ă���p�b�h�̐�
	[[nodiscard]] const int& GetConnectingPadNum()const { return connectingPadNum_; };


	/// @brief �w�肳�ꂽ�L�[�ɑΉ�����R���t�B�O�ݒ��Ԃ�
	/// @param key �L�[
	/// @return �w�肳�ꂽ�L�[�ɑΉ�����R���t�B�O�ݒ�
	[[nodiscard]] vector<INPUT_DATA> GetKeyConfig(const string& key);

	/// @brief �}�E�X�ړ��ʎ擾
	/// @return �}�E�X�ړ���
	[[nodiscard]] const Vector2& GetMouseMovePow() { return mouseMovePow_; }

	/// @brief �}�E�X���W�擾
	/// @return �}�E�X���W
	[[nodiscard]] const Position2D& GetMousePosition() { return mousePos_; }

	/// @brief 1�t���[���O�}�E�X���W�擾
	/// @return 1�t���[���O�}�E�X���W
	[[nodiscard]] const Position2D& GetPreMousePosition() { return preMousePos_; }

	/// @brief Window����}�E�X�̈ړ��ʂ��擾
	/// @param mouseMovePow �}�E�X�̈ړ���
	void SetMouseMovePowFromWindow(Vector2 mouseMovePow);

	/// @brief �}�E�X���W���Œ艻���邩�ǂ���
	/// @param isFixedMouse �}�E�X���W���Œ艻���邩�ǂ���
	void SetIsFixedMouse(bool isFixedMouse) { isFixedMouse_ = isFixedMouse; }

private:

	InputManager();
	~InputManager();
	PROOF_OF_STATIC_SINGLETON(InputManager);

	/// @brief �}�E�X���X�V
	void MouseDataUpdate();

	unique_ptr<KeyboardState> keyboard_;		/// @brief �L�[�{�[�h

	static constexpr int PAD_NUM_MAX = (4);		/// @brief �p�b�h�ő吔
	unique_ptr<PadState> pad_[PAD_NUM_MAX];		/// @brief �p�b�h

	unsigned int connectingPadNum_ = 0;			/// @brief ���ݐڑ�����Ă���p�b�h�̐�

	Position2D mouseMovePow_ = {};				/// @brief �}�E�X�ړ���
	Position2D mousePos_ = {};					/// @brief �}�E�X���W
	Position2D preMousePos_ = {};				/// @brief 1�t���[���O�}�E�X���W

	bool isFixedMouse_ = false;					/// @brief �J�[�\�����Œ肷�邩

	unordered_map<string, vector<INPUT_DATA>> keyConfig_;	/// @brief �L�[�R���t�B�O���X�g

	unordered_map<string, float> keyHoldTime_;				/// @brief �L�[��A�����ĉ����Ă�������

};

