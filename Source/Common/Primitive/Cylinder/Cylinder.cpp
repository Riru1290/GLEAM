#include "../../Mesh/Meshes/MeshDefault3D.h"
#include "Cylinder.h"

namespace
{
    const int CYLINDER_DIVISION_NUM = (12);
}

Cylinder::Cylinder()
{
    MESH_DATA data = CreateCylinderMeshData();

    mesh_ = make_unique<MeshDefault3D>(data, PSO_ID::PRIMITIVE_3D_INSTANCING);
}

MESH_DATA Cylinder::CreateCylinderMeshData()
{
    MESH_DATA data;

    float thetaStep = XM_2PI / static_cast<float>(CYLINDER_DIVISION_NUM);

    // 上円の中心点
    VERTEX_3D topCenterVertex = { XMFLOAT3(0.0f, DEFAULT_HEIGHT / 2.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f) };
    data.verticesDefault3D.emplace_back(topCenterVertex);

    // 上円の頂点
    for (int i = 0; i <= CYLINDER_DIVISION_NUM; ++i) {
        float theta = i * thetaStep;
        XMFLOAT3 position(DEFAULT_RADIUS * cosf(theta), DEFAULT_HEIGHT / 2.0f, DEFAULT_RADIUS * sinf(theta));
        XMFLOAT3 normal(0.0f, 1.0f, 0.0f); // 法線は上方向
        XMFLOAT2 uv(0.5f + 0.5f * cosf(theta), 0.5f - 0.5f * sinf(theta));
        data.verticesDefault3D.emplace_back(VERTEX_3D(position, normal, uv));
    }

    // 下円の中心点
    VERTEX_3D bottomCenterVertex = { XMFLOAT3(0.0f, -DEFAULT_HEIGHT / 2.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.5f, 1.0f) };
    data.verticesDefault3D.emplace_back(bottomCenterVertex);

    int bottomCenterIndex = static_cast<int>(data.verticesDefault3D.size()) - 1;

    // 下円の頂点
    for (int i = 0; i <= CYLINDER_DIVISION_NUM; ++i) {
        float theta = i * thetaStep;
        XMFLOAT3 position(DEFAULT_RADIUS * cosf(theta), -DEFAULT_HEIGHT / 2.0f, DEFAULT_RADIUS * sinf(theta));
        XMFLOAT3 normal(0.0f, -1.0f, 0.0f); // 法線は下方向
        XMFLOAT2 uv(0.5f + 0.5f * cosf(theta), 0.5f + 0.5f * sinf(theta));
        data.verticesDefault3D.emplace_back(VERTEX_3D(position, normal, uv));
    }

    // 中間部分の頂点
    for (int i = 0; i <= CYLINDER_DIVISION_NUM; ++i) {
        float theta = i * thetaStep;
        float x = DEFAULT_RADIUS * cosf(theta);
        float z = DEFAULT_RADIUS * sinf(theta);

        // 上側の中間部
        data.verticesDefault3D.emplace_back(VERTEX_3D(
            XMFLOAT3(x, DEFAULT_HEIGHT / 2.0f, z),
            XMFLOAT3(x, 0.0f, z),
            XMFLOAT2(i / static_cast<float>(CYLINDER_DIVISION_NUM), 0.0f)));

        // 下側の中間部
        data.verticesDefault3D.emplace_back(VERTEX_3D(
            XMFLOAT3(x, -DEFAULT_HEIGHT / 2.0f, z),
            XMFLOAT3(x, 0.0f, z),
            XMFLOAT2(i / static_cast<float>(CYLINDER_DIVISION_NUM), 1.0f)));
    }

    // インデックスの設定
    // 上円
    for (int i = 1; i <= CYLINDER_DIVISION_NUM; ++i) {
        data.indices.emplace_back(0);
        data.indices.emplace_back(i);
        data.indices.emplace_back(i + 1);
    }

    // 下円
    for (int i = 1; i <= CYLINDER_DIVISION_NUM; ++i) {
        data.indices.emplace_back(bottomCenterIndex);
        data.indices.emplace_back(bottomCenterIndex + i + 1);
        data.indices.emplace_back(bottomCenterIndex + i);
    }

    // 中間部分
    int ringVertexStart = static_cast<int>(data.verticesDefault3D.size()) - 2 * (CYLINDER_DIVISION_NUM + 1);
    for (int i = 0; i < CYLINDER_DIVISION_NUM; ++i) {
        int upperLeft = ringVertexStart + 2 * i;
        int upperRight = ringVertexStart + 2 * i + 2;
        int lowerLeft = ringVertexStart + 2 * i + 1;
        int lowerRight = ringVertexStart + 2 * i + 3;

        data.indices.emplace_back(upperLeft);
        data.indices.emplace_back(lowerLeft);
        data.indices.emplace_back(upperRight);

        data.indices.emplace_back(upperRight);
        data.indices.emplace_back(lowerLeft);
        data.indices.emplace_back(lowerRight);
    }

    return data;
}
