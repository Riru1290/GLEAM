#include "DXCommandAllocator.h"

DXCommandAllocator::DXCommandAllocator()
{
}

DXCommandAllocator::~DXCommandAllocator()
{
}

void DXCommandAllocator::Terminate()
{
}

bool DXCommandAllocator::Create()
{
    auto result = MainDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));

    return !(FAILED(result));
}

const ComPtr<ID3D12CommandAllocator>& DXCommandAllocator::GetCommandAllocator()
{
    if (commandAllocator_ == nullptr)
    {
        if (!Create()) 
        {
            assert(0 && "CommandAllocatorçÏê¨é∏îs");
        }
    }
    return commandAllocator_;
}
