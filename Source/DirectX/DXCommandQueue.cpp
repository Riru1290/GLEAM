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
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;				// タイムアウトなし
	cmdQueueDesc.NodeMask = 0;										// アダプターを1つしかつかないときは0でいい
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// プライオリティは特に指定なし
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;				// コマンドリストと合わせる

	// キュー生成
	auto result = MainDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue_));

	return !(FAILED(result));
}

const ComPtr<ID3D12CommandQueue>& DXCommandQueue::GetCommandQueue()
{
    if (commandQueue_ == nullptr)
    {
        if (!Create())
        {
            assert(0 && "CommandQueue作成失敗");
        }
    }
    return commandQueue_;
}
