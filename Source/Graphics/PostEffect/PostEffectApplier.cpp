#include "../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../Common/Mesh/Meshes/MeshDefault2D.h"
#include "../Material/Material.h"
#include "PostEffectApplier.h"

namespace
{
	const int POST_EFFECT_TARGET_SIGNATURE_NO = (7);
}

PostEffectApplier::PostEffectApplier() :
    currentBufferNo_(0)
{
	PSOListSetting();
    CreatePostEffectBuffer();
	CreateMesh();
}

PostEffectApplier::~PostEffectApplier()
{
}

void PostEffectApplier::Terminate()
{
}

void PostEffectApplier::SetRenderTarget()
{



	// ポストエフェクト用
	// RESOURCE -> RENDER_TARGET
	SetResourceBarrier(postEffectBuffer_[currentBufferNo_].Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// レンダーターゲットの設定
	auto rtvHandle = HeapAllocator.GetRTVCPUAddress(rtvHeapNo_[currentBufferNo_]);

	// 画面クリア
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	DxCommand->OMSetRenderTargets(1, &rtvHandle, false, &depthHandle_);

	DxCommand->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	DxCommand->ClearDepthStencilView(depthHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	
}

void PostEffectApplier::ReservePostEffect(POSTEFFECT_ID id)
{
	applyPosteffectList_.emplace_back(id);
}

void PostEffectApplier::ApplyPostEffect()
{
	for (const auto& id : applyPosteffectList_)
	{
		// バッファ番号変更
		currentBufferNo_ = 1 - currentBufferNo_;

		// ポストエフェクト用
		// RESOURCE -> RENDER_TARGET
		SetResourceBarrier(postEffectBuffer_[currentBufferNo_].Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

		// レンダーターゲットの設定
		auto rtvHandle = HeapAllocator.GetRTVCPUAddress(rtvHeapNo_[currentBufferNo_]);

		// 画面クリア
		float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		DxCommand->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		DxCommand->ClearDepthStencilView(depthHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		DxCommand->OMSetRenderTargets(1, &rtvHandle, false, &depthHandle_);

		// もう片方のリソース番号
		int otherNo = 1 - currentBufferNo_;

		// リソースをテクスチャとして使用
		SetResourceBarrier(postEffectBuffer_[otherNo].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		auto& material = mesh_->GetMaterial();
		material.SetPSOID(psoList_.at(id));
		material.SetMaterial();

		DxCommand->SetGraphicsRootDescriptorTable(POST_EFFECT_TARGET_SIGNATURE_NO,
			HeapAllocator.GetCBV_SRV_UAVGPUAddress(srvHeapNo_[otherNo]));

		mesh_->SetMesh();

		DxCommand->DrawIndexedInstanced(mesh_->GetIndexNum(), 1, 0, 0, 0);
	}

	// ポストエフェクト適用リストリセット
	applyPosteffectList_.clear();
}

void PostEffectApplier::Render()
{
	// リソースをテクスチャとして使用
	SetResourceBarrier(postEffectBuffer_[currentBufferNo_].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	auto& material = mesh_->GetMaterial();
	material.SetPSOID(psoList_.at(POSTEFFECT_ID::DEFAULT));
	material.SetMaterial();

	DxCommand->SetGraphicsRootDescriptorTable(POST_EFFECT_TARGET_SIGNATURE_NO,
		HeapAllocator.GetCBV_SRV_UAVGPUAddress(srvHeapNo_[currentBufferNo_]));

	mesh_->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh_->GetIndexNum(), 1, 0, 0, 0);

}

ComPtr<ID3D12Resource> PostEffectApplier::GetBuffer()
{
	return postEffectBuffer_[currentBufferNo_];
}

D3D12_CPU_DESCRIPTOR_HANDLE PostEffectApplier::GetRTVHandle()
{
	return HeapAllocator.GetRTVCPUAddress(rtvHeapNo_[currentBufferNo_]);
}

void PostEffectApplier::PSOListSetting()
{
	psoList_[POSTEFFECT_ID::DEFAULT] = PSO_ID::PE_DEFAULT;
	psoList_[POSTEFFECT_ID::MONOCHROME] = PSO_ID::PE_MONOCHROME;
	psoList_[POSTEFFECT_ID::REVERSE] = PSO_ID::PE_REVERSE;
	psoList_[POSTEFFECT_ID::LOCKER] = PSO_ID::PE_LOCKER;
}

void PostEffectApplier::CreatePostEffectBuffer()
{
	auto resDesc = DXSwapChain::GetInstance().GetSwapChainBuffers()[0]->GetDesc();

	for (int i = 0; i < static_cast<int>(_countof(postEffectBuffer_)); i++)
	{
		// バッファをどのメモリプールに配置するか
		D3D12_HEAP_PROPERTIES heapProp =
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		// RT
		float clasClr[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(
			DXGI_FORMAT_R8G8B8A8_UNORM, clasClr
		);

		auto result = MainDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(postEffectBuffer_[i].ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			assert(0 && "ポストエフェクト用バッファ作成失敗");
		}

		// RTV
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		rtvHeapNo_[i] =
		HeapAllocator.CreateRTV(postEffectBuffer_[i].Get(), rtvDesc);

		// SRV
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		srvHeapNo_[i] =
		HeapAllocator.CreateSRV(postEffectBuffer_[i].Get(), srvDesc);
	}
}

void PostEffectApplier::CreateMesh()
{
	MESH_DATA data;
	data.verticesDefault2D = 
		{
		{ {-1.0f,-1.0f},	{0.0f,1.0f} },
		{ {-1.0f,1.0f},	{0.0f,0.0f} },
		{ {1.0 ,-1.0f},	{1.0f,1.0f} },
		{ {1.0 ,1.0f},	{1.0f,0.0f} }
	};
	data.indices = {
		0,1,2,2,1,3
	};
	mesh_ = make_unique<MeshDefault2D>(data);
}
