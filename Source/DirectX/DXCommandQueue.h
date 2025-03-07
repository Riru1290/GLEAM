#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define CommandQueue DXCommandQueue::GetInstance().GetCommandQueue()

/// @brief �R�}���h�L���[�Ǘ�
class DXCommandQueue final : public StaticSingleton<DXCommandQueue>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief CommandQueue�쐬
	/// @return �쐬�ɐ���������
	[[nodiscard]] bool Create();

	/// @brief CommandQueue�擾
	/// @return CommandQueue
	[[nodiscard]] const ComPtr<ID3D12CommandQueue>& GetCommandQueue();

private:

	DXCommandQueue();
	~DXCommandQueue();
	PROOF_OF_STATIC_SINGLETON(DXCommandQueue);

	ComPtr<ID3D12CommandQueue> commandQueue_;	/// @brief �R�}���h�L���[
};

