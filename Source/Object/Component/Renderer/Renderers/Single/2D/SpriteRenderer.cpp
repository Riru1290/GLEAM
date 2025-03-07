#include "../../../../../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../../../../../../Graphics/Material/Material.h"
#include "../../../../../../Common/Mesh/Meshes/MeshDefault2D.h"
#include "../../../../../../Resource/Texture/Texture.h"
#include "../../../../../../Object/Component/Common/Transform.h"
#include "SpriteRenderer.h"

namespace
{
	const int OBJECT_NUM_MAX = (100);
}

SpriteRenderer::SpriteRenderer()
{
	CreateConstantBuffer();
}

SpriteRenderer::~SpriteRenderer()
{
	localWorldConstBuffer_->Unmap(0, nullptr);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(localWorldHeapNo_);
}

void SpriteRenderer::SetTexture(const Texture& texture, int xNum, int yNum)
{
	divideXNum_ = xNum;
	divideYNum_ = yNum;

	// �e�N�X�`���S�̂̃T�C�Y���擾
	Size2D texSize = texture.GetDivideSize();

	// �e�Z���̃T�C�Y���v�Z
	float cellWidth = texSize.x / static_cast<float>(xNum);
	float cellHeight = texSize.y / static_cast<float>(yNum);

	texSize_ = { cellWidth ,cellHeight };

	// UV�P�ʂ̃Z���T�C�Y���v�Z
	float uvCellWidth = 1.0f / static_cast<float>(xNum);
	float uvCellHeight = 1.0f / static_cast<float>(yNum);

	uvOffset_ = UV(uvCellWidth, uvCellHeight);

	int index = 0;

	// �S�Z���̒��_�ƃC���f�b�N�X�f�[�^�𐶐�
	for (int y = 0; y < yNum; ++y)
	{
		for (int x = 0; x < xNum; ++x)
		{
			MESH_DATA meshData;
			auto& vertices = meshData.verticesDefault2D;
			auto& indices = meshData.indices;

			// �Z���̍���̍��W�i���[�J����ԁj
			float startX = -texSize.x / 2.0f + static_cast<float>(x) * cellWidth;
			float startY = texSize.y / 2.0f - static_cast<float>(y) * cellHeight;

			//// ���݂̃Z���̃C���f�b�N�X�I�t�Z�b�g
			//int vertexOffset = index * 4;

			// ���_�f�[�^�i���W��UV�j��ǉ�
			vertices.push_back({ {-cellWidth / 2.0f, cellHeight / 2.0f}, {x * uvCellWidth, y * uvCellHeight + uvCellHeight} });             //����
			vertices.push_back({ {-cellWidth / 2.0f, -cellHeight / 2.0f}, {x * uvCellWidth, y * uvCellHeight} });             //����
			vertices.push_back({ {cellWidth / 2.0f, cellHeight / 2.0f}, {x * uvCellWidth + uvCellWidth, y * uvCellHeight + uvCellHeight} }); //�E��
			vertices.push_back({ {cellWidth / 2.0f, -cellHeight / 2.0f}, {x * uvCellWidth + uvCellWidth, y * uvCellHeight} }); //�E��

			// �C���f�b�N�X�f�[�^��ǉ�
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(2);
			indices.push_back(1);
			indices.push_back(3);

			divideUV_.emplace_back(UV{ x * uvCellWidth, y * uvCellHeight });

			index++;
		}
	}

	MESH_DATA meshData;
	mesh_ = make_unique<MeshDefault2D>(meshData);
	mesh_->GetMaterial().SetPSOID(PSO_ID::SPRITE2D);
	mesh_->GetMaterial().SetDiffuseMap(texture);
	mesh_->RecreateVertexBuffer(OBJECT_NUM_MAX * 4);
	mesh_->RecreateIndexBuffer(OBJECT_NUM_MAX * 6);

	vector<unsigned short> indices;
	mesh_.get()->SetIndices(indices);
	vector<VERTEX_2D> vertices;
	mesh_.get()->SetVertices(vertices);
}

void SpriteRenderer::SetTransform(Transform& transform)
{
	transform_ = transform;
}

void SpriteRenderer::SetMeshAlpha(float alpha)
{
	localWorldMatrixMap_->alpha = alpha;
}

void SpriteRenderer::SetMeshLocalTransformMatrix(const XMMATRIX& matrix)
{
	localWorldMatrixMap_->local = matrix;
}

void SpriteRenderer::SetMeshWorldTransformMatrix(const XMMATRIX& matrix)
{
	localWorldMatrixMap_->world = matrix;
}

