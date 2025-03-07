#include "DescriptorHeap.h"

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUAddress(int number)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)incrementSize_ * number;
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUAddress(int number)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = heap_->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)incrementSize_ * number;
	return handle;
}

void DescriptorHeap::ReleaseDescriptor(int registNo)
{
	if (!registNumber_.contains(registNo))return;

	registNumber_.at(registNo) = false;
}
