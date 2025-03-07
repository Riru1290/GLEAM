#include "../../../../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../../../../../Graphics/Material/Material.h"
#include "../../../../../../Common/Mesh/Mesh.h"
#include "../../../../../../Common/Primitive/Sphere/Sphere.h"
#include "SphereInstancingRenderer.h"

SphereInstancingRenderer::SphereInstancingRenderer() : PrimitiveInstancingRenderer()
{
	sphere_ = make_unique<Sphere>();
}

SphereInstancingRenderer::~SphereInstancingRenderer()
{
}

void SphereInstancingRenderer::Draw(Position3D center, float r, unsigned int color, float alpha, int fillFlag)
{
	if (static_cast<int>(primitiveData_.size()) >= INSTANCE_MAX_NUM)
	{
		return;
	}

	// ���[���h���W�n
	XMMATRIX world = XMMatrixIdentity();
	float scale = r / Sphere::DEFAULT_RADIUS;
	world *= XMMatrixScaling(scale, scale, scale);
	world *= XMMatrixTranslation(center.x, center.y, center.z);


	PRIMITIVE_DATA data;
	data.world = world;
	data.color.x = static_cast<float>((color >> 16) & 0xFF) / 255.0f; // �Ԑ���
	data.color.y = static_cast<float>((color >> 8) & 0xFF) / 255.0f;  // �ΐ���
	data.color.z = static_cast<float>(color & 0xFF) / 255.0f;         // ����
	data.color.w = static_cast<float>(alpha) / 255.0f;

	primitiveData_.emplace_back(data);
}

void SphereInstancingRenderer::RenderBegin()
{
	primitiveData_.clear();
}

void SphereInstancingRenderer::RenderEnd()
{
	auto& mesh = sphere_->GetMesh();

	auto& material = mesh->GetMaterial();
	material.SetMaterial();

	// �v���~�e�B�u�f�[�^���X�V
	for (int i = 0; i < static_cast<int>(primitiveData_.size()); i++)
	{
		instanceMap_[i] = primitiveData_[i];
	}

	DxCommand->SetGraphicsRootDescriptorTable(8, HeapAllocator.GetCBV_SRV_UAVGPUAddress(instanceHeapNo_));

	mesh->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), static_cast<UINT>(primitiveData_.size()), 0, 0, 0);
}
