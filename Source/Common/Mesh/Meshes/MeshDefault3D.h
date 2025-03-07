#pragma once
#include "../Mesh.h"

class MeshDefault3D final : public Mesh
{
public:

    [[nodiscard]] MeshDefault3D(MESH_DATA data, PSO_ID psoID = PSO_ID::DEFAULT_3D);
    virtual ~MeshDefault3D();

    /// @brief ���_�f�[�^�擾
    /// @return ���_�f�[�^
    [[nodiscard]] vector<VERTEX_3D>& GetVertices()
    {
        return vertices_;
    }

private:

    /// @brief ���_�o�b�t�@�쐬
    void CreateVertexBuffer() override;

    /// @brief ���_�o�b�t�@�č쐬
    void RecreateVertexBuffer(int vertexNum) override;

    /// @brief ���_�f�[�^�Z�b�g
    void SetVertex() override;

    /// @brief ���_�f�[�^�X�V
    /// @param indices ���_�f�[�^
    void SetVertices(const variant<vector<VERTEX_2D>, vector<VERTEX_3D>, vector<VERTEX_ANIMATION>>& vertices)override;


    /// @brief ���_���擾
    /// @return ���_��
    [[nodiscard]] UINT GetVertexNum()const override { return static_cast<UINT>(vertices_.size()); }



    vector<VERTEX_3D> vertices_;                       /// @brief ���_�f�[�^
    ComPtr<ID3D12Resource> vertexBuffer_;           /// @brief ���_�o�b�t�@
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;     /// @brief ���_�o�b�t�@�r���[
    VERTEX_3D* vertexMap_;                             /// @brief ���_�f�[�^�X�V�p�ϐ�

};

