#pragma once

/// @brief PipelineStateObject
struct PSO
{
	ComPtr<ID3D12PipelineState> pipelinestate;	/// @brief パイプラインステート

	ComPtr<ID3D12RootSignature> rootsignature;	/// @brief ルートシグネチャ
};
