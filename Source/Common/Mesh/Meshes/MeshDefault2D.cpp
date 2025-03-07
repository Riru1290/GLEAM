#include "MeshDefault2D.h"

MeshDefault2D::MeshDefault2D(MESH_DATA data, PSO_ID psoID) : Mesh(data, psoID)
{
	vertices_ = data.verticesDefault2D;

	CreateVertexBuffer();
}

MeshDefault2D::~MeshDefault2D()
{
	vertexBuffer_->Unmap(0, nullptr);
}

void MeshDefault2D::CreateVertexBuffer()
{
	if (vertices_.size() <= 0) {
		vertices_.emplace_back(VERTEX_2D());
	}

	auto vertexHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto vertexResDesc = CD3DX12_RESOURCE_DESC::Buffer(vertices_.size() * sizeof(VERTEX_2D));

	// ���_�o�b�t�@�[�̐���
	MainDevice->CreateCommittedResource(
		&vertexHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertexResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertexBuffer_.ReleaseAndGetAddressOf()));

	// ���_���̃R�s�[
	vertexMap_ = nullptr;
	vertexBuffer_->Map(0, nullptr, (void**)&vertexMap_);

	// ���_�o�b�t�@�[�r���[�̐���
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = static_cast<UINT>(vertices_.size() * sizeof(VERTEX_2D));
	vertexBufferView_.StrideInBytes = sizeof(VERTEX_2D);

	// �f�[�^�̃R�s�[
	copy(begin(vertices_), end(vertices_), vertexMap_);
}

void MeshDefault2D::RecreateVertexBuffer(int vertexNum)
{
	auto vertexHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto vertexResDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexNum * sizeof(VERTEX_2D));

	// ���_�o�b�t�@�[�̐���
	MainDevice->CreateCommittedResource(
		&vertexHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertexResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertexBuffer_.ReleaseAndGetAddressOf()));

	// ���_���̃R�s�[
	vertexMap_ = nullptr;
	vertexBuffer_->Map(0, nullptr, (void**)&vertexMap_);

	// ���_�o�b�t�@�[�r���[�̐���
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = static_cast<UINT>(vertexNum * sizeof(VERTEX_2D));
	vertexBufferView_.StrideInBytes = sizeof(VERTEX_2D);

	// �f�[�^�̃R�s�[
	copy(begin(vertices_), end(vertices_), vertexMap_);
}

void MeshDefault2D::SetVertex()
{
	DxCommand->IASetVertexBuffers(0, 1, &vertexBufferView_);                // ���_�o�b�t�@
}

void MeshDefault2D::SetVertices(const variant<vector<VERTEX_2D>, vector<VERTEX_3D>, vector<VERTEX_ANIMATION>>& vertices)
{
	if (holds_alternative<vector<VERTEX_2D>>(vertices)) {

		vertices_ = get<vector<VERTEX_2D>>(vertices);
	}
	// �f�[�^�̃R�s�[
	copy(begin(vertices_), end(vertices_), vertexMap_);
}
