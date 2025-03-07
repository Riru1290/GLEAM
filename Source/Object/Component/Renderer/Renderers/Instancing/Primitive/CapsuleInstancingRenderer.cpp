#include "../../../../../../Utility/Math/Quaternion.h"
#include "../../../../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../../../../../Graphics/Material/Material.h"
#include "../../../../../../Common/Mesh/Mesh.h"
#include "../../../../../../Common/Primitive/Capsule/Capsule.h"
#include "CapsuleInstancingRenderer.h"

CapsuleInstancingRenderer::CapsuleInstancingRenderer() : PrimitiveInstancingRenderer()
{
	CreateConstantBufferSemiCircleTop();
	CreateConstantBufferSemiCircleDown();
	capsule_ = make_unique<Capsule>();
}

CapsuleInstancingRenderer::~CapsuleInstancingRenderer()
{
	capTopBuffer_->Unmap(0, nullptr);
	capDownBuffer_->Unmap(0, nullptr);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(capTopHeapNo_);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(capDownHeapNo_);
}

void CapsuleInstancingRenderer::Draw(Position3D pos1, Position3D pos2, float r, unsigned int color, float alpha, int fillFlag)
{

	if (static_cast<int>(primitiveData_.size()) >= INSTANCE_MAX_NUM)return;

	// �v���~�e�B�u�f�[�^
	PRIMITIVE_DATA data = {};

	// �F�v�Z
	data.color.x = static_cast<float>((color >> 16) & 0xFF) / 255.0f; // �Ԑ���
	data.color.y = static_cast<float>((color >> 8) & 0xFF) / 255.0f;  // �ΐ���
	data.color.z = static_cast<float>(color & 0xFF) / 255.0f;         // ����
	data.color.w = static_cast<float>(alpha) / 255.0f;

	// ���S���W
	Position3D centerPos = Lerp(pos1, pos2, 0.5f);

	// ���[���h���W�n
	XMMATRIX world = XMMatrixIdentity();

	// �傫��
	float scale = 0.0f;

	// ��]�s��v�Z
	Vector3 vec1to2 = Quaternion::LookRotation((pos1 - pos2).Normalized()).ToEuler();
	Vector3 defVec = Quaternion::LookRotation((Position3D(0.0f, Capsule::DEFAULT_HEIGHT / 2.0f, 0.0f) - Position3D(0.0f, -Capsule::DEFAULT_HEIGHT / 2.0f, 0.0f)).Normalized()).ToEuler();
	Vector3 rot = vec1to2 - defVec;
	XMMATRIX rotMat = XMMatrixRotationX(rot.x) * XMMatrixRotationY(rot.y)* XMMatrixRotationZ(rot.z);


	// �J�v�Z���̔��~(��)
	scale = r / Capsule::DEFAULT_RADIUS;
	world *= XMMatrixScaling(scale, scale, scale);
	world *= rotMat;
	Position3D movePowTop = pos1 + Quaternion::PosAxis(Quaternion::Euler(rot), Position3D(0.0f, -Capsule::DEFAULT_HEIGHT, 0.0f));

	world *= XMMatrixTranslation(movePowTop.x, movePowTop.y, movePowTop.z);

	data.world = world;
	capTopData_.emplace_back(data);

	// �J�v�Z���̔��~(��)
	world = XMMatrixIdentity();
	world *= XMMatrixScaling(scale, scale, scale);
	world *= rotMat;
	Position3D movePowDown = pos2 + Quaternion::PosAxis(Quaternion::Euler(rot), Position3D(0.0f, Capsule::DEFAULT_HEIGHT, 0.0f));
	world *= XMMatrixTranslation(movePowDown.x, movePowDown.y, movePowDown.z);

	data.world = world;
	capDownData_.emplace_back(data);


	// �J�v�Z���̉~��
	world = XMMatrixIdentity();
	float yScale = (pos2 - pos1).Length() / Capsule::DEFAULT_HEIGHT;
	world *= XMMatrixScaling(scale, yScale, scale);
	world *= rotMat;
	world *= XMMatrixTranslation(centerPos.x, centerPos.y, centerPos.z);

	data.world = world;

	primitiveData_.emplace_back(data);

}

void CapsuleInstancingRenderer::RenderBegin()
{
	primitiveData_.clear();
	capTopData_.clear();
	capDownData_.clear();
}

void CapsuleInstancingRenderer::RenderEnd()
{
	DrawCapsuleTop();
	DrawCapsuleMiddle();
	DrawCapsuleDown();
}

void CapsuleInstancingRenderer::DrawCapsuleTop()
{
	auto& mesh = capsule_->GetMeshes()[0];

	auto& material = mesh->GetMaterial();
	material.SetMaterial();

	// �v���~�e�B�u�f�[�^���X�V
	for (int i = 0; i < static_cast<int>(capTopData_.size()); i++)
	{
		capTopMap_[i] = capTopData_[i];
	}

	//CommandList->SetDescriptorHeaps(1, instanceHeapCapT_.GetAddressOf());
	DxCommand->SetGraphicsRootDescriptorTable(8, HeapAllocator.GetCBV_SRV_UAVGPUAddress(capTopHeapNo_));

	mesh->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), static_cast<UINT>(capTopData_.size()), 0, 0, 0);


}

