#pragma once
#include "../../Common/Singleton/StaticSigleton.h"

class RTVHeap;
class DSVHeap;
class CBV_SRV_UAVHeap;

#define HeapAllocator DescriptorHeapAllocator::GetInstance()

/// @brief �f�B�X�N���v�^�q�[�v�Ǘ�
class DescriptorHeapAllocator final : public StaticSingleton<DescriptorHeapAllocator>
{
public:

	/// @brief �������
	void Terminate() override;

	void SetDescriptorHeaps();

	// SRV�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	/// @brief �w�背�W�X�^�ԍ��̃f�B�X�N���v�^�����
	/// @param registNo ���W�X�^�ԍ�
	void ReleaseDescriptorRTV(int registNo);

	/// @brief RTV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateRTV(ID3D12Resource* Buffer);

	/// @brief RTV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @param desc RTV�ݒ�
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateRTV(ID3D12Resource* Buffer, D3D12_RENDER_TARGET_VIEW_DESC desc);

	/// @brief CPU���A�h���X��Ԃ��֐�
	/// @param number �o�^�ԍ�
	/// @return CPU���A�h���X
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUAddress(int number);

	/// @brief GPU���A�h���X��Ԃ��֐�
	/// @param number �o�^�ԍ�
	/// @return GPU���A�h���X
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUAddress(int number);

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	// DSV�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	/// @brief �w�背�W�X�^�ԍ��̃f�B�X�N���v�^�����
	/// @param registNo ���W�X�^�ԍ�
	void ReleaseDescriptorDSV(int registNo);

	/// @brief DSV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateDSV(ID3D12Resource* Buffer);

	/// @brief DSV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @param desc DSV�ݒ�
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateDSV(ID3D12Resource* Buffer, D3D12_DEPTH_STENCIL_VIEW_DESC desc);

	/// @brief CPU���A�h���X��Ԃ��֐�
	/// @param number �o�^�ԍ�
	/// @return CPU���A�h���X
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUAddress(int number);

	/// @brief GPU���A�h���X��Ԃ��֐�
	/// @param number �o�^�ԍ�
	/// @return GPU���A�h���X
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUAddress(int number);

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	// CBV�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	/// @brief CBV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateCBV(ID3D12Resource* Buffer);

	/// @brief CBV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @param desc CBV�ݒ�
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateCBV(ID3D12Resource* Buffer, D3D12_CONSTANT_BUFFER_VIEW_DESC desc);

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	// SRV�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	/// @brief SRV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateSRV(ID3D12Resource* Buffer);

	/// @brief SRV�̍쐬
	/// @param Buffer �o�b�t�@�[�̃|�C���^
	/// @param desc SRV�ݒ�
	/// @return �q�[�v�̕R�Â���ꂽ�o�^�ԍ�
	[[nodiscard]] int CreateSRV(ID3D12Resource* Buffer, D3D12_SHADER_RESOURCE_VIEW_DESC desc);

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	// CBV,SRV,UAV�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	/// @brief �w�背�W�X�^�ԍ��̃f�B�X�N���v�^�����
	/// @param registNo ���W�X�^�ԍ�
	void ReleaseDescriptorCBV_SRV_UAV(int registNo);

	/// @brief CPU���A�h���X��Ԃ��֐�
	/// @param number �o�^�ԍ�
	/// @return CPU���A�h���X
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCBV_SRV_UAVCPUAddress(int number);

	/// @brief GPU���A�h���X��Ԃ��֐�
	/// @param number �o�^�ԍ�
	/// @return GPU���A�h���X
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetCBV_SRV_UAVGPUAddress(int number);

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

		// �e�X�g
	int GetRegisterNum();

private:

	DescriptorHeapAllocator();
	~DescriptorHeapAllocator();
	PROOF_OF_STATIC_SINGLETON(DescriptorHeapAllocator);

	unique_ptr<RTVHeap> rtvHeap_;					/// @brief RTV�p�f�B�X�N���v�^�q�[�v
	unique_ptr<DSVHeap> dsvHeap_;					/// @brief DSV�p�f�B�X�N���v�^�q�[�v
	unique_ptr<CBV_SRV_UAVHeap> cbv_srv_uavHeap_;	/// @brief CBV,SRV,UAV�p�f�B�X�N���v�^�q�[�v
};

