#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define DxCommand DXCommandList::GetInstance().GetCommandList()

/// @brief コマンドリスト管理
class DXCommandList final : public StaticSingleton<DXCommandList>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief CommandList作成
	/// @return 作成に成功したか
	[[nodiscard]] bool Create();

	/// @brief CommandList取得
	/// @return CommandList
	[[nodiscard]] const ComPtr<ID3D12GraphicsCommandList>& GetCommandList();

private:

	DXCommandList();
	~DXCommandList();
	PROOF_OF_STATIC_SINGLETON(DXCommandList);

	ComPtr<ID3D12GraphicsCommandList> commandList_;	/// @brief コマンドリスト
};

