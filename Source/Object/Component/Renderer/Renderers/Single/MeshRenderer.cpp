#ifdef _DEBUG
#include "../../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../../../../Graphics/Material/Material.h"
#include "../../../../../Common/Mesh/Mesh.h"
#include "../../../../../Resource/Texture/Texture.h"
#include "../../../../../Common/Mesh/Meshes/MeshDefault3D.h"
#include "../../../../../Object/Component/Common/Transform.h"
#include "../../../../../Camera/Camera.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() : Renderer()
{
	CreateConstantBuffer();
}

MeshRenderer::~MeshRenderer()
{
	localWorldConstBuffer_->Unmap(0, nullptr);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(localWorldHeapNo_);
}

void MeshRenderer::Create2DMesh(const Texture& texture)
{
	Size2D texSize = texture.GetDivideSize() / 4.0f;
	MESH_DATA meshData;
	auto& vertices = meshData.verticesDefault3D;
	auto& indices = meshData.indices;
	vertices.resize(4);
	indices.resize(6);

	vertices[0] = {
		{ -(texSize.x / 2.0f),-(texSize.y / 2.0f),0.0f } ,
		{ 0.0f,0.0f,-1.0f },
		{ 0.0f,1.0f },
		{ 1.0f,0.0f,0.0f },
		{ 0.0f,-1.0f,0.0f }
	};	//左下
	vertices[1] = { 
		{ -(texSize.x / 2.0f),(texSize.y / 2.0f),0.0f } ,
		{ 0.0f,0.0f,-1.0f },
		{ 0.0f,0.0f } ,
		{ 1.0f,0.0f,0.0f },
		{ 0.0f,-1.0f,0.0f } 
	};	//左上
	vertices[2] = { 
		{ (texSize.x / 2.0f),-(texSize.y / 2.0f),0.0f } ,
		{ 0.0f,0.0f,-1.0f },
		{ 1.0f,1.0f } ,
		{ 1.0f,0.0f,0.0f },
		{ 0.0f,-1.0f,0.0f } 
	};	//右下
	vertices[3] = { 
		{ (texSize.x / 2.0f),(texSize.y / 2.0f),0.0f } ,
		{ 0.0f,0.0f,-1.0f },
		{ 1.0f,0.0f } ,
		{ 1.0f,0.0f,0.0f },
		{ 0.0f,-1.0f,0.0f } 
	};	//右上

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	mesh_ = make_unique<MeshDefault3D>(meshData);

	mesh_->GetMaterial().SetDiffuseMap(texture);

	boundingSphereOrigin_ = mesh_->GetBoundingSphere();
	boundingSphere_ = mesh_->GetBoundingSphere();
}

void MeshRenderer::Create2DMesh(Size2D size)
{
	Size2D texSize = size;
	MESH_DATA meshData;
	auto& vertices = meshData.verticesDefault3D;
	auto& indices = meshData.indices;
	vertices.resize(4);
	indices.resize(6);

	vertices[0] = {
		{ -(texSize.x / 2.0f),-(texSize.y / 2.0f),0.0f } ,
		{ 0.0f,0.0f,-1.0f },
		{ 0.0f,1.0f },
		{ 1.0f,0.0f,0.0f },
		{ 0.0f,-1.0f,0.0f }
	};	//左下
	vertices[1] = {
		{ -(texSize.x / 2.0f),(texSize.y / 2.0f),0.0f } ,
		{ 0.0f,0.0f,-1.0f },
		{ 0.0f,0.0f } ,
		{ 1.0f,0.0f,0.0f },
		{ 0.0f,-1.0f,0.0f }
	};	//左上
	vertices[2] = {
		{ (texSize.x / 2.0f),-(texSize.y / 2.0f),0.0f } ,
		{ 0.0f,0.0f,-1.0f },
		{ 1.0f,1.0f } ,
		{ 1.0f,0.0f,0.0f },
		{ 0.0f,-1.0f,0.0f }
	};	//右下
	vertices[3] = {
		{ (texSize.x / 2.0f),(texSize.y / 2.0f),0.0f } ,
		{ 0.0f,0.0f,-1.0f },
		{ 1.0f,0.0f } ,
		{ 1.0f,0.0f,0.0f },
		{ 0.0f,-1.0f,0.0f }
	};	//右上

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	mesh_ = make_unique<MeshDefault3D>(meshData);


	boundingSphereOrigin_ = mesh_->GetBoundingSphere();
	boundingSphere_ = mesh_->GetBoundingSphere();
}

