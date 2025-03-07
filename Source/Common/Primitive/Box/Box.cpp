#include "../../Mesh/Meshes/MeshDefault3D.h"
#include "Box.h"


Box::Box()
{
	MESH_DATA data = CreateBoxMeshData();

	mesh_ = make_unique<MeshDefault3D>(data, PSO_ID::PRIMITIVE_3D_INSTANCING);
}

MESH_DATA Box::CreateBoxMeshData()
{
    MESH_DATA data = {};

    float halfSize = DEFAULT_SIDE_LENGTH / 2.0f;

    // 頂点の初期設定（8つの頂点）
    VERTEX_3D vertices[] = {
        // 前面
        { XMFLOAT3(-halfSize, -halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) }, // 左下前
        { XMFLOAT3(-halfSize,  halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) }, // 左上前
        { XMFLOAT3(halfSize,  halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) }, // 右上前
        { XMFLOAT3(halfSize, -halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) }, // 右下前

        // 背面
        { XMFLOAT3(-halfSize, -halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) }, // 左下後
        { XMFLOAT3(halfSize, -halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) }, // 右下後
        { XMFLOAT3(halfSize,  halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }, // 右上後
        { XMFLOAT3(-halfSize,  halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) }  // 左上後
    };

    // 頂点データを設定
    for (const auto& vertex : vertices) {
        data.verticesDefault3D.push_back(vertex);
    }

    // インデックスの設定（12枚の三角形でボックスを構成）
    uint32_t indices[] = {
        // 前面
        0, 1, 2,  0, 2, 3,
        // 背面
        4, 5, 6,  4, 6, 7,
        // 左側面
        0, 4, 7,  0, 7, 1,
        // 右側面
        3, 2, 6,  3, 6, 5,
        // 上面
        1, 7, 6,  1, 6, 2,
        // 底面
        0, 3, 5,  0, 5, 4
    };

    // インデックスデータを設定
    for (const auto& index : indices) {
        data.indices.push_back(index);
    }

    return data;
}
