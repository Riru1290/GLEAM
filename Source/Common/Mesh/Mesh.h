#pragma once
#include "MeshData.h"
#include "../AABB/BoundingSphere.h"
#include "../../Graphics/PipelineStateObject/PipelineStateObjectID.h"
#include "../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"


class Material;
class Transform;

/// @brief ���b�V��
class Mesh
{
public:

	[[nodiscard]] Mesh(MESH_DATA data,PSO_ID psoID = PSO_ID::DEFAULT_3D);
	virtual ~Mesh();

    /// @brief ���_�o�b�t�@�č쐬
    virtual void RecreateVertexBuffer(int vertexNum) = 0;

    /// @brief �C���f�b�N�X�o�b�t�@�č쐬
    void RecreateIndexBuffer(int indexNum);

    /// @brief ���b�V���f�[�^���Z�b�g
    void SetMesh();

    /// @brief ���b�V���f�[�^���Z�b�g
    void SetMeshShadow();

    /// @brief �`��g�|���W�[���Z�b�g
    /// @param topology �`��g�|���W�[
    void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology) { topology_ = topology; }

    /// @brief �萔�o�b�t�@�ǉ�
    /// @param bufferSize �o�b�t�@�T�C�Y
    /// @param signatureNo �V�O�l�`���ԍ�
    /// @param bufferNo �萔�o�b�t�@�Ǘ��ԍ�
    /// @return �o�b�t�@�X�V�p�|�C���^
    template<class T>
    T* AddConstantBuffer(unsigned long long bufferSize, int signatureNo, int& bufferNo)
    {
        ComPtr<ID3D12Resource> tempBuffer;
        T* bufferMap;

        auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(bufferSize) + 0xff) & ~0xff);

        MainDevice->CreateCommittedResource(
            &heapP,
            D3D12_HEAP_FLAG_NONE,
            &heapDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(tempBuffer.ReleaseAndGetAddressOf())
        );

        tempBuffer->Map(0, nullptr, (void**)&bufferMap);

        int heapNo_ = HeapAllocator.CreateCBV(tempBuffer.Get());

        constBuffers_.emplace(signatureNo, pair(tempBuffer, heapNo_));

        bufferNo = signatureNo;

        return bufferMap;
    }

    /// @brief �V�F�[�_�[���\�[�X�ǉ�
    /// @param bufferSize �o�b�t�@�T�C�Y
    /// @param signatureNo �V�O�l�`���ԍ�
    /// @param bufferNo �萔�o�b�t�@�Ǘ��ԍ�
    /// @return �o�b�t�@�X�V�p�|�C���^
    void AddShaderResource(ComPtr<ID3D12Resource> resource, int signatureNo, D3D12_SHADER_RESOURCE_VIEW_DESC desc)
    {
        int heapNo_ = HeapAllocator.CreateSRV(resource.Get(), desc);

        shaderResource_.emplace(signatureNo, pair(resource, heapNo_));
    }

    /// @brief �ǉ������萔�o�b�t�@���N���A
    void ClearConstantBuffer();

    /// @brief �ǉ������V�F�[�_�[���\�[�X���N���A
    void ClearShaderResource();



    /// @brief ���������b�V�����ǂ������Z�b�g
    /// @param isTrans ���������b�V�����ǂ���
    void SetTransState(bool isTrans) { isTrans_ = isTrans; }

    /// @brief ���_�f�[�^�X�V
    /// @param indices ���_�f�[�^
    virtual void SetVertices(const variant<vector<VERTEX_2D>, vector<VERTEX_3D>, vector<VERTEX_ANIMATION>>& vertices) = 0;

    /// @brief �C���f�b�N�X�f�[�^�X�V
    /// @param indices �C���f�b�N�X�f�[�^
    void SetIndices(vector<unsigned short>& indices);

    /// @brief ���������b�V�����ǂ���
    /// @return ���������b�V�����ǂ���
    [[nodiscard]] const bool& IsTrans()const { return isTrans_; }

    /// @brief �}�e���A���擾
    /// @return �}�e���A��
    [[nodiscard]] Material& GetMaterial()const { return *material_; }

    /// @brief ���_���擾
    /// @return ���_��
    [[nodiscard]] virtual UINT GetVertexNum()const = 0;

    /// @brief �C���f�b�N�X���擾
    /// @return �C���f�b�N�X��
    [[nodiscard]] UINT GetIndexNum()const { return static_cast<UINT>(indices_.size()); }

    /// @brief �C���f�b�N�X�f�[�^�擾
    /// @return �C���f�b�N�X�f�[�^
    [[nodiscard]] vector<unsigned short>& GetIndices(){ return indices_; }

    /// @brief �o�E���f�B���O�X�t�B�A�擾
    /// @return �o�E���f�B���O�X�t�B�A
    [[nodiscard]] const BoundingSphere& GetBoundingSphere()const { return boundingSphere_; }

protected:

    /// @brief ���_�o�b�t�@�쐬
    virtual void CreateVertexBuffer() = 0;

    /// @brief �C���f�b�N�X�o�b�t�@�쐬
    void CreateIndexBuffer();

    void CreateBoundingSphere(const vector<Vector3>& vertices);

    /// @brief ���_�f�[�^�Z�b�g
    virtual void SetVertex() = 0;

    bool isTrans_;                                  /// @brief ���������b�V�����ǂ���

    vector<unsigned short>indices_;                 /// @brief �C���f�b�N�X�f�[�^
    ComPtr<ID3D12Resource> indexBuffer_;            /// @brief �C���f�b�N�X�o�b�t�@
    D3D12_INDEX_BUFFER_VIEW indexBufferView_;       /// @brief �C���f�b�N�X�o�b�t�@�r���[
    unsigned short* indexMap_;                      /// @brief �C���f�b�N�X�f�[�^�X�V�p�ϐ�

    map<int, pair<ComPtr<ID3D12Resource>,int>> constBuffers_;			/// @brief �ǉ��萔�o�b�t�@(�L�[�F�V�O�l�`���ԍ��A�v�f�F�萔�o�b�t�@&�f�B�X�N���v�^�q�[�v�Ǘ��ԍ�)
    map<int, pair<ComPtr<ID3D12Resource>,int>> shaderResource_;			/// @brief �ǉ��V�F�[�_�[���\�[�X(�L�[�F�V�O�l�`���ԍ��A�v�f�F�萔�o�b�t�@&�f�B�X�N���v�^�q�[�v�Ǘ��ԍ�)

    shared_ptr<Material> material_;     /// @brief �}�e���A��

    D3D_PRIMITIVE_TOPOLOGY topology_;   /// @brief �`��g�|���W�[

    BoundingSphere boundingSphere_;     /// @brief �o�E���f�B���O�X�t�B�A
};

