#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define CommandAllocator DXCommandAllocator::GetInstance().GetCommandAllocator()

/// @brief コマンドアロケータ管理
class DXCommandAllocator final : public StaticSingleton<DXCommandAllocator>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief CommandAllocator作成
	/// @return 作成に成功したか
	[[nodiscard]] bool Create();

	/// @brief CommandAllocator取得
	/// @return CommandAllocator
	[[nodiscard]] const ComPtr<ID3D12CommandAllocator>& GetCommandAllocator();

private:

	DXCommandAllocator();
	~DXCommandAllocator();
	PROOF_OF_STATIC_SINGLETON(DXCommandAllocator);

	/// @brief コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
};