void SpriteRenderer::SetMeshTransformMatrixFromTransform(const Transform& transform)
{
	const Transform& t = transform;

	// ���[�J���s��
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

const Mesh& SpriteRenderer::GetMesh() const
{
	return *static_cast<const Mesh*>(mesh_.get());
}

void SpriteRenderer::RenderSprite(Transform transform, int no)
{
	renderInfo_.emplace_back(transform, no);
}

void SpriteRenderer::UpdateRenderer()
{
	UpdateLocalWorldMatrix();
}

void SpriteRenderer::RenderBegin()
{
	renderInfo_.clear();
	vector<VERTEX_2D> vertices;
	mesh_->SetVertices(vertices);
	vector<unsigned short> indices;
	mesh_->SetIndices(indices);
}

void SpriteRenderer::RenderEnd()
{
	RecreateVertex();

	if (mesh_->IsTrans())return;

	auto& material = mesh_->GetMaterial();
	material.SetMaterial();


	DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

	mesh_->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh_->GetIndexNum(), 1, 0, 0, 0);
}

void SpriteRenderer::RenderEndTrans()
{

	if (!mesh_->IsTrans())return;

	auto& material = mesh_->GetMaterial();
	material.SetMaterial();


	DxCommand->SetGraphicsRootDescriptorTable(1, HeapAllocator.GetCBV_SRV_UAVGPUAddress(localWorldHeapNo_));

	mesh_->SetMesh();

	DxCommand->DrawIndexedInstanced(mesh_->GetIndexNum(), 1, 0, 0, 0);
}

void SpriteRenderer::CreateConstantBuffer()
{
	// ���[�J���E���[���h�s��o�b�t�@�쐬
	CreateConstantBufferLocalWorldMatrix();
}

void SpriteRenderer::CreateConstantBufferLocalWorldMatrix()
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

void SpriteRenderer::UpdateLocalWorldMatrix()
{
	//if (!transform_.has_value())return;

	//Transform& t = transform_->get();

	//// ���[�J���s��
	//XMMATRIX local = XMMatrixIdentity();
	//Scale3D lScl = t.localScale;
	//local *= XMMatrixScaling(lScl.x, lScl.y, lScl.z);
	//Vector3 lRot = t.localQua.ToEuler();
	//local *= XMMatrixRotationY(lRot.y);
	//local *= XMMatrixRotationX(lRot.x);
	//local *= XMMatrixRotationZ(lRot.z);
	//Position3D lPos = t.localPos;
	//local *= XMMatrixTranslation(lPos.x, lPos.y, 0.0f);

	//SetMeshLocalTransformMatrix(local);

	//XMMATRIX world = XMMatrixIdentity();
	//Scale3D wScl = t.scale;
	//world *= XMMatrixScaling(wScl.x, wScl.y, wScl.z);
	//Vector3 wRot = t.qua.ToEuler();
	//world *= XMMatrixRotationY(wRot.y);
	//world *= XMMatrixRotationX(wRot.x);
	//world *= XMMatrixRotationZ(wRot.z);
	//Position3D wPos = t.pos;
	//world *= XMMatrixTranslation(wPos.x, wPos.y, 0.0f);

	//SetMeshWorldTransformMatrix(world);
}

void SpriteRenderer::RecreateVertex()
{
	auto& vertices = static_cast<MeshDefault2D*>(mesh_.get())->GetVertices();
	auto& indices = mesh_->GetIndices();

	for (const auto& [transform, no] : renderInfo_) {
		const auto& uv = divideUV_[no];
		const auto& pos = transform.pos;

		// ���݂̃Z���̃C���f�b�N�X�I�t�Z�b�g
		int vertexOffset = static_cast<int>(vertices.size());

		// ���_�f�[�^�i���W��UV�j��ǉ�
		vertices.push_back(
			{ {pos.x + (-texSize_.x / 2.0f),pos.y + (texSize_.y / 2.0f)},
			{uv.u, uv.v + uvOffset_.v} });             //����

		vertices.push_back(
			{ {pos.x + (-texSize_.x / 2.0f),pos.y + (-texSize_.y / 2.0f)},
			{uv.u, uv.v} });             //����

		vertices.push_back(
			{ {pos.x + (texSize_.x / 2.0f), pos.y + (texSize_.y / 2.0f)},
			{uv.u + uvOffset_.u, uv.v + uvOffset_.v} }); //�E��

		vertices.push_back(
			{ {pos.x + (texSize_.x / 2.0f), pos.y + (-texSize_.y / 2.0f)},
			{uv.u + uvOffset_.u, uv.v} }); //�E��

		// �C���f�b�N�X�f�[�^��ǉ�
		indices.emplace_back(0 + vertexOffset);
		indices.emplace_back(1 + vertexOffset);
		indices.emplace_back(2 + vertexOffset);
		indices.emplace_back(2 + vertexOffset);
		indices.emplace_back(1 + vertexOffset);
		indices.emplace_back(3 + vertexOffset);
	}
	mesh_->SetVertices(vertices);
	mesh_->SetIndices(indices);
}
