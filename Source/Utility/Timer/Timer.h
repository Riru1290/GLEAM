#pragma once
#include "../../Common/Singleton/StaticSigleton.h"

#define MainTimer Timer::GetInstance()

/// @brief ���ԊǗ��N���X
class Timer final : public StaticSingleton<Timer>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief ���s
	void Run();

	/// @brief �^�C�}�[�v���Z�X���Z�b�g
	void ResetMyTimer();

	/// @brief �w��̃v���Z�X���Z�b�g
	/// @param key �L�[
	void Reset(string key);

	/// @brief �w��̃v���Z�X�J�n
	/// @param key �L�[
	void Start(string key);

	/// @brief �w��̃v���Z�X��~
	/// @param key �L�[
	void Stop(string key);

	/// @brief �w��̃v���Z�X���X�^�[�g
	/// @param key �L�[
	void Restart(string key);

	/// @brief �w��̃v���Z�X�폜
	/// @param key �L�[
	void Delete(string key);


	/// @brief �^�C�}�[�v���Z�X�ǉ�
	/// @param key �L�[
	/// @param maxTime �Z�b�g���鎞��
	/// @param start �����ŃX�^�[�g�����邩
	void SetTimer(string key ,float maxTime , bool start = false);

	/// @brief �w��̃v���Z�X�̎��Ԑݒ�
	/// @param key �L�[
	/// @param time ����
	void SetTime(string key ,float time);

	/// @brief �n���ꂽ�l�Ƀf���^�^�C����K�p������
	/// @param val �f���^�^�C����K�p���������l
	void MultDelta(float& val);

	/// @brief �n���ꂽ�l�Ƀf���^�^�C����K�p���������̂�Ԃ�
	/// @param val �f���^�^�C����K�p���������l
	/// @return �f���^�^�C���K�p��̒l
	[[nodiscard]] float GetMultDelta(const float& val);


	/// @brief ���Ԃ��w�莞��,�w��̃X�s�[�h�Ői�܂���
	/// @param rate ���Ԍo�ߑ��x
	/// @param time ���Ԍo�ߑ��x��ω������鎞��
	void SetWitchTimeOnMyTimer(float rate, float time);

	/// @brief �f���^�^�C���擾
	/// @return �f���^�^�C��
	[[nodiscard]] float GetDelta()const { return deltaTime_ * whichTimeRate_; }

	/// @brief �w��̃v���Z�X���ݎ��Ԏ擾
	/// @param key �L�[
	/// @return �v���Z�X���ݎ���
	[[nodiscard]] float GetTime(string key)const;

	/// @brief �w��̃v���Z�X�^�C�}�[���Ԏ擾
	/// @param key �L�[
	/// @return �^�C�}�[����
	[[nodiscard]] float GetMaxTime(string key)const;

	// ���݂̎��Ԍo�߂̊�����Ԃ� : �o�ߎ���1�b,�ݒ莞��10�b�̏ꍇ0.1���Ԃ����
	[[nodiscard]] float GetRate(string key)const;

	/// @brief �w��̃v���Z�X���I���ς݂��ǂ�����Ԃ�
	/// @param key �L�[
	/// @return �w��̃v���Z�X���I���ς݂��ǂ���
	bool IsEndTimer(string key)const;

	/// @brief �w��̃v���Z�X���ғ������ǂ�����Ԃ�
	/// @param key �L�[
	/// @return �w��̃v���Z�X���ғ������ǂ���
	bool InProgress(string key)const;

	/// @brief �f���^�^�C���Čv�Z
	void RecalculationDelta();

private:

	Timer();
	~Timer();
	PROOF_OF_STATIC_SINGLETON(Timer);

	float deltaTime_; /// @brief �f���^�^�C��

	chrono::system_clock::time_point preTime_; /// @brief 1�t���[���O����

	/// @brief �^�C�}�[�v���Z�X
	struct Process
	{
		bool isRunning_ = false;	/// @brief ���s�����ǂ���
		bool isForward_ = false;	/// @brief ���Ԃ𑝂₵�Ă����̂�

		float time_ = 0.0f;			/// @brief �v���Ɏg������
		float maxTime_ = 0.0f;		/// @brief �^�C�}�[�Z�b�g���̎���

		bool isEnd_ = false;		/// @brief �ꎞ��~�����邩

		/// @brief ���s����
		void Run()
		{
			if (!isRunning_)return;

			if (isForward_) {
				time_ += MainTimer.GetDelta();
			}
			else {
				time_ -= MainTimer.GetDelta();
				if (time_ <= 0.0f) {
					isEnd_ = true;
					isRunning_ = false;
					time_ = 0.0f;
				}
			}
		}

		/// @brief ����������
		void Reset()
		{
			if (isForward_) {
				time_ = 0.0f;
			}
			else {
				time_ = maxTime_;
			}

			isRunning_ = false;
			isEnd_ = false;
		}
	};

	unordered_map<string, Process> processes_;	/// @brief �v���Z�X

	float whichTimeRate_;	/// @brief ���Ԍo�ߑ��x
	float whichTime_;		/// @brief ���Ԍo�ߑ��x��ω������鎞��

};

