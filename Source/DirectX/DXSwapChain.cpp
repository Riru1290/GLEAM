#include "DescriptorHeap/DescriptorHeapAllocator.h"
#include "DescriptorHeap/Heaps/RTVHeap.h"
#include "DXSwapChain.h"

DXSwapChain::DXSwapChain()
{
}

DXSwapChain::~DXSwapChain()
{
}

void DXSwapChain::Terminate()
{
}

bool DXSwapChain::Create(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = SCREEN_WIDTH;
	swapchainDesc.Height = SCREEN_HEIGHT;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;									// �_�u���o�b�t�@
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// �t���b�v��͑��₩�ɔj��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// �E�B���h�E�ƃt���X�N���[���؂�ւ��\

	auto result = Factory->CreateSwapChainForHwnd(CommandQueue.Get(), hwnd, &swapchainDesc,
		nullptr, nullptr, (IDXGISwapChain1**)swapChain_.ReleaseAndGetAddressOf());


	//if (!RTVHeap_->Create(2))
	//{
	//	assert(0 && "RTV�q�[�v�쐬���s");
	//	return false;
	//}

	// RTV�̍쐬�ƃX���b�v�`�F�[���Ƃ̊֘A�t��
	for (int i = 0; i < (int)swapchainBuffers_.size(); i++)
	{


		auto hr = swapChain_->GetBuffer(i, IID_PPV_ARGS(&swapchainBuffers_[i]));

		

		if (FAILED(hr))
		{
			assert(0 && "RTV�쐬���s");
			return false;
		}
		rtvHeapNo_[i] = HeapAllocator.CreateRTV(swapchainBuffers_[i].Get());
	}

	return !(FAILED(result));

}

const ComPtr<IDXGISwapChain4>& DXSwapChain::GetSwapChain()
{
    if (swapChain_ == nullptr)
    {
		assert(0 && "SwapChain�̍쐬���s");
    }
    return swapChain_;
}

const array<ComPtr<ID3D12Resource>, 2>& DXSwapChain::GetSwapChainBuffers()
{
	return swapchainBuffers_;
}

const D3D12_CPU_DESCRIPTOR_HANDLE DXSwapChain::GetRTVCPUAddress(int index)
{
	return HeapAllocator.GetRTVCPUAddress(rtvHeapNo_[index]);
}
