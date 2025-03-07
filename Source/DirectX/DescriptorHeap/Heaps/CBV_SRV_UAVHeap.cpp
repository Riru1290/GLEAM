#include "CBV_SRV_UAVHeap.h"

bool CBV_SRV_UAVHeap::Create(int useCount)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = useCount;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	auto result = MainDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));

	useCount_ = useCount;
	incrementSize_ = MainDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int i = 0; i < useCount; ++i) {
		registNumber_[i] = false;
	}

	return !(FAILED(result));
}

int CBV_SRV_UAVHeap::CreateCBV(ID3D12Resource* Buffer)
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

	// 定数バッファビュー（CBV）を作成
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = Buffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(Buffer->GetDesc().Width);
	MainDevice->CreateConstantBufferView(
		&cbvDesc,
		handle
	);

	return registNo;
}

int CBV_SRV_UAVHeap::CreateCBV(ID3D12Resource* buffer, D3D12_CONSTANT_BUFFER_VIEW_DESC desc)
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

	// 定数バッファビュー（CBV）を作成
	MainDevice->CreateConstantBufferView(
		&desc,
		handle
	);

	return registNo;
}

int CBV_SRV_UAVHeap::CreateSRV(ID3D12Resource* buffer)
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

	// シェーダリソースビュー（SRV）を作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = 1;
	srvDesc.Buffer.StructureByteStride = static_cast<UINT>(buffer->GetDesc().Width);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	// SRVをディスクリプタヒープに作成
	MainDevice->CreateShaderResourceView(
		buffer,
		&srvDesc,
		handle
	);

	return registNo;
}

int CBV_SRV_UAVHeap::CreateSRV(ID3D12Resource* buffer, D3D12_SHADER_RESOURCE_VIEW_DESC desc)
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

	// SRVをディスクリプタヒープに作成
	MainDevice->CreateShaderResourceView(
		buffer,
		&desc,
		handle
	);

	return registNo;
}
