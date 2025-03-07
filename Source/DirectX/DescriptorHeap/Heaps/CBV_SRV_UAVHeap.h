#pragma once
#include "../DescriptorHeap.h"

/// @brief CBV_SRV_UAV用のディスクリプタヒープ
class CBV_SRV_UAVHeap final : public DescriptorHeap
{
public:

	[[nodiscard]] CBV_SRV_UAVHeap() = default;
	~CBV_SRV_UAVHeap() = default;

	/// @brief  ヒープ作成
	/// @param useCount 使用個数
	/// @return 作成に成功したか
	[[nodiscard]] bool Create(int useCount) override;

	/// @brief CBVの作成
	/// @param buffer バッファーのポインタ
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateCBV(ID3D12Resource* buffer);

	/// @brief CBVの作成
	/// @param buffer バッファーのポインタ
	/// @param desc CBV設定
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateCBV(ID3D12Resource* buffer, D3D12_CONSTANT_BUFFER_VIEW_DESC desc);

	/// @brief SRVの作成
	/// @param buffer バッファーのポインタ
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateSRV(ID3D12Resource* buffer);

	/// @brief SRVの作成
	/// @param buffer バッファーのポインタ
	/// @param desc SRV設定
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateSRV(ID3D12Resource* buffer, D3D12_SHADER_RESOURCE_VIEW_DESC desc);
 
};

