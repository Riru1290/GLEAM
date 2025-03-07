#pragma once
#include "../DescriptorHeap.h"

/// @brief DSV�p�̃f�B�X�N���v�^�q�[�v
class DSVHeap final : public DescriptorHeap
{
public:

	[[nodiscard]] DSVHeap() = default;
	~DSVHeap() = default;

	/// @brief �q�[�v�쐬
	/// @param useCount �g�p��
	/// @return �쐬�ɐ���������
	[[nodiscard]] bool Create(int useCount) override;

	/// @brief DSV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateDSV(ID3D12Resource* Buffer);

	/// @brief DSV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @param desc DSV�ݒ�
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateDSV(ID3D12Resource* Buffer, D3D12_DEPTH_STENCIL_VIEW_DESC desc);
};

