#include "../../Mesh/Meshes/MeshDefault3D.h"
#include "Capsule.h"

namespace
{
    const int CAPSULE_DIVISION_NUM = (12);
}

Capsule::Capsule()
{
    MESH_DATA data = CreateCapsuleTopMeshData();
    meshes_.emplace_back(move(make_unique<MeshDefault3D>(data, PSO_ID::PRIMITIVE_3D_INSTANCING)));

    data = CreateCapsuleMiddleMeshData();
    meshes_.emplace_back(move(make_unique<MeshDefault3D>(data, PSO_ID::PRIMITIVE_3D_INSTANCING)));

    data = CreateCapsuleDownMeshData();
    meshes_.emplace_back(move(make_unique<MeshDefault3D>(data, PSO_ID::PRIMITIVE_3D_INSTANCING)));
}

MESH_DATA Capsule::CreateCapsuleTopMeshData()
{
    MESH_DATA data = {};
    float phiStep = XM_PI / static_cast<float>(CAPSULE_DIVISION_NUM);
    float thetaStep = 2.0f * XM_PI / static_cast<float>(CAPSULE_DIVISION_NUM);

    // 上部半球の頂点を生成
    for (int i = 0; i <= CAPSULE_DIVISION_NUM / 2; ++i) {
        float phi = i * phiStep;
        for (int j = 0; j <= CAPSULE_DIVISION_NUM; ++j) {
            float theta = j * thetaStep;

            XMFLOAT3 position(
                DEFAULT_RADIUS * sinf(phi) * cosf(theta),
                DEFAULT_RADIUS * cosf(phi) + DEFAULT_HEIGHT / 2,
                DEFAULT_RADIUS * sinf(phi) * sinf(theta)
            );

            XMFLOAT3 normal = position;
            XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&normal)));

            XMFLOAT2 uv(
                theta / XM_2PI,
                phi / XM_PI
            );

            data.verticesDefault3D.emplace_back(VERTEX_3D(position, normal, uv));
        }
    }

    // インデックスの設定（上部半球のトライアングル）
    int ringVertexCount = CAPSULE_DIVISION_NUM + 1;
    for (int i = 0; i < CAPSULE_DIVISION_NUM / 2; ++i) {
        for (int j = 0; j < CAPSULE_DIVISION_NUM; ++j) {
            data.indices.emplace_back(i * ringVertexCount + j);
            data.indices.emplace_back((i + 1) * ringVertexCount + j);
            data.indices.emplace_back(i * ringVertexCount + j + 1);

            data.indices.emplace_back(i * ringVertexCount + j + 1);
            data.indices.emplace_back((i + 1) * ringVertexCount + j);
            data.indices.emplace_back((i + 1) * ringVertexCount + j + 1);
        }
    }

    return data;

    return data;
}

MESH_DATA Capsule::CreateCapsuleMiddleMeshData()
{
    MESH_DATA data = {};
    float thetaStep = 2.0f * XM_PI / static_cast<float>(CAPSULE_DIVISION_NUM);

    for (int i = 0; i <= 1; ++i) {
        float y = (i == 0) ? DEFAULT_HEIGHT / 2 : -DEFAULT_HEIGHT / 2;
        for (int j = 0; j <= CAPSULE_DIVISION_NUM; ++j) {
            float theta = j * thetaStep;

            XMFLOAT3 position(
                DEFAULT_RADIUS * cosf(theta),
                y,
                DEFAULT_RADIUS * sinf(theta)
            );

            XMFLOAT3 normal = XMFLOAT3(position.x, 0.0f, position.z);
            XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&normal)));

            XMFLOAT2 uv(
                theta / XM_2PI,
                0.5f + i * 0.5f
            );

            data.verticesDefault3D.emplace_back(VERTEX_3D(position, normal, uv));
        }
    }

    int ringVertexCount = CAPSULE_DIVISION_NUM + 1;
    for (int i = 0; i < CAPSULE_DIVISION_NUM; ++i) {
        data.indices.emplace_back(i);
        data.indices.emplace_back(i + 1);
        data.indices.emplace_back(i + ringVertexCount);

        data.indices.emplace_back(i + ringVertexCount);
        data.indices.emplace_back(i + 1);
        data.indices.emplace_back(i + ringVertexCount + 1);
    }

    return data;
}

MESH_DATA Capsule::CreateCapsuleDownMeshData()
{
    MESH_DATA data = {};
    float phiStep = XM_PI / static_cast<float>(CAPSULE_DIVISION_NUM);
    float thetaStep = 2.0f * XM_PI / static_cast<float>(CAPSULE_DIVISION_NUM);

    // 下部半球の頂点を生成
    for (int i = 0; i <= CAPSULE_DIVISION_NUM / 2; ++i) {
        float phi = i * phiStep;
        for (int j = 0; j <= CAPSULE_DIVISION_NUM; ++j) {
            float theta = j * thetaStep;

            XMFLOAT3 position(
                DEFAULT_RADIUS * sinf(phi) * cosf(theta),
                -DEFAULT_RADIUS * cosf(phi) - DEFAULT_HEIGHT / 2,
                DEFAULT_RADIUS * sinf(phi) * sinf(theta)
            );

            XMFLOAT3 normal = position;
            XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&normal)));

            XMFLOAT2 uv(
                theta / XM_2PI,
                phi / XM_PI
            );

            data.verticesDefault3D.emplace_back(VERTEX_3D(position, normal, uv));
        }
    }

    // インデックスの設定（下部半球のトライアングル）
    int ringVertexCount = CAPSULE_DIVISION_NUM + 1;
    for (int i = 0; i < CAPSULE_DIVISION_NUM / 2; ++i) {
        for (int j = 0; j < CAPSULE_DIVISION_NUM; ++j) {
            data.indices.emplace_back(i * ringVertexCount + j);
            data.indices.emplace_back((i + 1) * ringVertexCount + j);
            data.indices.emplace_back(i * ringVertexCount + j + 1);

            data.indices.emplace_back(i * ringVertexCount + j + 1);
            data.indices.emplace_back((i + 1) * ringVertexCount + j);
            data.indices.emplace_back((i + 1) * ringVertexCount + j + 1);
        }
    }

    return data;

}
