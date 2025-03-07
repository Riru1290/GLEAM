#ifdef _DEBUG
#include "../../../../../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../../../../../Common/AABB/BoundingSphere.h"
#include "../../../../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../../../../../Graphics/Material/Material.h"
#include "../../../../../../Common/Mesh/Mesh.h"
#include "../../../../../../Resource/Model/Model.h"
#include "../../../../Common/Transform.h"
#include "../../../../../../Camera/Camera.h"
#include "../../../../../../Graphics/PipelineStateObject/PipelineStateObjectManager.h"
#include "../../../../../../Graphics/Shadow/ShadowRenderer.h"
#include "../../../../../../Graphics/Light/LightController.h"
#include "AnimationModelRenderer.h"

namespace
{
	// ボーン最大数
	const int BONE_MAX_NUM = (128);
}

AnimationModelRenderer::AnimationModelRenderer() : Renderer()
{
	CreateConstantBuffer();

	fill(begin(boneFrameMatrix), end(boneFrameMatrix), XMMatrixIdentity());
}

AnimationModelRenderer::~AnimationModelRenderer()
{
	localWorldConstBuffer_->Unmap(0, nullptr);
	boneConstBuffer_->Unmap(0, nullptr);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(localWorldHeapNo_);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(boneHeapNo_);
}

void AnimationModelRenderer::SetModel(const Model& model)
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

void AnimationModelRenderer::SetTransform(Transform& transform)
{
	transform_ = transform;
}

void AnimationModelRenderer::SetModelAlpha(float alpha)
{
	localWorldMatrixMap_->alpha = alpha;
}

void AnimationModelRenderer::SetModelLocalTransformMatrix(const XMMATRIX& matrix)
{
	localWorldMatrixMap_->local = matrix;
}

void AnimationModelRenderer::SetModelWorldTransformMatrix(const XMMATRIX& matrix)
{
	localWorldMatrixMap_->world = matrix;
}

void AnimationModelRenderer::SetModelBoneMatrix(const XMMATRIX* matrix, int boneNum)
{
	for (int i = 0; i < boneNum; i++)
	{
		boneMatrixMap_[i] = matrix[i];
	}
}

void AnimationModelRenderer::SetModelBoneMatrix(const XMMATRIX& matrix, int boneIndex)
{
	boneMatrixMap_[boneIndex] = matrix;
}

void AnimationModelRenderer::SetAnimBoneMatrix(const XMMATRIX& matrix, int boneIndex)
{
	animMatrix[boneIndex] = matrix;
}

void AnimationModelRenderer::MultiplyMatrixToModelBoneAndChildren(const XMMATRIX& matrix, int boneIndex)
{
	const auto& bone = model_->get().GetBones()[boneIndex];

	boneFrameMatrix[boneIndex] = matrix;


	//animMatrix[boneIndex] *= matrix;
	//boneMatrixMap_[boneIndex] = model_->GetBones()[boneIndex].globalBindInverseMat * animMatrix[boneIndex];


	// 子供ボーンにも適用
	const auto& chiledlen = model_->get().GetBones()[boneIndex].chiledlenBoneIndices;
	for (int child : chiledlen)
	{
		MultiplyMatrixToModelBoneAndChildren(matrix,child);
	}
}

XMMATRIX AnimationModelRenderer::GetLocalModelBoneMatrix(int boneIndex)
{
	return boneMatrixMap_[boneIndex];
}

XMMATRIX AnimationModelRenderer::GetGlobalModelBoneMatrix(int boneIndex)
{
	return boneMatrixMap_[boneIndex] * localWorldMatrixMap_->local * localWorldMatrixMap_->world;
}

XMMATRIX AnimationModelRenderer::GetLocalAnimBoneMatrix(int boneIndex)
{
	return animMatrix[boneIndex];
}

XMMATRIX AnimationModelRenderer::GetGlobalAnimBoneMatrix(int boneIndex)
{
	return animMatrix[boneIndex] * boneFrameMatrix[boneIndex] * localWorldMatrixMap_->local * localWorldMatrixMap_->world;
}

