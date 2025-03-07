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
            assert(0 && "Fence作成失敗");
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
        auto event = CreateEvent(nullptr, false, false, nullptr);	// イベントハンドルの取得
        if (!event)
        {
            assert(0 && "イベントエラー、アプリケーションを終了します");
        }
        fence_->SetEventOnCompletion(fenceVal_, event);
        WaitForSingleObject(event, INFINITE);	// イベントが発生するまで待ち続ける
        CloseHandle(event);						// イベントハンドルを閉じる
    }
}
