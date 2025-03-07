#ifdef _DEBUG
#include "../../../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../../../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../../../../../Graphics/Material/Material.h"
#include "../../../../../../Common/Mesh/Mesh.h"
#include "../../../../../../Resource/Model/Model.h"
#include "../../../../Common/Transform.h"
#include "../../../../../../Camera/Camera.h"
#include "../../../../../../Graphics/PipelineStateObject/PipelineStateObjectManager.h"
#include "../../../../../../Graphics/Shadow/ShadowRenderer.h"
#include "../../../../../../Graphics/Light/LightController.h"
#include "ModelRenderer.h"

ModelRenderer::ModelRenderer() : Renderer()
{
	CreateConstantBuffer();
}

ModelRenderer::~ModelRenderer()
{
	localWorldConstBuffer_->Unmap(0, nullptr);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(localWorldHeapNo_);
}

void ModelRenderer::SetModel(const Model& model)
{
	model_ = model;

	boundingSphereOrigin_.clear();
	boundingSphere_.clear();

	for (auto& mesh : model_->get().GetMeshes())
	{
		boundingSphereOrigin_.emplace_back(mesh.get()->GetBoundingSphere());
		boundingSphere_.emplace_back(mesh.get()->GetBoundingSphere());
	}
}

void ModelRenderer::SetTransform(Transform& transform)
{
	transform_ = transform;
}

void ModelRenderer::SetModelLocalTransformMatrix(const XMMATRIX& matrix)
{
	localWorldMatrixMap_->local = matrix;
}

void ModelRenderer::SetModelWorldTransformMatrix(const XMMATRIX& matrix)
{
	localWorldMatrixMap_->world = matrix;
}

void ModelRenderer::SetMeshTransformMatrixFromTransform(const Transform& transform)
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

	SetModelLocalTransformMatrix(local);

	XMMATRIX world = XMMatrixIdentity();
	Scale3D wScl = t.scale;
	world *= XMMatrixScaling(wScl.x, wScl.y, wScl.z);
	Vector3 wRot = t.qua.ToEuler();
	world *= XMMatrixRotationX(wRot.x);
	world *= XMMatrixRotationY(wRot.y);
	world *= XMMatrixRotationZ(wRot.z);
	Position3D wPos = t.pos;
	world *= XMMatrixTranslation(wPos.x, wPos.y, wPos.z);

	SetModelWorldTransformMatrix(world);
}

void ModelRenderer::SetModelAlpha(float alpha)
{
	localWorldMatrixMap_->alpha = alpha;
}

void ModelRenderer::UpdateRenderer()
{
	UpdateLocalWorldMatrix();

	if (transform_.has_value())
	{
		UpdateBoundingSphere(transform_.value());
	}

//#ifdef _DEBUG
//	for (auto& sphere : boundingSphere_)
//	{
//		DebugRender.DrawSphere(
//			sphere.center, sphere.radius, 0xff0000, 255.0f, true);
//	}
//#endif
}

void ModelRenderer::RenderBegin()
{
}

void ModelRenderer::RenderEnd()
{
	if (!model_.has_value())return;

	for (int index = 0;auto& mesh : model_->get().GetMeshes())
	{
		if (mesh->IsTrans() || !IsObjectInCameraFrustum(boundingSphere_[index]))continue;

		auto& material = mesh->GetMaterial();
		material.SetMaterial();

		DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

		mesh->SetMesh();

		DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), 1, 0, 0, 0);

		index++;
	}
}

void ModelRenderer::RenderEndTrans()
{
	if (!model_.has_value())return;

	for (int index = 0; auto& mesh : model_->get().GetMeshes())
	{
		if (!mesh->IsTrans() || !IsObjectInCameraFrustum(boundingSphere_[index]))continue;

		auto& material = mesh->GetMaterial();
		material.SetMaterial();

		DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

		mesh->SetMesh();

		DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), 1, 0, 0, 0);

		index++;
	}
}

void ModelRenderer::RenderShadow()
{
	if (!model_.has_value())return;

	PSOMng.SetPSO(PSO_ID::SHADOW_DEFAULT);
	Shadow.SetShadowMapIndex(13);
	Light.SetBeforeDraw();

	for (auto& mesh : model_->get().GetMeshes())
	{
		if (mesh->IsTrans())continue;

		DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

		mesh->SetMeshShadow();

		DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), 1, 0, 0, 0);
	}
}

void ModelRenderer::UpdateLocalWorldMatrix()
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

	SetModelLocalTransformMatrix(local);

	XMMATRIX world = XMMatrixIdentity();
	Scale3D wScl = t.scale;
	world *= XMMatrixScaling(wScl.x, wScl.y, wScl.z);
	Vector3 wRot = t.qua.ToEuler();
	world *= XMMatrixRotationY(wRot.y);
	world *= XMMatrixRotationX(wRot.x);
	world *= XMMatrixRotationZ(wRot.z);
	Position3D wPos = t.pos;
	world *= XMMatrixTranslation(wPos.x, wPos.y, wPos.z);

	SetModelWorldTransformMatrix(world);
}

void ModelRenderer::CreateConstantBuffer()
{
	// ローカル・ワールド行列バッファ作成
	CreateConstantBufferLocalWorldMatrix();
}

void ModelRenderer::CreateConstantBufferLocalWorldMatrix()
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

void ModelRenderer::UpdateBoundingSphere(const Transform& transform)
{
	for (int i = 0; i < static_cast<int>(boundingSphere_.size()); i++)
	{
		boundingSphere_[i].center = transform.pos +
			(transform.qua.PosAxis(boundingSphereOrigin_[i].center * transform.scale.x));
		boundingSphere_[i].radius = boundingSphereOrigin_[i].radius * transform.scale.x;
	}
}

bool ModelRenderer::IsObjectInCameraFrustum(const BoundingSphere& sphere) const
{
	// ToDo フラスタムカリングを実装
	return true;

	const auto& frustum = MainCamera.GetViewProjectionFrustum();

	XMFLOAT3 center = { sphere.center.x,sphere.center.y,sphere.center.z };

	// ワールド空間の球の中心をビュー空間へ変換
	XMVECTOR worldCenter = XMLoadFloat3(&center);
	XMVECTOR viewCenter = XMVector3TransformCoord(worldCenter, MainCamera.GetViewMatrix());

	XMFLOAT3 transformedCenter;
	XMStoreFloat3(&transformedCenter, viewCenter);

	for (int i = 0; i < 6; ++i) {
		const FrustumPlane& plane = frustum.planes[i];

		// 球の中心と平面の距離を計算
		float dist = plane.normal.x * transformedCenter.x +
			plane.normal.y * transformedCenter.y +
			plane.normal.z * transformedCenter.z +
			plane.d;

		// 完全に裏側にある場合はカリング
		if (dist < -sphere.radius) {
			return false;
		}
	}
	return true; // すべての平面でカリングされなかったら描画対象
}
