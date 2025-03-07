#ifdef _DEBUG
#include "../../Graphics/DebugRender/DebugRenderManager.h"
#endif
#include "../../Graphics/Material/Material.h"
#include "../../Resource/ResourceManager.h"
#include "../../Resource/Texture/Texture.h"
#include "Mesh.h"

Mesh::Mesh(MESH_DATA data, PSO_ID psoID) :
	topology_(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	isTrans_(false)
{
	indices_ = data.indices;
	CreateIndexBuffer();

	material_ = make_shared<Material>(psoID);

	if (data.diffuseTexturePath != "")
	{
		material_->SetDiffuseMap(ResourceMng.LoadAdditionalTexture(data.diffuseTexturePath));
	}
	else
	{
		material_->SetDiffuseMap(ResourceMng.LoadAdditionalTexture(""));
	}

	if (data.specularTexturePath != "")
	{
		material_->SetSpecularMap(ResourceMng.LoadAdditionalTexture(data.specularTexturePath));
	}
	else
	{
		material_->SetSpecularMap(ResourceMng.LoadAdditionalTexture(""));
	}

	if (data.normalTexturePath != "")
	{
		material_->SetNormalMap(ResourceMng.LoadAdditionalTexture(data.normalTexturePath));
	}
	else
	{
		material_->SetNormalMap(ResourceMng.LoadAdditionalTexture(""));
	}

	if (data.aoTexturePath != "")
	{
		material_->SetAmbientOcclusionMap(ResourceMng.LoadAdditionalTexture(data.aoTexturePath));
	}
	else
	{
		material_->SetAmbientOcclusionMap(ResourceMng.LoadAdditionalTexture(""));
	}
}

Mesh::~Mesh()
{
	indexBuffer_->Unmap(0, nullptr);
	ClearConstantBuffer();
	ClearShaderResource();
}

void Mesh::RecreateIndexBuffer(int indexNum)
{
	auto indexHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto indexResDesc = CD3DX12_RESOURCE_DESC::Buffer(indexNum * sizeof(unsigned short));

	// インデックスバッファの生成
	MainDevice->CreateCommittedResource(
		&indexHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&indexResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(indexBuffer_.ReleaseAndGetAddressOf()));

	// インデックス情報のコピー
	indexMap_ = nullptr;
	indexBuffer_->Map(0, nullptr, (void**)&indexMap_);

	// インデックスバッファビューの生成
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.Format = DXGI_FORMAT_R16_UINT;
	indexBufferView_.SizeInBytes = static_cast<UINT>(indexNum * sizeof(unsigned short));

	// データのコピー
	copy(begin(indices_), end(indices_), indexMap_);
}

void Mesh::SetMesh()
{
	for (auto& [signatureNo, bufferPair] : constBuffers_)
	{
		auto& [buffer, heapNo] = bufferPair;
		DxCommand->SetGraphicsRootDescriptorTable(signatureNo, HeapAllocator.GetCBV_SRV_UAVGPUAddress(heapNo));
	}

	for (auto& [signatureNo, bufferPair] : shaderResource_)
	{
		auto& [buffer, heapNo] = bufferPair;
		DxCommand->SetGraphicsRootDescriptorTable(signatureNo, HeapAllocator.GetCBV_SRV_UAVGPUAddress(heapNo));
	}

	DxCommand->IASetPrimitiveTopology(topology_);		// プリミティブトポロジの設定 (三角ポリゴン)
	DxCommand->IASetIndexBuffer(&indexBufferView_);		// インデックスバッファ
	SetVertex();										// 頂点バッファ
}

void Mesh::SetMeshShadow()
{
	DxCommand->IASetPrimitiveTopology(topology_);		// プリミティブトポロジの設定 (三角ポリゴン)
	DxCommand->IASetIndexBuffer(&indexBufferView_);		// インデックスバッファ
	SetVertex();										// 頂点バッファ
}

void Mesh::ClearConstantBuffer()
{
	for (auto& [key, bufferPair] : constBuffers_)
	{
		auto& [buffer, heapNo] = bufferPair;
		buffer->Unmap(0, nullptr);

		HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(heapNo);
	}
	constBuffers_.clear();
}

void Mesh::ClearShaderResource()
{
	for (auto& [key, bufferPair] : shaderResource_)
	{
		auto& [buffer, heapNo] = bufferPair;

		HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(heapNo);
	}
	shaderResource_.clear();
}

void Mesh::SetIndices(vector<unsigned short>& indices)
{
	indices_ = indices;
	// データのコピー
	copy(begin(indices_), end(indices_), indexMap_);
}


void Mesh::CreateIndexBuffer()
{
	if (indices_.size() <= 0) {
		indices_.emplace_back(0);
	}

	auto indexHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto indexResDesc = CD3DX12_RESOURCE_DESC::Buffer(indices_.size() * sizeof(unsigned short));

	// インデックスバッファの生成
	MainDevice->CreateCommittedResource(
		&indexHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&indexResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(indexBuffer_.ReleaseAndGetAddressOf()));

	// インデックス情報のコピー
	indexMap_ = nullptr;
	indexBuffer_->Map(0, nullptr, (void**)&indexMap_);

	// インデックスバッファビューの生成
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.Format = DXGI_FORMAT_R16_UINT;
	indexBufferView_.SizeInBytes = static_cast<UINT>(indices_.size() * sizeof(unsigned short));

	// データのコピー
	copy(begin(indices_), end(indices_),indexMap_);
}

void Mesh::CreateBoundingSphere(const vector<Vector3>& vertices)
{
	// モデルの中心を計算
	Position3D high = vertices[0];
	Position3D low = vertices[0];
	Vector3 center = { 0.0f, 0.0f, 0.0f };
	for (const auto& vertex : vertices) {
		high.x = max(high.x, vertex.x);
		high.y = max(high.y, vertex.y);
		high.z = max(high.z, vertex.z);

		low.x = min(low.x, vertex.x);
		low.y = min(low.y, vertex.y);
		low.z = min(low.z, vertex.z);
	}
	center = (high + low) / 2.0f;

	// 半径を計算
	float radius = (center - high).Length();
	//for (const auto& vertex : vertices) {
	//	float distance = (vertex - center).Length(); // 中心から頂点までの距離
	//	radius = max(radius, distance);         // 最大距離を半径とする
	//}

	boundingSphere_ = { center,radius };
}
