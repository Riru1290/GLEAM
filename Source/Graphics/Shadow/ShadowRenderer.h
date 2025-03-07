#pragma once
#include "../PipelineStateObject/PipelineStateObjectID.h"
#include "../../Common/Singleton/StaticSigleton.h"

#define Shadow ShadowRenderer::GetInstance()

class ShadowRenderer final : public StaticSingleton<ShadowRenderer>
{
public:

	static constexpr int SHADOW_MAP_MAX_NUM = (10);

	/// @brief �������
	void Terminate() override;

	void RenderBegin();

	/// @brief �[�x�o�b�t�@�B�e����
	void SetRenderTargetDepth();

	void SetBeforeDepthTexture();
	void SetDepthTexture(int rootSignatureIndex);

	void SetShadowMapIndex(int rootIndex);

private:

	ShadowRenderer();
	~ShadowRenderer();
	PROOF_OF_STATIC_SINGLETON(ShadowRenderer);

	/// @brief �e�p�̃o�b�t�@���쐬
	void CreateShadowBuffer();

	/// @brief �V���h�E�}�b�v�C���f�b�N�X�p�̃o�b�t�@���쐬
	void CreateShadowMapIndexBuffer();

	/// @brief �r���[�|�[�g,�V�U�[��`��ݒ�
	void ViewportScissorRectSetting();

	ComPtr<ID3D12Resource> shadowBuffer_[SHADOW_MAP_MAX_NUM];		/// @brief ���\�[�X
	int srvHeapNo_[SHADOW_MAP_MAX_NUM];								/// @brief �e�N�X�`���f�B�X�N���v�^�q�[�v�ԍ�
	int dsvHeapNo_[SHADOW_MAP_MAX_NUM];								/// @brief DSV�f�B�X�N���v�^�q�[�v�ԍ�

	ComPtr<ID3D12Resource> shadowMapIndexConstBuffer_;		/// @brief �V���h�E�}�b�v�C���f�b�N�X���o�b�t�@
	int shadowMapIndexHeapNo_;								/// @brief �V���h�E�}�b�v�C���f�b�N�X���f�B�X�N���v�^�q�[�v�ԍ�
	int* shadowMapIndexMap_;								/// @brief �o�b�t�@�X�V�p�ϐ�

	int setCount_;

	D3D12_VIEWPORT viewport_;						/// @brief �r���[�|�[�g
	D3D12_RECT scissorrect_;						/// @brief �V�U�[�Z�`

	//// �e�X�g
	//Position3D pos;
	//Position3D target;
	//Vector3 up;
	//ComPtr<ID3D12Resource> constBuffer_;
	//int heapNo_;
	//XMMATRIX* matrixMap_;
};

