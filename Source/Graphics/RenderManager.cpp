#ifdef _DEBUG
#include "DebugRender/DebugRenderManager.h"
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_impl_win32.h"
#include "../../External/imgui/imgui_impl_dx12.h"
#endif
#include "../Application/Application.h"
#include "../Utility/Math/Quaternion.h"
#include "Light/LightController.h"
#include "PipelineStateObject/PipelineStateObjectManager.h"
#include "../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../Object/Effect/EffectController.h"
#include "../Object/Component/Renderer/Renderers/Single/2D/Object2DRenderer.h"
#include "../Resource/Texture/Texture.h"
#include "PostEffect/PostEffectApplier.h"
#include "Shadow/ShadowRenderer.h"
#include "../Camera/Camera.h"
#include "RenderManager.h"


RenderManager::RenderManager()
{
    // ビューポート,シザー矩形を設定
    ViewportScissorRectSetting();

    // 深度バッファービュー作成
    CreateDepthBuffer();

    auto dsvHandle = HeapAllocator.GetDSVCPUAddress(dsvHeapNo_);
    PostEffect.SetDepthBufferHandle(dsvHandle);

}

RenderManager::~RenderManager()
{
}


void RenderManager::Terminate()
{

}

void RenderManager::DeleteRenderer(shared_ptr<Component> renderer)
{
    if (renderer == nullptr)return;

    for (auto& [layer, renderers] : renderers_)
    {
        erase_if(renderers,
            [&renderer](const weak_ptr<Renderer>& weakRenderer)
            {
                return weakRenderer.lock() == renderer;
            });
    }

    for (auto& [layer, renderers] : renderers2D_)
    {
        erase_if(renderers,
            [&renderer](const weak_ptr<Renderer>& weakRenderer)
            {
                return weakRenderer.lock() == renderer;
            });
    }
}

void RenderManager::RenderBegin()
{
    // コマンドリストのリセット
    CommandAllocator->Reset();
    DxCommand->Reset(CommandAllocator.Get(), nullptr);

    // PSOリセット
    PSOMng.Reset();

    // ディスクリプタヒープセット
    HeapAllocator.SetDescriptorHeaps();

    // レンダラー
    for (auto& [layer, renderers] : renderers_)
    {
        for (auto& renderer : renderers)
        {
            if (!renderer.lock()->IsActive())continue;

            renderer.lock()->RenderBegin();
        }
    }

    for (auto& [layer, renderers] : renderers2D_)
    {
        for (auto& renderer : renderers)
        {
            if (!renderer.lock()->IsActive())continue;

            renderer.lock()->RenderBegin();
        }
    }

#ifdef _DEBUG
    ImGuiBegin;

    ImGui::Text("HeapUseCount : %d", HeapAllocator.GetRegisterNum());
#endif
}

namespace
{
    float time;
    chrono::system_clock::time_point nowTime;
}


