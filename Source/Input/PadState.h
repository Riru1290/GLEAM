#pragma once
#include "PadID.h"

/// @brief �p�b�h(XInput)���͊Ǘ�
class PadState
{
public:

	[[nodiscard]] PadState(UINT padNo = PAD_1);
	~PadState();

	/// @brief �X�V����
	/// @return �ڑ�����Ă��Ȃ����false��Ԃ�
	[[nodiscard]] bool Update();

	/// @brief ��������
	/// @param button �{�^��
	/// @param buffer �o�b�t�@�ԍ�
	/// @return  �������ꂽ��
	[[nodiscard]] bool Pressed(unsigned int button, unsigned int buffer = defaultBuffer_);

	/// @brief �L�[�������ꂽ�u�Ԃ̔���
	/// @param button �{�^��
	/// @return �����ꂽ�u�Ԃ�
	[[nodiscard]] bool Triggered(unsigned int button);

	/// @brief �����ꂽ�u�Ԃ̔���
	/// @param button �{�^��
	/// @return �����ꂽ�u�Ԃ�
	[[nodiscard]] bool Released(unsigned int button);

	/// @brief ���g���K�[�̉������ݗʎ擾
	/// @param buffer �o�b�t�@�ԍ�
	/// @return ���g���K�[�̉������ݗ�0(min)~255(max)
	[[nodiscard]] int GetLeftTriggerState(unsigned int buffer = defaultBuffer_);

	/// @brief �E�g���K�[�̉������ݗʎ擾
	/// @param buffer �o�b�t�@�ԍ�
	/// @return �E�g���K�[�̉������ݗ�0(min)~255(max)
	[[nodiscard]] int GetRightTriggerState(unsigned int buffer = defaultBuffer_);

	/// @brief ���X�e�B�b�N�̈ړ��ʗʎ擾
	/// @param buffer �o�b�t�@�ԍ� 
	/// @return ���X�e�B�b�N�̈ړ���  -32768(min)~32767(max)
	[[nodiscard]] Vector2 GetLeftStickState(unsigned int buffer = defaultBuffer_);

	/// @brief �E�X�e�B�b�N�̈ړ��ʗʎ擾
	/// @param buffer �o�b�t�@�ԍ� 
	/// @return �E�X�e�B�b�N�̈ړ���  -32768(min)~32767(max)
	[[nodiscard]] Vector2 GetRightStickState(unsigned int buffer = defaultBuffer_);
		
	/// @brief ��������{�^���������ꂽ
	/// @return ��������{�^���������ꂽ��
	[[nodiscard]] bool AnyPressed();

	/// @brief ��������{�^���������ꂽ
	/// @return ��������{�^���������ꂽ
	[[nodiscard]] bool AnyTriggered();

private:

	unsigned int currentBuffer_;						/// @brief ���݂̃o�b�t�@�ԍ�
	static constexpr size_t defaultBuffer_ = UINT_MAX;	/// @brief �f�t�H���g�̃o�b�t�@�ԍ�

	XINPUT_STATE state_[2];		/// @brief �L�[�{�[�h�̓��͏��

	UINT padNo_;				/// @brief �p�b�h�ԍ�
};

