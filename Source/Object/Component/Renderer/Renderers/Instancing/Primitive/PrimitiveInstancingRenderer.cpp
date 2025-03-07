#include "../../../../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "PrimitiveInstancingRenderer.h"

PrimitiveInstancingRenderer::PrimitiveInstancingRenderer() : Renderer()
{
	CreateConstantBuffer();

	primitiveData_.reserve(static_cast<size_t>(INSTANCE_MAX_NUM));
}

PrimitiveInstancingRenderer::~PrimitiveInstancingRenderer()
{
	instanceBuffer_->Unmap(0, nullptr);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(instanceHeapNo_);
}

void PrimitiveInstancingRenderer::CreateConstantBuffer()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(((sizeof(PRIMITIVE_DATA) * INSTANCE_MAX_NUM) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(instanceBuffer_.ReleaseAndGetAddressOf())
	);

	instanceBuffer_->Map(0, nullptr, (void**)&instanceMap_);

	

	//D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	//desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//desc.NodeMask = 0;
	//desc.NumDescriptors = 1;    // ディスクリプタの数
	//desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//Device->CreateDescriptorHeap(
	//	&desc,
	//	IID_PPV_ARGS(instanceHeap_.ReleaseAndGetAddressOf())
	//);

	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	//cbvDesc.BufferLocation = instanceBuffer_->GetGPUVirtualAddress();
	//cbvDesc.SizeInBytes = static_cast<UINT>(instanceBuffer_->GetDesc().Width);
	//Device->CreateConstantBufferView(
	//	&cbvDesc,
	//	instanceHeap_->GetCPUDescriptorHandleForHeapStart()
	//);


	// シェーダリソースビュー（SRV）を作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;  // StructuredBufferなのでフォーマットは指定しません
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = INSTANCE_MAX_NUM; // インスタンスの数
	srvDesc.Buffer.StructureByteStride = sizeof(PRIMITIVE_DATA); // 構造体のサイズ
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	instanceHeapNo_ = HeapAllocator.CreateSRV(instanceBuffer_.Get(), srvDesc);

	//// SRVをディスクリプタヒープに作成
	//Device->CreateShaderResourceView(
	//	instanceBuffer_.Get(),
	//	&srvDesc,
	//	instanceHeap_->GetCPUDescriptorHandleForHeapStart()
	//);
}
