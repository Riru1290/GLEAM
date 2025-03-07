#pragma once
#include "../PipelineStateObject/PipelineStateObjectID.h"
#include "../../Common/Singleton/StaticSigleton.h"

#define PostEffect PostEffectApplier::GetInstance()

enum class POSTEFFECT_ID
{
	DEFAULT,
	MONOCHROME,
	REVERSE,
	LOCKER,
};

class Mesh;

/// @brief �|�X�g�G�t�F�N�g�K�p
class PostEffectApplier final : public StaticSingleton<PostEffectApplier>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief �[�x�o�b�t�@�n���h����ݒ�
	/// @param depthHandle �[�x�o�b�t�@�n���h��
	void SetDepthBufferHandle(D3D12_CPU_DESCRIPTOR_HANDLE depthHandle)
	{
		depthHandle_ = depthHandle;
	}

	/// @brief �����_�[�^�[�Q�b�g��ݒ�
	void SetRenderTarget();

	/// @brief �K�p������|�X�g�G�t�F�N�g��\��
	/// @param id �|�X�g�G�t�F�N�gID
	void ReservePostEffect(POSTEFFECT_ID id);

	/// @brief �|�X�g�G�t�F�N�g��K�p
	void ApplyPostEffect();

	/// @brief �|�X�g�G�t�F�N�g�K�p��X�N���[����`��
	void Render();

	ComPtr<ID3D12Resource> GetBuffer();

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle();

private:

	PostEffectApplier();
	~PostEffectApplier();
	PROOF_OF_STATIC_SINGLETON(PostEffectApplier);

	/// @brief PSOID�Ή��\��ݒ�
	void PSOListSetting();

	/// @brief �|�X�g�G�t�F�N�g�p�̃o�b�t�@���쐬
	void CreatePostEffectBuffer();

	/// @brief �X�N���[����𕢂����b�V�����쐬
	void CreateMesh();


	ComPtr<ID3D12Resource> postEffectBuffer_[2];	/// @brief ���\�[�X
	int srvHeapNo_[2];								/// @brief �e�N�X�`���f�B�X�N���v�^�q�[�v�ԍ�
	int rtvHeapNo_[2];								/// @brief RTV�f�B�X�N���v�^�q�[�v�ԍ�

	int currentBufferNo_;							/// @brief ���ݎg�p���Ă���o�b�t�@�ԍ�

	unordered_map<POSTEFFECT_ID,PSO_ID> psoList_;	/// @brief PSOID�Ή��\

	vector<POSTEFFECT_ID> applyPosteffectList_;		/// @brief �K�p������|�X�g�G�t�F�N�g

	unique_ptr<Mesh> mesh_;							/// @brief �X�N���[����𕢂����b�V��

	D3D12_CPU_DESCRIPTOR_HANDLE depthHandle_;
};

