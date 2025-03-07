#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define CommandAllocator DXCommandAllocator::GetInstance().GetCommandAllocator()

/// @brief �R�}���h�A���P�[�^�Ǘ�
class DXCommandAllocator final : public StaticSingleton<DXCommandAllocator>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief CommandAllocator�쐬
	/// @return �쐬�ɐ���������
	[[nodiscard]] bool Create();

	/// @brief CommandAllocator�擾
	/// @return CommandAllocator
	[[nodiscard]] const ComPtr<ID3D12CommandAllocator>& GetCommandAllocator();

private:

	DXCommandAllocator();
	~DXCommandAllocator();
	PROOF_OF_STATIC_SINGLETON(DXCommandAllocator);

	/// @brief �R�}���h�A���P�[�^
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
};

