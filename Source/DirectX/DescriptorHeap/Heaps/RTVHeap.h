#pragma once
#include "../DescriptorHeap.h"

/// @brief RTV�p�̃f�B�X�N���v�^�q�[�v
class RTVHeap final : public DescriptorHeap
{
public:

	[[nodiscard]] RTVHeap() = default;
	~RTVHeap() = default;
	
	/// @brief  �q�[�v�쐬
	/// @param useCount �g�p��
	/// @return �쐬�ɐ���������
	[[nodiscard]] bool Create(int useCount) override;

	/// @brief RTV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateRTV(ID3D12Resource* Buffer);

	/// @brief RTV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @param desc RTV�ݒ�
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateRTV(ID3D12Resource* Buffer, D3D12_RENDER_TARGET_VIEW_DESC desc);

};

