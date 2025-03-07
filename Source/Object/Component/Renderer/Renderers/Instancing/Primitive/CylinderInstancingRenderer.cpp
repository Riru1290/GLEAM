#include "../../../../../../Utility/Math/Quaternion.h"
#include "../../../../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../../../../../Graphics/Material/Material.h"
#include "../../../../../../Common/Mesh/Mesh.h"
#include "../../../../../../Common/Primitive/Cylinder/Cylinder.h"
#include "CylinderInstancingRenderer.h"

CylinderInstancingRenderer::CylinderInstancingRenderer()
{
	cylinder_ = make_unique<Cylinder>();
}

CylinderInstancingRenderer::~CylinderInstancingRenderer()
{
}

void CylinderInstancingRenderer::Draw(Position3D pos, float height, float r, Vector3 angle, unsigned int color, float alpha, int fillFlag)
{
	if (static_cast<int>(primitiveData_.size()) >= INSTANCE_MAX_NUM)return;

	// �v���~�e�B�u�f�[�^
	PRIMITIVE_DATA data = {};

	// �F�v�Z
	data.color.x = static_cast<float>((color >> 16) & 0xFF) / 255.0f; // �Ԑ���
	data.color.y = static_cast<float>((color >> 8) & 0xFF) / 255.0f;  // �ΐ���
	data.color.z = static_cast<float>(color & 0xFF) / 255.0f;         // ����
	data.color.w = static_cast<float>(alpha) / 255.0f;

	// ���[���h���W�n
	XMMATRIX world = XMMatrixIdentity();

	// �傫��
	float scale = r / Cylinder::DEFAULT_RADIUS;

	// ��]�s��v�Z
	XMMATRIX rotMat = XMMatrixRotationX(angle.x) * XMMatrixRotationY(angle.y) * XMMatrixRotationZ(angle.z);


	// �J�v�Z���̉~��
	world = XMMatrixIdentity();
	float yScale = height / Cylinder::DEFAULT_HEIGHT;
	world *= XMMatrixScaling(scale, yScale, scale);
	world *= rotMat;
	world *= XMMatrixTranslation(pos.x, pos.y, pos.z);

	data.world = world;

	primitiveData_.emplace_back(data);
}

void CylinderInstancingRenderer::RenderBegin()
{
	primitiveData_.clear();
}

void CylinderInstancingRenderer::RenderEnd()
{
	auto& mesh = cylinder_->GetMesh();

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
