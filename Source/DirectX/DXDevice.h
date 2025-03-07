#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define MainDevice DXDevice::GetInstance().GetDevice()

/// @brief �f�o�C�X�Ǘ�
class DXDevice final : public StaticSingleton<DXDevice>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief Device�쐬
	/// @return �쐬�ɐ���������
	[[nodiscard]] bool Create();

	/// @brief Device�擾
	/// @return Device
	[[nodiscard]] const ComPtr<ID3D12Device>& GetDevice();

private:

	DXDevice();
	~DXDevice();
	PROOF_OF_STATIC_SINGLETON(DXDevice);

	ComPtr<ID3D12Device> device_;	/// @brief �f�o�C�X

	// GPU�e�B�A���X�g
	// ���̏��Ԃ��D��x�ƂȂ�
	enum class GPUTier
	{
		NVIDIA,
		Amd,
		Intel,
		Arm,
		Qualcomm,
		Kind,
	};

	void EnableDebugLayer() {
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
			debugController_->EnableDebugLayer();
		}
		else {
			OutputDebugStringA("Direct3D �f�o�b�O���C���[�̗L�����Ɏ��s���܂����B\n");
		}
	}

	ComPtr<ID3D12Debug> debugController_;
};

