#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define CommandQueue DXCommandQueue::GetInstance().GetCommandQueue()

/// @brief コマンドキュー管理
class DXCommandQueue final : public StaticSingleton<DXCommandQueue>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief CommandQueue作成
	/// @return 作成に成功したか
	[[nodiscard]] bool Create();

	/// @brief CommandQueue取得
	/// @return CommandQueue
	[[nodiscard]] const ComPtr<ID3D12CommandQueue>& GetCommandQueue();

private:

	DXCommandQueue();
	~DXCommandQueue();
	PROOF_OF_STATIC_SINGLETON(DXCommandQueue);

	ComPtr<ID3D12CommandQueue> commandQueue_;	/// @brief コマンドキュー
};

