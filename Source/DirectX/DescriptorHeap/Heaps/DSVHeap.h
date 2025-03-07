#pragma once
#include "../DescriptorHeap.h"

/// @brief DSV用のディスクリプタヒープ
class DSVHeap final : public DescriptorHeap
{
public:

	[[nodiscard]] DSVHeap() = default;
	~DSVHeap() = default;

	/// @brief ヒープ作成
	/// @param useCount 使用個数
	/// @return 作成に成功したか
	[[nodiscard]] bool Create(int useCount) override;

	/// @brief DSVの作成
	/// @param Buffer バッファーのポインタ
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateDSV(ID3D12Resource* Buffer);

	/// @brief DSVの作成
	/// @param Buffer バッファーのポインタ
	/// @param desc DSV設定
	/// @return ヒープの紐づけられた登録番号
	[[nodiscard]] int CreateDSV(ID3D12Resource* Buffer, D3D12_DEPTH_STENCIL_VIEW_DESC desc);
};

