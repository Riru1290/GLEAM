#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define SwapChain DXSwapChain::GetInstance().GetSwapChain()

class RTVHeap;

/// @brief �X���b�v�`�F�[���Ǘ�
class DXSwapChain final : public StaticSingleton<DXSwapChain>
{
public:

	/// @brief �������
	void Terminate() override;
	
	/// @brief SwapChain�쐬
	/// @param hwnd �E�B���h�E�n���h��
	/// @return �쐬�ɐ���������
	[[nodiscard]] bool Create(HWND hwnd);

	/// @brief SwapChain�擾
	/// @return SwapChain
	[[nodiscard]] const ComPtr<IDXGISwapChain4>& GetSwapChain();

	/// @brief SwapChain�̃o�b�t�@�擾
	/// @return SwapChain�̃o�b�t�@
	[[nodiscard]] const array<ComPtr<ID3D12Resource>, 2>& GetSwapChainBuffers();

	/// @brief RTV��CPU��A�h���X�擾
	/// @param index RTV�z��̔ԍ�
	/// @return RTV��CPU��A�h���X�擾
	[[nodiscard]] const D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUAddress(int index);

private:

	DXSwapChain();
	~DXSwapChain();
	PROOF_OF_STATIC_SINGLETON(DXSwapChain);

	ComPtr<IDXGISwapChain4> swapChain_;						/// @brief �X���b�v�`�F�[��
	array<ComPtr<ID3D12Resource>, 2> swapchainBuffers_;		/// @brief �X���b�v�`�F�[���o�b�t�@

	int rtvHeapNo_[2];										/// @brief RTV�q�[�v�ԍ�
};

