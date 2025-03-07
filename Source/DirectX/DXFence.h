#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define Fence DXFence::GetInstance().GetFence()

/// @brief �t�F���X�Ǘ�
class DXFence final : public StaticSingleton<DXFence>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief Fence�쐬
	/// @return �쐬�ɐ���������
	[[nodiscard]] bool Create();

	/// @brief Fence�擾
	/// @return Fence
	[[nodiscard]] const ComPtr<ID3D12Fence>& GetFence();

	/// @brief �C�x���g�����
	void Close();

	/// @brief �R�}���h�L���[�̓����҂�
	void WaitForCommandQueue();

private:

	DXFence();
	~DXFence();
	PROOF_OF_STATIC_SINGLETON(DXFence);

	ComPtr<ID3D12Fence> fence_;	/// @brief �t�F���X

	UINT64 fenceVal_ = 0;		/// @brief �C�x���J�E���g
	HANDLE fenceEvent_;			/// @brief �C�x���g
};

