#include "../../../../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../../../../../Graphics/Material/Material.h"
#include "../../../../../../Common/Mesh/Meshes/MeshDefault2D.h"
#include "../../../../../../Resource/Texture/Texture.h"
#include "../../../../../../Object/Component/Common/Transform.h"
#include "Object2DRenderer.h"

Object2DRenderer::Object2DRenderer()
{
	CreateConstantBuffer();
}

Object2DRenderer::~Object2DRenderer()
{
	localWorldConstBuffer_->Unmap(0, nullptr);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(localWorldHeapNo_);
}

void Object2DRenderer::SetTexture(const Texture& texture)
{
	Size2D texSize = texture.GetDivideSize();

	MESH_DATA meshData;
	auto& vertices = meshData.verticesDefault2D;
	auto& indices = meshData.indices;
	vertices.resize(4);
	indices.resize(6);

	vertices[0] = { {-(texSize.x / 2.0f),(texSize.y / 2.0f)} ,{0.0f,1.0f} };	//左下
	vertices[1] = { {-(texSize.x / 2.0f),-(texSize.y / 2.0f)} ,{0.0f,0.0f} };	//左上
	vertices[2] = { {(texSize.x / 2.0f),(texSize.y / 2.0f)} ,{1.0f,1.0f} };		//右下
	vertices[3] = { {(texSize.x / 2.0f),-(texSize.y / 2.0f)} ,{1.0f,0.0f} };	//右上

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	mesh_ = make_unique<MeshDefault2D>(meshData);
	mesh_->GetMaterial().SetDiffuseMap(texture);

	textureSize_ = texSize;
}

void Object2DRenderer::CreateMesh(Size2D size)
{
	MESH_DATA meshData;
	auto& vertices = meshData.verticesDefault2D;
	auto& indices = meshData.indices;
	vertices.resize(4);
	indices.resize(6);

	vertices[0] = { {-(size.x / 2.0f),(size.y / 2.0f)} ,{0.0f,1.0f} };	//左下
	vertices[1] = { {-(size.x / 2.0f),-(size.y / 2.0f)} ,{0.0f,0.0f} };	//左上
	vertices[2] = { {(size.x / 2.0f),(size.y / 2.0f)} ,{1.0f,1.0f} };		//右下
	vertices[3] = { {(size.x / 2.0f),-(size.y / 2.0f)} ,{1.0f,0.0f} };	//右上

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	mesh_ = make_unique<MeshDefault2D>(meshData);

	textureSize_ = size;
}

void Object2DRenderer::SetTransform(Transform& transform)
{
	transform_ = transform;
}

void Object2DRenderer::SetMeshAlpha(float alpha)
{
	localWorldMatrixMap_->alpha = alpha;
}

void Object2DRenderer::SetMeshLocalTransformMatrix(const XMMATRIX& matrix)
{
	localWorldMatrixMap_->local = matrix;
}

void Object2DRenderer::SetMeshWorldTransformMatrix(const XMMATRIX& matrix)
{
	localWorldMatrixMap_->world = matrix;
}

void Object2DRenderer::SetMeshTransformMatrixFromTransform(const Transform& transform)
{
	const Transform& t = transform;

	// ローカル行列
	XMMATRIX local = XMMatrixIdentity();
	Scale3D lScl = t.localScale;
	local *= XMMatrixScaling(lScl.x, lScl.y, lScl.z);
	Vector3 lRot = t.localQua.ToEuler();
	local *= XMMatrixRotationX(lRot.x);
	local *= XMMatrixRotationY(lRot.y);
	local *= XMMatrixRotationZ(lRot.z);
	Position3D lPos = t.localPos;
	local *= XMMatrixTranslation(lPos.x, lPos.y, lPos.z);

	SetMeshLocalTransformMatrix(local);

	XMMATRIX world = XMMatrixIdentity();
	Scale3D wScl = t.scale;
	world *= XMMatrixScaling(wScl.x, wScl.y, wScl.z);
	Vector3 wRot = t.qua.ToEuler();
	world *= XMMatrixRotationX(wRot.x);
	world *= XMMatrixRotationY(wRot.y);
	world *= XMMatrixRotationZ(wRot.z);
	Position3D wPos = t.pos;
	world *= XMMatrixTranslation(wPos.x, wPos.y, wPos.z);

	SetMeshWorldTransformMatrix(world);
}

void Object2DRenderer::UpdateRenderer()
{
	UpdateLocalWorldMatrix();
}

void Object2DRenderer::RenderBegin()
{

}

void Object2DRenderer::RenderEnd()
{
	if (mesh_ == nullptr || mesh_->IsTrans())return;

	auto& material = mesh_->GetMaterial();
	material.SetMaterial();

	DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

	mesh_->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh_->GetIndexNum(), 1, 0, 0, 0);
}

void Object2DRenderer::RenderEndTrans()
{
	if (mesh_ == nullptr || !mesh_->IsTrans())return;

	auto& material = mesh_->GetMaterial();
	material.SetMaterial();

	DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

	mesh_->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh_->GetIndexNum(), 1, 0, 0, 0);
}

void Object2DRenderer::CreateConstantBuffer()
{
	// ローカル・ワールド行列バッファ作成
	CreateConstantBufferLocalWorldMatrix();
}

void Object2DRenderer::CreateConstantBufferLocalWorldMatrix()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(LW_MATRIX_DATA) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(localWorldConstBuffer_.ReleaseAndGetAddressOf())
	);

	localWorldConstBuffer_->Map(0, nullptr, (void**)&localWorldMatrixMap_);

	XMMATRIX mat = XMMatrixIdentity();
	localWorldMatrixMap_->local = mat;
	localWorldMatrixMap_->world = mat;
	localWorldMatrixMap_->alpha = 1.0f;

	localWorldHeapNo_ = HeapAllocator.CreateCBV(localWorldConstBuffer_.Get());
}

void Object2DRenderer::UpdateLocalWorldMatrix()
{
	if (!transform_.has_value())return;

	Transform& t = transform_->get();

	// ローカル行列
	XMMATRIX local = XMMatrixIdentity();
	Scale3D lScl = t.localScale;
	local *= XMMatrixScaling(lScl.x, lScl.y, lScl.z);
	Vector3 lRot = t.localQua.ToEuler();
	local *= XMMatrixRotationY(lRot.y);
	local *= XMMatrixRotationX(lRot.x);
	local *= XMMatrixRotationZ(lRot.z);
	Position3D lPos = t.localPos;
	local *= XMMatrixTranslation(lPos.x, lPos.y, 0.0f);

	SetMeshLocalTransformMatrix(local);

	XMMATRIX world = XMMatrixIdentity();
	Scale3D wScl = t.scale;
	world *= XMMatrixScaling(wScl.x, wScl.y, wScl.z);
	Vector3 wRot = t.qua.ToEuler();
	world *= XMMatrixRotationY(wRot.y);
	world *= XMMatrixRotationX(wRot.x);
	world *= XMMatrixRotationZ(wRot.z);
	Position3D wPos = t.pos;
	world *= XMMatrixTranslation(wPos.x, wPos.y, 0.0f);

	SetMeshWorldTransformMatrix(world);
}
