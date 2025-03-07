#include "MeshDefault3D.h"

MeshDefault3D::MeshDefault3D(MESH_DATA data, PSO_ID psoID) : Mesh(data, psoID)
{
	vertices_ = data.verticesDefault3D;

	CreateVertexBuffer();

	int size = static_cast<int>(vertices_.size());
	vector<Vector3> vec(vertices_.size());
#pragma omp parallel for
	for (int i = 0; i < size; i++)
	{
		Vector3 temp = {};
		temp.x = vertices_[i].pos.x;
		temp.y = vertices_[i].pos.y;
		temp.z = vertices_[i].pos.z;

		vec[i] = temp;
	}

	CreateBoundingSphere(vec);
}

MeshDefault3D::~MeshDefault3D()
{
	vertexBuffer_->Unmap(0, nullptr);
}

void MeshDefault3D::CreateVertexBuffer()
{
	auto vertexHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto vertexResDesc = CD3DX12_RESOURCE_DESC::Buffer(vertices_.size() * sizeof(VERTEX_3D));

	// 頂点バッファーの生成
	MainDevice->CreateCommittedResource(
		&vertexHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertexResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertexBuffer_.ReleaseAndGetAddressOf()));

	// 頂点情報のコピー
	vertexMap_ = nullptr;
	vertexBuffer_->Map(0, nullptr, (void**)&vertexMap_);

	// 頂点バッファービューの生成
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = static_cast<UINT>(vertices_.size() * sizeof(VERTEX_3D));
	vertexBufferView_.StrideInBytes = sizeof(VERTEX_3D);

	// データのコピー
	copy(begin(vertices_), end(vertices_), vertexMap_);
}

void MeshDefault3D::RecreateVertexBuffer(int vertexNum)
{
	auto vertexHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto vertexResDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexNum * sizeof(VERTEX_3D));

	// 頂点バッファーの生成
	MainDevice->CreateCommittedResource(
		&vertexHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertexResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertexBuffer_.ReleaseAndGetAddressOf()));

	// 頂点情報のコピー
	vertexMap_ = nullptr;
	vertexBuffer_->Map(0, nullptr, (void**)&vertexMap_);

	// 頂点バッファービューの生成
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = static_cast<UINT>(vertexNum * sizeof(VERTEX_3D));
	vertexBufferView_.StrideInBytes = sizeof(VERTEX_3D);

	// データのコピー
	copy(begin(vertices_), end(vertices_), vertexMap_);
}

void MeshDefault3D::SetVertex()
{
	DxCommand->IASetVertexBuffers(0, 1, &vertexBufferView_);                // 頂点バッファ
}

void MeshDefault3D::SetVertices(const variant<vector<VERTEX_2D>, vector<VERTEX_3D>, vector<VERTEX_ANIMATION>>& vertices)
{
	if (holds_alternative<vector<VERTEX_3D>>(vertices)) {

		vertices_ = get<vector<VERTEX_3D>>(vertices);
	}
	// データのコピー
	copy(begin(vertices_), end(vertices_), vertexMap_);
}
