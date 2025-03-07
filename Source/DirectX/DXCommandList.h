#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define DxCommand DXCommandList::GetInstance().GetCommandList()

/// @brief �R�}���h���X�g�Ǘ�
class DXCommandList final : public StaticSingleton<DXCommandList>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief CommandList�쐬
	/// @return �쐬�ɐ���������
	[[nodiscard]] bool Create();

	/// @brief CommandList�擾
	/// @return CommandList
	[[nodiscard]] const ComPtr<ID3D12GraphicsCommandList>& GetCommandList();

private:

	DXCommandList();
	~DXCommandList();
	PROOF_OF_STATIC_SINGLETON(DXCommandList);

	ComPtr<ID3D12GraphicsCommandList> commandList_;	/// @brief �R�}���h���X�g
};