XMMATRIX AnimationModelRenderer::GetGlobalBoneMatrix(int boneIndex)
{
	return model_->get().GetBones()[boneIndex].transformLinkMat * boneFrameMatrix[boneIndex] * localWorldMatrixMap_->local * localWorldMatrixMap_->world;
}

XMMATRIX AnimationModelRenderer::GetBoneFrameMatrix(int boneIndex)
{
	return boneFrameMatrix[boneIndex];
}

void AnimationModelRenderer::UpdateRenderer()
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

void AnimationModelRenderer::RenderBegin()
{
}

void AnimationModelRenderer::RenderEnd()
{
	if (!model_.has_value())return;

	for (int index = 0;auto& mesh : model_->get().GetMeshes())
	{
		if (mesh->IsTrans() || !IsObjectInCameraFrustum(boundingSphere_[index]))continue;

		auto& material = mesh->GetMaterial();
		material.SetMaterial();

		DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

		DxCommand->SetGraphicsRootDescriptorTable(13, HeapAllocator.GetCBV_SRV_UAVGPUAddress(boneHeapNo_));

		mesh->SetMesh();

		DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), 1, 0, 0, 0);

		index++;
	}
}

void AnimationModelRenderer::RenderEndTrans()
{
	if (!model_.has_value())return;

	for (int index = 0; auto& mesh : model_->get().GetMeshes())
	{
		if (!mesh->IsTrans() || !IsObjectInCameraFrustum(boundingSphere_[index]))continue;

		auto& material = mesh->GetMaterial();
		material.SetMaterial();

		DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

		DxCommand->SetGraphicsRootDescriptorTable(13, HeapAllocator.GetCBV_SRV_UAVGPUAddress(boneHeapNo_));

		mesh->SetMesh();

		DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), 1, 0, 0, 0);


	}
}

void AnimationModelRenderer::RenderShadow()
{
	PSOMng.SetPSO(PSO_ID::SHADOW_ANIMATION);

	Shadow.SetShadowMapIndex(14);

	for (auto& mesh : model_->get().GetMeshes())
	{
		if (mesh->IsTrans())continue;

		DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

		DxCommand->SetGraphicsRootDescriptorTable(13, HeapAllocator.GetCBV_SRV_UAVGPUAddress(boneHeapNo_));

		mesh->SetMeshShadow();



		DxCommand->DrawIndexedInstanced(mesh->GetIndexNum(), 1, 0, 0, 0);
	}
}

void AnimationModelRenderer::UpdateLocalWorldMatrix()
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

void AnimationModelRenderer::CreateConstantBuffer()
{
	// ローカル・ワールド行列バッファ作成
	CreateConstantBufferLocalWorldMatrix();

	// ボーン行列バッファ作成
	CreateConstantBufferBoneMatrix();
}

void AnimationModelRenderer::CreateConstantBufferLocalWorldMatrix()
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

void AnimationModelRenderer::CreateConstantBufferBoneMatrix()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(((sizeof(XMMATRIX) * BONE_MAX_NUM) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(boneConstBuffer_.ReleaseAndGetAddressOf())
	);

	boneConstBuffer_->Map(0, nullptr, (void**)&boneMatrixMap_);

	XMMATRIX mat = XMMatrixIdentity();
	for (int i = 0; i < BONE_MAX_NUM; i++)
	{
		boneMatrixMap_[i] = mat;
	}

	boneHeapNo_ = HeapAllocator.CreateCBV(boneConstBuffer_.Get());
}

void AnimationModelRenderer::UpdateBoundingSphere(const Transform& transform)
{
	for (int i = 0; i < static_cast<int>(boundingSphere_.size()); i++)
	{
		boundingSphere_[i].center = transform.pos +
			(transform.qua.PosAxis(boundingSphereOrigin_[i].center * transform.scale.x));
		boundingSphere_[i].radius = boundingSphereOrigin_[i].radius * transform.scale.x;
	}
}

bool AnimationModelRenderer::IsObjectInCameraFrustum(const BoundingSphere& sphere) const
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
