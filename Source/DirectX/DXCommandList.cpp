#include "DXCommandList.h"

DXCommandList::DXCommandList()
{
}

DXCommandList::~DXCommandList()
{
}

void DXCommandList::Terminate()
{
}

bool DXCommandList::Create()
{
    auto result = MainDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(),
		nullptr, IID_PPV_ARGS(&commandList_));

    return !(FAILED(result));
}

const ComPtr<ID3D12GraphicsCommandList>& DXCommandList::GetCommandList()
{
    if (commandList_ == nullptr)
    {
        if (!Create())
        {
            assert(0 && "CommandListçÏê¨é∏îs");
        }
    }
    return commandList_;
}
