#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define SwapChain DXSwapChain::GetInstance().GetSwapChain()

class RTVHeap;

/// @brief スワップチェーン管理
class DXSwapChain final : public StaticSingleton<DXSwapChain>
{
public:

	/// @brief 解放処理
	void Terminate() override;
	
	/// @brief SwapChain作成
	/// @param hwnd ウィンドウハンドル
	/// @return 作成に成功したか
	[[nodiscard]] bool Create(HWND hwnd);

	/// @brief SwapChain取得
	/// @return SwapChain
	[[nodiscard]] const ComPtr<IDXGISwapChain4>& GetSwapChain();

	/// @brief SwapChainのバッファ取得
	/// @return SwapChainのバッファ
	[[nodiscard]] const array<ComPtr<ID3D12Resource>, 2>& GetSwapChainBuffers();

	/// @brief RTVのCPU上アドレス取得
	/// @param index RTV配列の番号
	/// @return RTVのCPU上アドレス取得
	[[nodiscard]] const D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUAddress(int index);

private:

	DXSwapChain();
	~DXSwapChain();
	PROOF_OF_STATIC_SINGLETON(DXSwapChain);

	ComPtr<IDXGISwapChain4> swapChain_;						/// @brief スワップチェーン
	array<ComPtr<ID3D12Resource>, 2> swapchainBuffers_;		/// @brief スワップチェーンバッファ

	int rtvHeapNo_[2];										/// @brief RTVヒープ番号
};

