#pragma once
#include "MeshData.h"
#include "../AABB/BoundingSphere.h"
#include "../../Graphics/PipelineStateObject/PipelineStateObjectID.h"
#include "../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"


class Material;
class Transform;

/// @brief メッシュ
class Mesh
{
public:

	[[nodiscard]] Mesh(MESH_DATA data,PSO_ID psoID = PSO_ID::DEFAULT_3D);
	virtual ~Mesh();

    /// @brief 頂点バッファ再作成
    virtual void RecreateVertexBuffer(int vertexNum) = 0;

    /// @brief インデックスバッファ再作成
    void RecreateIndexBuffer(int indexNum);

    /// @brief メッシュデータをセット
    void SetMesh();

    /// @brief メッシュデータをセット
    void SetMeshShadow();

    /// @brief 描画トポロジーをセット
    /// @param topology 描画トポロジー
    void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology) { topology_ = topology; }

    /// @brief 定数バッファ追加
    /// @param bufferSize バッファサイズ
    /// @param signatureNo シグネチャ番号
    /// @param bufferNo 定数バッファ管理番号
    /// @return バッファ更新用ポインタ
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

    /// @brief シェーダーリソース追加
    /// @param bufferSize バッファサイズ
    /// @param signatureNo シグネチャ番号
    /// @param bufferNo 定数バッファ管理番号
    /// @return バッファ更新用ポインタ
    void AddShaderResource(ComPtr<ID3D12Resource> resource, int signatureNo, D3D12_SHADER_RESOURCE_VIEW_DESC desc)
    {
        int heapNo_ = HeapAllocator.CreateSRV(resource.Get(), desc);

        shaderResource_.emplace(signatureNo, pair(resource, heapNo_));
    }

    /// @brief 追加した定数バッファをクリア
    void ClearConstantBuffer();

    /// @brief 追加したシェーダーリソースをクリア
    void ClearShaderResource();



    /// @brief 半透明メッシュかどうかをセット
    /// @param isTrans 半透明メッシュかどうか
    void SetTransState(bool isTrans) { isTrans_ = isTrans; }

    /// @brief 頂点データ更新
    /// @param indices 頂点データ
    virtual void SetVertices(const variant<vector<VERTEX_2D>, vector<VERTEX_3D>, vector<VERTEX_ANIMATION>>& vertices) = 0;

    /// @brief インデックスデータ更新
    /// @param indices インデックスデータ
    void SetIndices(vector<unsigned short>& indices);

    /// @brief 半透明メッシュかどうか
    /// @return 半透明メッシュかどうか
    [[nodiscard]] const bool& IsTrans()const { return isTrans_; }

    /// @brief マテリアル取得
    /// @return マテリアル
    [[nodiscard]] Material& GetMaterial()const { return *material_; }

    /// @brief 頂点数取得
    /// @return 頂点数
    [[nodiscard]] virtual UINT GetVertexNum()const = 0;

    /// @brief インデックス数取得
    /// @return インデックス数
    [[nodiscard]] UINT GetIndexNum()const { return static_cast<UINT>(indices_.size()); }

    /// @brief インデックスデータ取得
    /// @return インデックスデータ
    [[nodiscard]] vector<unsigned short>& GetIndices(){ return indices_; }

    /// @brief バウンディングスフィア取得
    /// @return バウンディングスフィア
    [[nodiscard]] const BoundingSphere& GetBoundingSphere()const { return boundingSphere_; }

protected:

    /// @brief 頂点バッファ作成
    virtual void CreateVertexBuffer() = 0;

    /// @brief インデックスバッファ作成
    void CreateIndexBuffer();

    void CreateBoundingSphere(const vector<Vector3>& vertices);

    /// @brief 頂点データセット
    virtual void SetVertex() = 0;

    bool isTrans_;                                  /// @brief 半透明メッシュかどうか

    vector<unsigned short>indices_;                 /// @brief インデックスデータ
    ComPtr<ID3D12Resource> indexBuffer_;            /// @brief インデックスバッファ
    D3D12_INDEX_BUFFER_VIEW indexBufferView_;       /// @brief インデックスバッファビュー
    unsigned short* indexMap_;                      /// @brief インデックスデータ更新用変数

    map<int, pair<ComPtr<ID3D12Resource>,int>> constBuffers_;			/// @brief 追加定数バッファ(キー：シグネチャ番号、要素：定数バッファ&ディスクリプタヒープ管理番号)
    map<int, pair<ComPtr<ID3D12Resource>,int>> shaderResource_;			/// @brief 追加シェーダーリソース(キー：シグネチャ番号、要素：定数バッファ&ディスクリプタヒープ管理番号)

    shared_ptr<Material> material_;     /// @brief マテリアル

    D3D_PRIMITIVE_TOPOLOGY topology_;   /// @brief 描画トポロジー

    BoundingSphere boundingSphere_;     /// @brief バウンディングスフィア
};

