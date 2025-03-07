#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define Fence DXFence::GetInstance().GetFence()

/// @brief フェンス管理
class DXFence final : public StaticSingleton<DXFence>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief Fence作成
	/// @return 作成に成功したか
	[[nodiscard]] bool Create();

	/// @brief Fence取得
	/// @return Fence
	[[nodiscard]] const ComPtr<ID3D12Fence>& GetFence();

	/// @brief イベントを閉じる
	void Close();

	/// @brief コマンドキューの同期待ち
	void WaitForCommandQueue();

private:

	DXFence();
	~DXFence();
	PROOF_OF_STATIC_SINGLETON(DXFence);

	ComPtr<ID3D12Fence> fence_;	/// @brief フェンス

	UINT64 fenceVal_ = 0;		/// @brief イベンカウント
	HANDLE fenceEvent_;			/// @brief イベント
};