void CapsuleInstancingRenderer::DrawCapsuleMiddle()
{
	auto& mesh = capsule_->GetMeshes()[1];

	auto& material = mesh->GetMaterial();
	material.SetMaterial();

	// �v���~�e�B�u�f�[�^���X�V
	for (int i = 0; i < static_cast<int>(primitiveData_.size()); i++)
	{
		instanceMap_[i] = primitiveData_[i];
	}

	//CommandList->SetDescriptorHeaps(1, instanceHeap_.GetAddressOf());
	DxCommand->SetGraphicsRootDescriptorTable(8, HeapAllocator.GetCBV_SRV_UAVGPUAddress(instanceHeapNo_));

	mesh->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), static_cast<UINT>(primitiveData_.size()), 0, 0, 0);
}

void CapsuleInstancingRenderer::DrawCapsuleDown()
{
	auto& mesh = capsule_->GetMeshes()[2];

	auto& material = mesh->GetMaterial();
	material.SetMaterial();

	// �v���~�e�B�u�f�[�^���X�V
	for (int i = 0; i < static_cast<int>(capDownData_.size()); i++)
	{
		capDownMap_[i] = capDownData_[i];
	}

	//CommandList->SetDescriptorHeaps(1, instanceHeapCapD_.GetAddressOf());
	DxCommand->SetGraphicsRootDescriptorTable(8, HeapAllocator.GetCBV_SRV_UAVGPUAddress(capDownHeapNo_));
	//CommandList->SetGraphicsRootDescriptorTable(7, instanceHeapCapD_->GetGPUDescriptorHandleForHeapStart());

	mesh->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), static_cast<UINT>(capDownData_.size()), 0, 0, 0);
}

void CapsuleInstancingRenderer::CreateConstantBufferSemiCircleTop()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(((sizeof(PRIMITIVE_DATA) * INSTANCE_MAX_NUM) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(capTopBuffer_.ReleaseAndGetAddressOf())
	);

	capTopBuffer_->Map(0, nullptr, (void**)&capTopMap_);

	//D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	//desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//desc.NodeMask = 0;
	//desc.NumDescriptors = 1;    // �f�B�X�N���v�^�̐�
	//desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//Device->CreateDescriptorHeap(
	//	&desc,
	//	IID_PPV_ARGS(instanceHeapCapT_.ReleaseAndGetAddressOf())
	//);

	// �V�F�[�_���\�[�X�r���[�iSRV�j���쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;  // StructuredBuffer�Ȃ̂Ńt�H�[�}�b�g�͎w�肵�܂���
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = INSTANCE_MAX_NUM; // �C���X�^���X�̐�
	srvDesc.Buffer.StructureByteStride = sizeof(PRIMITIVE_DATA); // �\���̂̃T�C�Y
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	capTopHeapNo_ = HeapAllocator.CreateSRV(capTopBuffer_.Get(), srvDesc);

	//// SRV���f�B�X�N���v�^�q�[�v�ɍ쐬
	//Device->CreateShaderResourceView(
	//	capTopBuffer_.Get(),
	//	&srvDesc,
	//	instanceHeapCapT_->GetCPUDescriptorHandleForHeapStart()
	//);
}

void CapsuleInstancingRenderer::CreateConstantBufferSemiCircleDown()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(((sizeof(PRIMITIVE_DATA) * INSTANCE_MAX_NUM) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(capDownBuffer_.ReleaseAndGetAddressOf())
	);

	capDownBuffer_->Map(0, nullptr, (void**)&capDownMap_);

	//D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	//desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//desc.NodeMask = 0;
	//desc.NumDescriptors = 1;    // �f�B�X�N���v�^�̐�
	//desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//Device->CreateDescriptorHeap(
	//	&desc,
	//	IID_PPV_ARGS(instanceHeapCapD_.ReleaseAndGetAddressOf())
	//);

	// �V�F�[�_���\�[�X�r���[�iSRV�j���쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;  // StructuredBuffer�Ȃ̂Ńt�H�[�}�b�g�͎w�肵�܂���
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = INSTANCE_MAX_NUM; // �C���X�^���X�̐�
	srvDesc.Buffer.StructureByteStride = sizeof(PRIMITIVE_DATA); // �\���̂̃T�C�Y
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	capDownHeapNo_ = HeapAllocator.CreateSRV(capDownBuffer_.Get(), srvDesc);

	//// SRV���f�B�X�N���v�^�q�[�v�ɍ쐬
	//Device->CreateShaderResourceView(
	//	capDownBuffer_.Get(),
	//	&srvDesc,
	//	instanceHeapCapD_->GetCPUDescriptorHandleForHeapStart()
	//);
}
