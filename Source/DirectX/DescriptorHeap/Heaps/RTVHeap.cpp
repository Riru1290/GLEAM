#include "RTVHeap.h"

bool RTVHeap::Create(int useCount)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// レンダーターゲットとして使う
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = useCount;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// 特に指定なし

	auto result = MainDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));

	useCount_ = useCount;
	incrementSize_ = MainDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (int i = 0; i < useCount; ++i) {
		registNumber_[i] = false;
	}

	return !(FAILED(result));
}

int RTVHeap::CreateRTV(ID3D12Resource* Buffer)
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

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	MainDevice->CreateRenderTargetView(Buffer, &rtvDesc, handle);

	return registNo;
}

int RTVHeap::CreateRTV(ID3D12Resource* Buffer, D3D12_RENDER_TARGET_VIEW_DESC desc)
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

	MainDevice->CreateRenderTargetView(Buffer, &desc, handle);

	return registNo;
}
