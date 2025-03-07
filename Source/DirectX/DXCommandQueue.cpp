#include "DXCommandQueue.h"

DXCommandQueue::DXCommandQueue()
{
}

DXCommandQueue::~DXCommandQueue()
{
}

void DXCommandQueue::Terminate()
{
}

bool DXCommandQueue::Create()
{
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;				// �^�C���A�E�g�Ȃ�
	cmdQueueDesc.NodeMask = 0;										// �A�_�v�^�[��1�������Ȃ��Ƃ���0�ł���
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// �v���C�I���e�B�͓��Ɏw��Ȃ�
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;				// �R�}���h���X�g�ƍ��킹��

	// �L���[����
	auto result = MainDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue_));

	return !(FAILED(result));
}

const ComPtr<ID3D12CommandQueue>& DXCommandQueue::GetCommandQueue()
{
    if (commandQueue_ == nullptr)
    {
        if (!Create())
        {
            assert(0 && "CommandQueue�쐬���s");
        }
    }
    return commandQueue_;
}
