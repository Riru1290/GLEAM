#pragma once
#include "../DescriptorHeap.h"

/// @brief CBV_SRV_UAV�p�̃f�B�X�N���v�^�q�[�v
class CBV_SRV_UAVHeap final : public DescriptorHeap
{
public:

	[[nodiscard]] CBV_SRV_UAVHeap() = default;
	~CBV_SRV_UAVHeap() = default;

	/// @brief  �q�[�v�쐬
	/// @param useCount �g�p��
	/// @return �쐬�ɐ���������
	[[nodiscard]] bool Create(int useCount) override;

	/// @brief CBV�̍쐬
	/// @param buffer �o�b�t�@�[�̃|�C���^
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateCBV(ID3D12Resource* buffer);

	/// @brief CBV�̍쐬
	/// @param buffer �o�b�t�@�[�̃|�C���^
	/// @param desc CBV�ݒ�
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateCBV(ID3D12Resource* buffer, D3D12_CONSTANT_BUFFER_VIEW_DESC desc);

	/// @brief SRV�̍쐬
	/// @param buffer �o�b�t�@�[�̃|�C���^
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateSRV(ID3D12Resource* buffer);

	/// @brief SRV�̍쐬
	/// @param buffer �o�b�t�@�[�̃|�C���^
	/// @param desc SRV�ݒ�
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateSRV(ID3D12Resource* buffer, D3D12_SHADER_RESOURCE_VIEW_DESC desc);
 
};

