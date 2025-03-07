#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "PipelineStateObject.h"
#include "PipelineStateObjectID.h"

#define PSOCreator PipelineStateObjectCreator::GetInstance()

/// @brief PSO�N���G�C�^�[
class PipelineStateObjectCreator final : public StaticSingleton<PipelineStateObjectCreator>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief PSO�쐬
	/// @param  PipelineStateObjectID
	/// @return PSO
	[[nodiscard]] PSO CreatePSO(PSO_ID psoID);

private:

	PipelineStateObjectCreator();
	~PipelineStateObjectCreator();
	PROOF_OF_STATIC_SINGLETON(PipelineStateObjectCreator);

	/// @brief �f�t�H���g��3D�pPSO���쐬
	PSO CreatePSODefault3D();

	/// @brief �f�t�H���g��2D�pPSO���쐬
	PSO CreatePSODefault2D();

	/// @brief �v���~�e�B�u3D�pPSO���쐬
	PSO CreatePSOPrimitive3D();

	/// @brief �v���~�e�B�u3D�C���X�^���X�`��pPSO���쐬
	PSO CreatePSOPrimitive3DInstancing();

	/// @brief UI3D�pPSO���쐬
	PSO CreatePSOUI3D();

	/// @brief �A�j���[�V����3D�pPSO���쐬
	PSO CreatePSOAnimation3D();

	/// @brief �X�v���C�g�摜�pPSO���쐬
	PSO CreatePSOSprite2D();

	/// @brief �������C�g�pPSO���쐬
	PSO CreatePSOLimlight();

	/// @brief �g�pPSO���쐬
	PSO CreatePSOWave();

	/// @brief �t�F�[�h�pPSO���쐬
	PSO CreatePSOFade();

	/// @brief TV�X�N���[���pPSO���쐬
	PSO CreatePSOTVScreen();

	/// @brief TV�pPSO���쐬
	PSO CreatePSOTV();

	/// @brief �f�t�H���g�V���h�EPSO���쐬
	PSO CreatePSOShadowDefault();

	/// @brief �A�j���[�V�����V���h�EPSO���쐬
	PSO CreatePSOShadowAnimation();

	/// @brief 2D�V���h�EPSO���쐬
	PSO CreatePSOShadow2D();

	/// @brief �f�t�H���g�|�X�g�G�t�F�N�gPSO���쐬
	PSO CreatePSODefaultPE();

	/// @brief ���m�N���|�X�g�G�t�F�N�gPSO���쐬
	PSO CreatePSOMonochromePE();

	/// @brief ���]�|�X�g�G�t�F�N�gPSO���쐬
	PSO CreatePSOReversePE();

	/// @brief ���b�J�[�|�X�g�G�t�F�N�gPSO���쐬
	PSO CreatePSOLockerPE();

	/// @brief �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
	/// @param dRange �f�B�X�N���v�^�����W
	/// @param rParam ���[�g�p�����[�^
	/// @param sDesc �T���v���[�f�X�N
	void SetDefaultRootSignatureDesc2D(
		CD3DX12_DESCRIPTOR_RANGE* dRange, CD3DX12_ROOT_PARAMETER* rParam, CD3DX12_STATIC_SAMPLER_DESC* sDesc);

	/// @brief �f�t�H���g��GPS�ݒ���Z�b�g
	/// @param gpsDesc �O���t�B�b�N�X�p�C�v���C���X�e�[�g
	/// @param rootSignature ���[�g�V�O�l�`��
	/// @param inputLayout ���_���C�A�E�g
	/// @param inputLayoutNum ���_���C�A�E�g�p�����[�^��
	void SetDefaultGraphicsPipelineStateDesc2D(
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature,
		D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum);

	/// @brief �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
	/// @param dRange �f�B�X�N���v�^�����W
	/// @param rParam ���[�g�p�����[�^
	/// @param sDesc �T���v���[�f�X�N
	void SetDefaultRootSignatureDesc3D(
		CD3DX12_DESCRIPTOR_RANGE* dRange, CD3DX12_ROOT_PARAMETER* rParam, CD3DX12_STATIC_SAMPLER_DESC* sDesc);

	/// @brief �f�t�H���g�̃��[�g�V�O�l�`���ݒ���Z�b�g
	/// @param dRange �f�B�X�N���v�^�����W
	/// @param rParam ���[�g�p�����[�^
	/// @param sDesc �T���v���[�f�X�N
	void SetDefaultRootSignatureDescShadow(
		CD3DX12_DESCRIPTOR_RANGE* dRange, CD3DX12_ROOT_PARAMETER* rParam, CD3DX12_STATIC_SAMPLER_DESC* sDesc);

	/// @brief �f�t�H���g��GPS�ݒ���Z�b�g
	/// @param gpsDesc �O���t�B�b�N�X�p�C�v���C���X�e�[�g
	/// @param rootSignature ���[�g�V�O�l�`��
	/// @param inputLayout ���_���C�A�E�g
	/// @param inputLayoutNum ���_���C�A�E�g�p�����[�^��
	void SetDefaultGraphicsPipelineStateDesc3D(
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature,
		D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum);

	/// @brief �f�t�H���g��GPS�ݒ���Z�b�g
	/// @param gpsDesc �O���t�B�b�N�X�p�C�v���C���X�e�[�g
	/// @param rootSignature ���[�g�V�O�l�`��
	/// @param inputLayout ���_���C�A�E�g
	/// @param inputLayoutNum ���_���C�A�E�g�p�����[�^��
	/// @param vsPath ���_�V�F�[�_�p�X
	void SetDefaultGraphicsPipelineStateDescShadow(
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature,
		D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum);

	/// @brief �f�t�H���g��GPS�ݒ���Z�b�g
	/// @param gpsDesc �O���t�B�b�N�X�p�C�v���C���X�e�[�g
	/// @param rootSignature ���[�g�V�O�l�`��
	/// @param inputLayout ���_���C�A�E�g
	/// @param inputLayoutNum ���_���C�A�E�g�p�����[�^��
	void SetDefaultGraphicsPipelineStateDescPostEffect(
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature,
		D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum);

	unordered_map<PSO_ID, function<PSO()>> CreatePSO_;	/// @brief PSO�쐬�֐�
};

