#pragma once
#include "../Mesh.h"

class MeshDefault3D final : public Mesh
{
public:

    [[nodiscard]] MeshDefault3D(MESH_DATA data, PSO_ID psoID = PSO_ID::DEFAULT_3D);
    virtual ~MeshDefault3D();

    /// @brief 頂点データ取得
    /// @return 頂点データ
    [[nodiscard]] vector<VERTEX_3D>& GetVertices()
    {
        return vertices_;
    }

private:

    /// @brief 頂点バッファ作成
    void CreateVertexBuffer() override;

    /// @brief 頂点バッファ再作成
    void RecreateVertexBuffer(int vertexNum) override;

    /// @brief 頂点データセット
    void SetVertex() override;

    /// @brief 頂点データ更新
    /// @param indices 頂点データ
    void SetVertices(const variant<vector<VERTEX_2D>, vector<VERTEX_3D>, vector<VERTEX_ANIMATION>>& vertices)override;


    /// @brief 頂点数取得
    /// @return 頂点数
    [[nodiscard]] UINT GetVertexNum()const override { return static_cast<UINT>(vertices_.size()); }



    vector<VERTEX_3D> vertices_;                       /// @brief 頂点データ
    ComPtr<ID3D12Resource> vertexBuffer_;           /// @brief 頂点バッファ
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;     /// @brief 頂点バッファビュー
    VERTEX_3D* vertexMap_;                             /// @brief 頂点データ更新用変数

};

