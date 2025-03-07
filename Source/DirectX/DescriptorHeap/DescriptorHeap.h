#pragma once

/// @brief �f�B�X�N���v�^�q�[�v����N���X
class DescriptorHeap
{
public:

	DescriptorHeap() = default;
	virtual ~DescriptorHeap() = default;

	/// @brief  �q�[�v�쐬
	/// @param useCount �g�p��
	/// @return �쐬�ɐ���������
	[[nodiscard]] virtual bool Create(int useCount) = 0;

	/// @brief CPU���A�h���X��Ԃ��֐�
	/// @param number �o�^�ԍ�
	/// @return CPU���A�h���X
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCPUAddress(int number);

	/// @brief GPU���A�h���X��Ԃ��֐�
	/// @param number �o�^�ԍ�
	/// @return GPU���A�h���X
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGPUAddress(int number);

	/// @brief �f�B�X�N���v�^�q�[�v���̎擾
	/// @return �f�B�X�N���v�^�q�[�v
	[[nodiscard]] ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap()const { return heap_; }

	/// @brief �w�背�W�X�^�ԍ��̃f�B�X�N���v�^�����
	/// @param registNo ���W�X�^�ԍ�
	void ReleaseDescriptor(int registNo);


	// �e�X�g
	int GetRegisterNum()
	{
		int use = 0;
		for (auto& [no, flag] : registNumber_)
		{
			if (flag) use++;
		}
		return use;
	}

protected:

	ComPtr<ID3D12DescriptorHeap> heap_ = nullptr;	/// @brief �q�[�v

	int useCount_ = 0;								/// @brief �g�p��

	int incrementSize_ = 0;							/// @brief �C���N�������g�T�C�Y

	map<int, bool> registNumber_;			/// @brief ���W�X�^�ԍ��Ǘ�
};

