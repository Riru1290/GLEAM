#include "../PipelineStateObject/PipelineStateObjectManager.h"
#include "../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "ShadowRenderer.h"

namespace
{
	const int SCREEN_SHADOW_WIDTH = (1920 * 4);			// スクリーンサイズ(幅)
	static const int SCREEN_SHADOW_HEIGHT = (1080 * 4); // スクリーンサイズ(高さ)
}

ShadowRenderer::ShadowRenderer():
	setCount_(0)
{
	CreateShadowBuffer();
	CreateShadowMapIndexBuffer();
	ViewportScissorRectSetting();
}

ShadowRenderer::~ShadowRenderer()
{
}

void ShadowRenderer::CreateShadowBuffer()
{
	D3D12_HEAP_PROPERTIES heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC resdesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R32_TYPELESS, SCREEN_SHADOW_WIDTH, SCREEN_SHADOW_HEIGHT);
	resdesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE cv = {};
	cv.DepthStencil.Depth = 1.0f;
	cv.Format = DXGI_FORMAT_D32_FLOAT;

	// DSV
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	// SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;


	for (int i = 0; i < SHADOW_MAP_MAX_NUM; i++)
	{
		auto result = MainDevice->CreateCommittedResource(
			&heapprop,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&cv,
			IID_PPV_ARGS(shadowBuffer_[i].ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			assert(0 && "シャドウバッファ作成失敗");
		}

		// 深度バッファビュー作成
		dsvHeapNo_[i] =
			HeapAllocator.CreateDSV(shadowBuffer_[i].Get(), dsvDesc);

		// テクスチャバッファービュー作成
		srvHeapNo_[i] =
			HeapAllocator.CreateSRV(shadowBuffer_[i].Get(), srvDesc);
	}
}

void ShadowRenderer::CreateShadowMapIndexBuffer()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(UINT) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(shadowMapIndexConstBuffer_.ReleaseAndGetAddressOf())
	);

	shadowMapIndexConstBuffer_->Map(0, nullptr, (void**)&shadowMapIndexMap_);

	*shadowMapIndexMap_ = 0;

	shadowMapIndexHeapNo_ = HeapAllocator.CreateCBV(shadowMapIndexConstBuffer_.Get());
}

void ShadowRenderer::ViewportScissorRectSetting()
{
	// ビューポート
	viewport_.Width = SCREEN_SHADOW_WIDTH;
	viewport_.Height = SCREEN_SHADOW_HEIGHT;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MaxDepth = 1.0f;
	viewport_.MinDepth = 0.0f;

	// シザー矩形
	scissorrect_.top = 0;
	scissorrect_.left = 0;
	scissorrect_.right = scissorrect_.left + SCREEN_SHADOW_WIDTH;
	scissorrect_.bottom = scissorrect_.top + SCREEN_SHADOW_HEIGHT;
}

void ShadowRenderer::Terminate()
{
	shadowMapIndexConstBuffer_->Unmap(0, nullptr);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(shadowMapIndexHeapNo_);
}

void ShadowRenderer::RenderBegin()
{
	setCount_ = 0;
	// RESOURCE -> DEPTH
	SetResourceBarrier(shadowBuffer_[0].Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_DEPTH_WRITE);

	// ビューポートセット
	DxCommand->RSSetViewports(1, &viewport_);

	// シザー短形セット
	DxCommand->RSSetScissorRects(1, &scissorrect_);
}

void ShadowRenderer::SetRenderTargetDepth()
{

	auto dsvHandle = HeapAllocator.GetDSVCPUAddress(dsvHeapNo_[setCount_]);

	DxCommand->OMSetRenderTargets(0, nullptr, false, &dsvHandle);

	// 画面クリア
	DxCommand->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	*shadowMapIndexMap_ = setCount_;


	setCount_++;
}

void ShadowRenderer::SetBeforeDepthTexture()
{
	// DEPTH -> RESOURCE
	SetResourceBarrier(shadowBuffer_[0].Get(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void ShadowRenderer::SetDepthTexture(int rootSignatureIndex)
{
	DxCommand->SetGraphicsRootDescriptorTable(rootSignatureIndex, HeapAllocator.GetCBV_SRV_UAVGPUAddress(srvHeapNo_[0]));
}

void ShadowRenderer::SetShadowMapIndex(int rootIndex)
{
	DxCommand->SetGraphicsRootDescriptorTable(rootIndex, HeapAllocator.GetCBV_SRV_UAVGPUAddress(shadowMapIndexHeapNo_));
}
