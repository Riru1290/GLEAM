#pragma once

/// @brief PipelineStateObject
struct PSO
{
	ComPtr<ID3D12PipelineState> pipelinestate;	/// @brief �p�C�v���C���X�e�[�g

	ComPtr<ID3D12RootSignature> rootsignature;	/// @brief ���[�g�V�O�l�`��
};
