#pragma once
#include "../DescriptorHeap.h"

/// @brief RTV用のディスクリプタヒープ
class RTVHeap final : public DescriptorHeap
{
public:

	[[nodiscard]] RTVHeap() = default;
	~RTVHeap() = default;
	
	/// @brief  ヒープ作成
	/// @param useCount 使用個数
	/// @return 作成に成功したか
	[[nodiscard]] bool Create(int useCount) override;

	/// @brief RTVの作成
	/// @param Buffer バッファーのポインタ
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateRTV(ID3D12Resource* Buffer);

	/// @brief RTVの作成
	/// @param Buffer バッファーのポインタ
	/// @param desc RTV設定
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateRTV(ID3D12Resource* Buffer, D3D12_RENDER_TARGET_VIEW_DESC desc);

};

