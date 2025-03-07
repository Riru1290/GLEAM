#include "DXFence.h"

DXFence::DXFence()
{
}

DXFence::~DXFence()
{
}

void DXFence::Terminate()
{
    CloseHandle(fenceEvent_);
}

bool DXFence::Create()
{
    auto result = MainDevice->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    fenceEvent_ = CreateEvent(nullptr, false, false, nullptr);

    return !(FAILED(result));
}

const ComPtr<ID3D12Fence>& DXFence::GetFence()
{
    if (fence_ == nullptr)
    {
        if (!Create())
        {
            assert(0 && "Fence�쐬���s");
        }
    }
    return fence_;
}

void DXFence::Close()
{
    CloseHandle(fenceEvent_);
}

void DXFence::WaitForCommandQueue()
{
    CommandQueue->Signal(fence_.Get(), ++fenceVal_);

    if (fence_->GetCompletedValue() != fenceVal_)
    {
        auto event = CreateEvent(nullptr, false, false, nullptr);	// �C�x���g�n���h���̎擾
        if (!event)
        {
            assert(0 && "�C�x���g�G���[�A�A�v���P�[�V�������I�����܂�");
        }
        fence_->SetEventOnCompletion(fenceVal_, event);
        WaitForSingleObject(event, INFINITE);	// �C�x���g����������܂ő҂�������
        CloseHandle(event);						// �C�x���g�n���h�������
    }
}