void MeshRenderer::SetMesh(unique_ptr<Mesh> mesh)
{
	mesh_ = move(mesh);

	boundingSphereOrigin_ = mesh_->GetBoundingSphere();
	boundingSphere_ = mesh_->GetBoundingSphere();
}

void MeshRenderer::SetTransform(Transform& transform)
{
	transform_ = transform;
}

void MeshRenderer::SetMeshAlpha(float alpha)
{
	localWorldMatrixMap_->alpha = alpha;
}

void MeshRenderer::SetMeshLocalTransformMatrix(const XMMATRIX& matrix)
{
	localWorldMatrixMap_->local = matrix;
}

void MeshRenderer::SetMeshWorldTransformMatrix(const XMMATRIX& matrix)
{
	localWorldMatrixMap_->world = matrix;
}

void MeshRenderer::SetMeshTransformMatrixFromTransform(const Transform& transform)
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

void MeshRenderer::UpdateRenderer()
{
	UpdateLocalWorldMatrix();

	if (transform_.has_value())
	{
		UpdateBoundingSphere(transform_.value());
	}

//#ifdef _DEBUG
//	DebugRender.DrawSphere(
//		boundingSphere_.center, boundingSphere_.radius, 0xff0000, 255.0f, true);
//#endif
}

void MeshRenderer::RenderBegin()
{
}

void MeshRenderer::RenderEnd()
{
	if (mesh_->IsTrans() || !IsObjectInCameraFrustum(boundingSphere_))return;

	auto& material = mesh_->GetMaterial();
	material.SetMaterial();

	DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

	mesh_->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh_->GetIndexNum(), 1, 0, 0, 0);

}

void MeshRenderer::RenderEndTrans()
{
	if (!mesh_->IsTrans() || !IsObjectInCameraFrustum(boundingSphere_))return;

	auto& material = mesh_->GetMaterial();
	material.SetMaterial();

	DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

	mesh_->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh_->GetIndexNum(), 1, 0, 0, 0);

}

void MeshRenderer::CreateConstantBuffer()
{
	// ローカル・ワールド行列バッファ作成
	CreateConstantBufferLocalWorldMatrix();
}

void MeshRenderer::CreateConstantBufferLocalWorldMatrix()
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

void MeshRenderer::UpdateLocalWorldMatrix()
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
	local *= XMMatrixTranslation(lPos.x, lPos.y, lPos.z);

	SetMeshLocalTransformMatrix(local);

	XMMATRIX world = XMMatrixIdentity();
	Scale3D wScl = t.scale;
	world *= XMMatrixScaling(wScl.x, wScl.y, wScl.z);
	Vector3 wRot = t.qua.ToEuler();
	world *= XMMatrixRotationY(wRot.y);
	world *= XMMatrixRotationX(wRot.x);
	world *= XMMatrixRotationZ(wRot.z);
	Position3D wPos = t.pos;
	world *= XMMatrixTranslation(wPos.x, wPos.y, wPos.z);

	SetMeshWorldTransformMatrix(world);
}

void MeshRenderer::UpdateBoundingSphere(const Transform& transform)
{
	boundingSphere_.center = transform.pos +
		(transform.qua.PosAxis(boundingSphereOrigin_.center) * transform.scale.x);
	boundingSphere_.radius = boundingSphereOrigin_.radius * transform.scale.x;
}

bool MeshRenderer::IsObjectInCameraFrustum(const BoundingSphere& sphere) const
{
	return true;
	const auto& frustum = MainCamera.GetViewProjectionFrustum();

	for (int i = 0; i < 6; ++i) {
		const FrustumPlane& plane = frustum.planes[i];

		// 球の中心と平面の距離を計算
		float dist = plane.normal.x * sphere.center.x +
			plane.normal.y * sphere.center.y +
			plane.normal.z * sphere.center.z +
			plane.d;

		// 完全に裏側にある場合はカリング
		if (dist < -sphere.radius) {
			return false;
		}
	}
	return true; // すべての平面でカリングされなかったら描画対象
}
