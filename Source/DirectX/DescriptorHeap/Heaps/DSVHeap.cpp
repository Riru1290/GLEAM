#include "DSVHeap.h"

bool DSVHeap::Create(int useCount)
{
	// 深度バッファービュー
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = useCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	auto result = MainDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));

	useCount_ = useCount;
	incrementSize_ = MainDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	for (int i = 0; i < useCount; ++i) {
		registNumber_[i] = false;
	}

	return !(FAILED(result));
}

int DSVHeap::CreateDSV(ID3D12Resource* Buffer)
{
	int registNo = -1;

	// 使用されていない領域を検索
	for (auto& [no, useFlag] : registNumber_)
	{
		if (useFlag)continue;

		registNo = no;
		useFlag = true;
		break;
	}

	if (registNo == -1)
	{
		assert(0 && "確保済みのヒープ領域を超えました");
		return -1;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)registNo * incrementSize_;

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	MainDevice->CreateDepthStencilView(Buffer, &dsvDesc, handle);

	return registNo;
}

int DSVHeap::CreateDSV(ID3D12Resource* Buffer, D3D12_DEPTH_STENCIL_VIEW_DESC desc)
{
	int registNo = -1;

	// 使用されていない領域を検索
	for (auto& [no, useFlag] : registNumber_)
	{
		if (useFlag)continue;

		registNo = no;
		useFlag = true;
		break;
	}

	if (registNo == -1)
	{
		assert(0 && "確保済みのヒープ領域を超えました");
		return -1;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)registNo * incrementSize_;

	MainDevice->CreateDepthStencilView(Buffer, &desc, handle);

	return registNo;
}
