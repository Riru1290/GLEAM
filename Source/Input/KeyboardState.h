#pragma once

/// @brief �L�[�{�[�h���͊Ǘ�
class KeyboardState
{
public:

	[[nodiscard]] KeyboardState(const HWND& hWnd);
	~KeyboardState();

	/// @brief �X�V����
	void Update();

	/// @brief ��������
	/// @param key �L�[
	/// @param buffer �o�b�t�@�ԍ�
	/// @return  �������ꂽ��
	[[nodiscard]] bool Pressed(unsigned int key , unsigned int buffer = defaultBuffer_);

	/// @brief �L�[�������ꂽ�u�Ԃ̔���
	/// @param key �L�[
	/// @return �����ꂽ�u�Ԃ�
	[[nodiscard]] bool Triggered(unsigned int key);

	/// @brief �����ꂽ�u�Ԃ̔���
	/// @param key �L�[
	/// @return �����ꂽ�u�Ԃ�
	[[nodiscard]] bool Released(unsigned int key);

	/// @brief ��������L�[�������ꂽ
	/// @return ��������L�[�������ꂽ��
	[[nodiscard]] bool AnyPressed();

	/// @brief ��������L�[�������ꂽ
	/// @return ��������L�[�������ꂽ
	[[nodiscard]] bool AnyTriggered();



private:

	unsigned int currentBuffer_;						/// @brief ���݂̃o�b�t�@�ԍ�
	static constexpr size_t defaultBuffer_ = UINT_MAX;	/// @brief �f�t�H���g�̃o�b�t�@�ԍ�

	BYTE state_[2][256];		/// @brief �L�[�{�[�h�̓��͏��


};

