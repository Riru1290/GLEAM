#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define MainDevice DXDevice::GetInstance().GetDevice()

/// @brief デバイス管理
class DXDevice final : public StaticSingleton<DXDevice>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief Device作成
	/// @return 作成に成功したか
	[[nodiscard]] bool Create();

	/// @brief Device取得
	/// @return Device
	[[nodiscard]] const ComPtr<ID3D12Device>& GetDevice();

private:

	DXDevice();
	~DXDevice();
	PROOF_OF_STATIC_SINGLETON(DXDevice);

	ComPtr<ID3D12Device> device_;	/// @brief デバイス

	// GPUティアリスト
	// この順番が優先度となる
	enum class GPUTier
	{
		NVIDIA,
		Amd,
		Intel,
		Arm,
		Qualcomm,
		Kind,
	};

	void EnableDebugLayer() {
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
			debugController_->EnableDebugLayer();
		}
		else {
			OutputDebugStringA("Direct3D デバッグレイヤーの有効化に失敗しました。\n");
		}
	}

	ComPtr<ID3D12Debug> debugController_;
};

