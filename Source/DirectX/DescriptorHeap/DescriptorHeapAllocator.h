#pragma once
#include "../../Common/Singleton/StaticSigleton.h"

class RTVHeap;
class DSVHeap;
class CBV_SRV_UAVHeap;

#define HeapAllocator DescriptorHeapAllocator::GetInstance()

/// @brief ディスクリプタヒープ管理
class DescriptorHeapAllocator final : public StaticSingleton<DescriptorHeapAllocator>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	void SetDescriptorHeaps();

	// SRVーーーーーーーーーーーーーーーーーーーー

	/// @brief 指定レジスタ番号のディスクリプタを解放
	/// @param registNo レジスタ番号
	void ReleaseDescriptorRTV(int registNo);

	/// @brief RTVの作成
	/// @param Buffer バッファーのポインタ
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateRTV(ID3D12Resource* Buffer);

	/// @brief RTVの作成
	/// @param Buffer バッファーのポインタ
	/// @param desc RTV設定
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateRTV(ID3D12Resource* Buffer, D3D12_RENDER_TARGET_VIEW_DESC desc);

	/// @brief CPU側アドレスを返す関数
	/// @param number 登録番号
	/// @return CPU側アドレス
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUAddress(int number);

	/// @brief GPU側アドレスを返す関数
	/// @param number 登録番号
	/// @return GPU側アドレス
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUAddress(int number);

	//ーーーーーーーーーーーーーーーーーーーーーー

	// DSVーーーーーーーーーーーーーーーーーーーー

	/// @brief 指定レジスタ番号のディスクリプタを解放
	/// @param registNo レジスタ番号
	void ReleaseDescriptorDSV(int registNo);

	/// @brief DSVの作成
	/// @param Buffer バッファーのポインタ
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateDSV(ID3D12Resource* Buffer);

	/// @brief DSVの作成
	/// @param Buffer バッファーのポインタ
	/// @param desc DSV設定
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateDSV(ID3D12Resource* Buffer, D3D12_DEPTH_STENCIL_VIEW_DESC desc);

	/// @brief CPU側アドレスを返す関数
	/// @param number 登録番号
	/// @return CPU側アドレス
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUAddress(int number);

	/// @brief GPU側アドレスを返す関数
	/// @param number 登録番号
	/// @return GPU側アドレス
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUAddress(int number);

	//ーーーーーーーーーーーーーーーーーーーーーー

	// CBVーーーーーーーーーーーーーーーーーーーー

	/// @brief CBVの作成
	/// @param Buffer バッファーのポインタ
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateCBV(ID3D12Resource* Buffer);

	/// @brief CBVの作成
	/// @param Buffer バッファーのポインタ
	/// @param desc CBV設定
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateCBV(ID3D12Resource* Buffer, D3D12_CONSTANT_BUFFER_VIEW_DESC desc);

	//ーーーーーーーーーーーーーーーーーーーーーー

	// SRVーーーーーーーーーーーーーーーーーーーー

	/// @brief SRVの作成
	/// @param Buffer バッファーのポインタ
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateSRV(ID3D12Resource* Buffer);

	/// @brief SRVの作成
	/// @param Buffer バッファーのポインタ
	/// @param desc SRV設定
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateSRV(ID3D12Resource* Buffer, D3D12_SHADER_RESOURCE_VIEW_DESC desc);

	//ーーーーーーーーーーーーーーーーーーーーーー

	// CBV,SRV,UAVーーーーーーーーーーーーーーーー

	/// @brief 指定レジスタ番号のディスクリプタを解放
	/// @param registNo レジスタ番号
	void ReleaseDescriptorCBV_SRV_UAV(int registNo);

	/// @brief CPU側アドレスを返す関数
	/// @param number 登録番号
	/// @return CPU側アドレス
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCBV_SRV_UAVCPUAddress(int number);

	/// @brief GPU側アドレスを返す関数
	/// @param number 登録番号
	/// @return GPU側アドレス
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetCBV_SRV_UAVGPUAddress(int number);

	//ーーーーーーーーーーーーーーーーーーーーーー

		// テスト
	int GetRegisterNum();

private:

	DescriptorHeapAllocator();
	~DescriptorHeapAllocator();
	PROOF_OF_STATIC_SINGLETON(DescriptorHeapAllocator);

	unique_ptr<RTVHeap> rtvHeap_;					/// @brief RTV用ディスクリプタヒープ
	unique_ptr<DSVHeap> dsvHeap_;					/// @brief DSV用ディスクリプタヒープ
	unique_ptr<CBV_SRV_UAVHeap> cbv_srv_uavHeap_;	/// @brief CBV,SRV,UAV用ディスクリプタヒープ
};