void RenderManager::RenderEnd()
{

    // ライト情報更新
    Light.Update();

    // 影
    RenderShadow();


    // レンダーターゲット設定
    PostEffect.SetRenderTarget();

    // ビューポートセット
    DxCommand->RSSetViewports(1, &viewport_);

    // シザー短形セット
    DxCommand->RSSetScissorRects(1, &scissorrect_);


    RenderDefault();

    RenderTrans();

    EffectCon.Update();

    // エフェクト描画後に再度ディスクリプタヒープを設定
    HeapAllocator.SetDescriptorHeaps();

    Render2D();


    PostEffect.ApplyPostEffect();

#ifdef _DEBUG
    auto preTime = chrono::system_clock::now();

    auto t = static_cast<float>(
        chrono::duration_cast<chrono::nanoseconds>(nowTime - preTime).count() / 1000000000.0);


    ImGui::Text("RenderTime = %f", t);
#endif
    
    // バックバッファの番号を取得
    UINT frameIndex = SwapChain->GetCurrentBackBufferIndex();

    // リソースバリアの設定
    // PRESENT -> RENDER_TARGET
    SetResourceBarrier(DXSwapChain::GetInstance().GetSwapChainBuffers()[frameIndex].Get(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

    // レンダーターゲットの設定
    auto rtvHandle = DXSwapChain::GetInstance().GetRTVCPUAddress(frameIndex);
    auto dsvHandle = HeapAllocator.GetDSVCPUAddress(dsvHeapNo_);

    DxCommand->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

    // 画面クリア
    float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };  // 白色
    DxCommand->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    DxCommand->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    PostEffect.Render();

#if defined(_DEBUG)
    ImGuiEnd;
#endif

    // リソースバリアの設定
    // RENDER_TARGET -> PRESENT
    SetResourceBarrier(DXSwapChain::GetInstance().GetSwapChainBuffers()[frameIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

    // 命令のクローズ
    DxCommand->Close();

    // コマンドリストの実行
    ID3D12CommandList* cmdlists[] = { DxCommand.Get() };
    CommandQueue->ExecuteCommandLists(1, cmdlists);

    // 画面のスワップ
    HRESULT hr = SwapChain->Present(1, 0);
    if (FAILED(hr))
    {
        // エラーが出た場合強制的にアプリケーションを閉じる
        exit(0);
    }

    // GPU処理の終了を待機
    DXFence::GetInstance().WaitForCommandQueue();


#ifdef _DEBUG
    nowTime = chrono::system_clock::now();
#endif
}

void RenderManager::RenderDefault()
{
    // レンダラー
    for (auto& [layer, renderers] : renderers_)
    {
        for (auto& renderer : renderers)
        {
            if (!renderer.lock()->IsActive())continue;

            renderer.lock()->RenderEnd();
        }
    }
}

void RenderManager::RenderTrans()
{
    // レンダラー
    for (auto& [layer, renderers] : renderers_)
    {
        for (auto& renderer : renderers)
        {
            if (!renderer.lock()->IsActive())continue;

            renderer.lock()->RenderEndTrans();
        }
    }
}

void RenderManager::RenderShadow()
{
    Shadow.RenderBegin();

    int shadowMapNum = Light.GetLightCameraProjMatNum();

    if (shadowMapNum == 2)
    {
        int i = 0;
    }

    for (int i = 0; i < shadowMapNum; i++)
    {
        Shadow.SetRenderTargetDepth();


        // レンダラー
        for (auto& [layer, renderers] : renderers_)
        {
            for (auto& renderer : renderers)
            {
                if (!renderer.lock()->IsActive())continue;

                renderer.lock()->RenderShadow();
            }
        }
    }

    Shadow.SetBeforeDepthTexture();
}

void RenderManager::Render2D()
{
    // レンダラー
    for (auto& [layer, renderers] : renderers2D_)
    {
        for (auto& renderer : renderers)
        {
            if (!renderer.lock()->IsActive())continue;

            renderer.lock()->RenderEnd();
        }
    }
}

void RenderManager::DeleteRendererFromList()
{
    if (deleteRendererList_.size() <= 0)return;

    for (auto& listRenderer : deleteRendererList_)
    {
        if (listRenderer.expired())continue;

        for (auto& [layer, renderers] : renderers_)
        {
            erase_if(renderers,
                [&](const weak_ptr<Renderer>& renderer)
                {
                    return renderer.lock() == listRenderer.lock();
                });
        }
    }

    deleteRendererList_.clear();
}

void RenderManager::ViewportScissorRectSetting()
{
    // ビューポート
    viewport_.Width = SCREEN_WIDTH;
    viewport_.Height = SCREEN_HEIGHT;
    viewport_.TopLeftX = 0;
    viewport_.TopLeftY = 0;
    viewport_.MaxDepth = 1.0f;
    viewport_.MinDepth = 0.0f;

    // シザー矩形
    scissorrect_.top = 0;
    scissorrect_.left = 0;
    scissorrect_.right = scissorrect_.left + SCREEN_WIDTH;
    scissorrect_.bottom = scissorrect_.top + SCREEN_HEIGHT;
}

void RenderManager::CreateDepthBuffer()
{
    // 深度バッファー作成
    D3D12_RESOURCE_DESC depthResDesc = {};
    depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthResDesc.Width = SCREEN_WIDTH;
    depthResDesc.Height = SCREEN_HEIGHT;
    depthResDesc.DepthOrArraySize = 1;
    depthResDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    depthResDesc.SampleDesc.Count = 1;
    depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    depthResDesc.MipLevels = 1;
    depthResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthResDesc.Alignment = 0;
    auto depthHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    // クリアバリューの設定
    D3D12_CLEAR_VALUE _depthClearValue = {};
    _depthClearValue.DepthStencil.Depth = 1.0f;      //深さ１(最大値)でクリア
    _depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; //32bit深度値としてクリア
    MainDevice->CreateCommittedResource(
        &depthHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &depthResDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE, //デプス書き込みに使用
        &_depthClearValue,
        IID_PPV_ARGS(depthBuffer_.ReleaseAndGetAddressOf()));

    // 深度バッファービュー作成
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

    dsvHeapNo_ = HeapAllocator.CreateDSV(depthBuffer_.Get(), dsvDesc);

}

