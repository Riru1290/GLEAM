#include "../../../../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../../../../../Graphics/Material/Material.h"
#include "../../../../../../Common/Mesh/Mesh.h"
#include "../../../../../../Common/Primitive/Box/Box.h"
#include "BoxInstancingRenderer.h"

BoxInstancingRenderer::BoxInstancingRenderer()
{
	box_ = make_unique<Box>();
}

BoxInstancingRenderer::~BoxInstancingRenderer()
{
}

void BoxInstancingRenderer::Draw(Position3D Pos1, Position3D Pos2, unsigned int color, float alpha, int fillFlag)
{
	if (static_cast<int>(primitiveData_.size()) >= INSTANCE_MAX_NUM)
	{
		return;
	}

	// ワールド座標系
	XMMATRIX world = XMMatrixIdentity();
	float scaleX = fabsf(Pos1.x - Pos2.x) / Box::DEFAULT_SIDE_LENGTH;
	float scaleY = fabsf(Pos1.y - Pos2.y) / Box::DEFAULT_SIDE_LENGTH;
	float scaleZ = fabsf(Pos1.z - Pos2.z) / Box::DEFAULT_SIDE_LENGTH;
	world *= XMMatrixScaling(scaleX, scaleY, scaleZ);

	Position3D center = (Pos1 + Pos2) / 2.0f;
	world *= XMMatrixTranslation(center.x, center.y, center.z);


	PRIMITIVE_DATA data;
	data.world = world;
	data.color.x = static_cast<float>((color >> 16) & 0xFF) / 255.0f; // 赤成分
	data.color.y = static_cast<float>((color >> 8) & 0xFF) / 255.0f;  // 緑成分
	data.color.z = static_cast<float>(color & 0xFF) / 255.0f;         // 青成分
	data.color.w = static_cast<float>(alpha) / 255.0f;

	primitiveData_.emplace_back(data);
}

void BoxInstancingRenderer::RenderBegin()
{
	primitiveData_.clear();
}

void BoxInstancingRenderer::RenderEnd()
{
	auto& mesh = box_->GetMesh();

	auto& material = mesh->GetMaterial();
	material.SetMaterial();

	// プリミティブデータを更新
	for (int i = 0; i < static_cast<int>(primitiveData_.size()); i++)
	{
		instanceMap_[i] = primitiveData_[i];
	}

	//CommandList->SetDescriptorHeaps(1, instanceHeap_.GetAddressOf());
	DxCommand->SetGraphicsRootDescriptorTable(8, HeapAllocator.GetCBV_SRV_UAVGPUAddress(instanceHeapNo_));

	mesh->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), static_cast<UINT>(primitiveData_.size()), 0, 0, 0);
}
