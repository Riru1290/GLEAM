#include "Heaps/RTVHeap.h"
#include "Heaps/DSVHeap.h"
#include "Heaps/CBV_SRV_UAVHeap.h"
#include "DescriptorHeapAllocator.h"

namespace
{
	const int RTV_DISCRIPTOR_MAX_NUM = (15);
	const int DSV_DISCRIPTOR_MAX_NUM = (15);
	const int CBV_SRV_UAV_DISCRIPTOR_MAX_NUM = (300);
}

int DescriptorHeapAllocator::GetRegisterNum()
{
	int use = 0;
	use += rtvHeap_->GetRegisterNum();
	use += dsvHeap_->GetRegisterNum();
	use += cbv_srv_uavHeap_->GetRegisterNum();
	return use;
}

DescriptorHeapAllocator::DescriptorHeapAllocator()
{
	rtvHeap_ = make_unique<RTVHeap>();
	if (!rtvHeap_->Create(RTV_DISCRIPTOR_MAX_NUM))
	{
		assert(0 && "RTVHeapçÏê¨é∏îs");
	}

	dsvHeap_ = make_unique<DSVHeap>();
	if(!dsvHeap_->Create(DSV_DISCRIPTOR_MAX_NUM))
	{
		assert(0 && "DSVHeapçÏê¨é∏îs");
	}

	cbv_srv_uavHeap_ = make_unique<CBV_SRV_UAVHeap>();
	if (!cbv_srv_uavHeap_->Create(CBV_SRV_UAV_DISCRIPTOR_MAX_NUM))
	{
		assert(0 && "CBV_SRV_UAVHeapçÏê¨é∏îs");
	}
}

DescriptorHeapAllocator::~DescriptorHeapAllocator()
{
}

void DescriptorHeapAllocator::Terminate()
{

}

void DescriptorHeapAllocator::SetDescriptorHeaps()
{
	DxCommand->SetDescriptorHeaps(1, cbv_srv_uavHeap_->GetDescriptorHeap().GetAddressOf());
}

void DescriptorHeapAllocator::ReleaseDescriptorRTV(int registNo)
{
	rtvHeap_->ReleaseDescriptor(registNo);
}

int DescriptorHeapAllocator::CreateRTV(ID3D12Resource* Buffer)
{
	return rtvHeap_->CreateRTV(Buffer);
}

int DescriptorHeapAllocator::CreateRTV(ID3D12Resource* Buffer, D3D12_RENDER_TARGET_VIEW_DESC desc)
{
	return rtvHeap_->CreateRTV(Buffer, desc);
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapAllocator::GetRTVCPUAddress(int number)
{
	return rtvHeap_->GetCPUAddress(number);
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapAllocator::GetRTVGPUAddress(int number)
{
	return rtvHeap_->GetGPUAddress(number);
}

void DescriptorHeapAllocator::ReleaseDescriptorDSV(int registNo)
{
	dsvHeap_->ReleaseDescriptor(registNo);
}

int DescriptorHeapAllocator::CreateDSV(ID3D12Resource* Buffer)
{
	return dsvHeap_->CreateDSV(Buffer);
}

int DescriptorHeapAllocator::CreateDSV(ID3D12Resource* Buffer, D3D12_DEPTH_STENCIL_VIEW_DESC desc)
{
	return dsvHeap_->CreateDSV(Buffer, desc);
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapAllocator::GetDSVCPUAddress(int number)
{
	return dsvHeap_->GetCPUAddress(number);
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapAllocator::GetDSVGPUAddress(int number)
{
	return dsvHeap_->GetGPUAddress(number);
}

int DescriptorHeapAllocator::CreateCBV(ID3D12Resource* Buffer)
{
	return cbv_srv_uavHeap_->CreateCBV(Buffer);
}

int DescriptorHeapAllocator::CreateCBV(ID3D12Resource* Buffer, D3D12_CONSTANT_BUFFER_VIEW_DESC desc)
{
	return cbv_srv_uavHeap_->CreateCBV(Buffer, desc);
}

int DescriptorHeapAllocator::CreateSRV(ID3D12Resource* Buffer)
{
	return cbv_srv_uavHeap_->CreateSRV(Buffer);
}

int DescriptorHeapAllocator::CreateSRV(ID3D12Resource* Buffer, D3D12_SHADER_RESOURCE_VIEW_DESC desc)
{
	return cbv_srv_uavHeap_->CreateSRV(Buffer, desc);
}

void DescriptorHeapAllocator::ReleaseDescriptorCBV_SRV_UAV(int registNo)
{
	cbv_srv_uavHeap_->ReleaseDescriptor(registNo);
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapAllocator::GetCBV_SRV_UAVCPUAddress(int number)
{
	return cbv_srv_uavHeap_->GetCPUAddress(number);
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapAllocator::GetCBV_SRV_UAVGPUAddress(int number)
{
	return cbv_srv_uavHeap_->GetGPUAddress(number);
}
