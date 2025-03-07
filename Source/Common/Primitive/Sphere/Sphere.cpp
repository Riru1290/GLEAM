#include "../../Mesh/Meshes/MeshDefault3D.h"
#include "Sphere.h"

namespace
{
    const int CAPSULE_DIVISION_NUM = (12);
}

Sphere::Sphere()
{
	MESH_DATA data = CreateSphereMeshData();

	mesh_ = make_unique<MeshDefault3D>(data, PSO_ID::PRIMITIVE_3D_INSTANCING);
}

MESH_DATA Sphere::CreateSphereMeshData()
{
	MESH_DATA data = {};

    // 頂点の初期設定
    VERTEX_3D topVertex = { XMFLOAT3(0.0f, DEFAULT_RADIUS, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
    VERTEX_3D bottomVertex = { XMFLOAT3(0.0f, -DEFAULT_RADIUS, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };

    data.verticesDefault3D.emplace_back(topVertex);


    float phiStep = XM_PI / static_cast<float>(CAPSULE_DIVISION_NUM);
    float thetaStep = 2.0f * XM_PI / static_cast<float>(CAPSULE_DIVISION_NUM);

    for (int i = 1; i <= CAPSULE_DIVISION_NUM - 1; ++i) {
        float phi = i * phiStep;

        for (int j = 0; j <= CAPSULE_DIVISION_NUM; ++j) {
            float theta = j * thetaStep;

            XMFLOAT3 position(
                DEFAULT_RADIUS * sinf(phi) * cosf(theta),
                DEFAULT_RADIUS * cosf(phi),
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

    data.verticesDefault3D.emplace_back(bottomVertex);

    // インデックスの設定
    for (int i = 1; i <= CAPSULE_DIVISION_NUM; ++i) {
        data.indices.emplace_back(0);
        data.indices.emplace_back(i + 1);
        data.indices.emplace_back(i);
    }

    int baseIndex = 1;
    int ringVertexCount = CAPSULE_DIVISION_NUM + 1;
    for (int i = 0; i < CAPSULE_DIVISION_NUM - 2; ++i) {
        for (int j = 0; j < CAPSULE_DIVISION_NUM; ++j) {
            data.indices.emplace_back(baseIndex + i * ringVertexCount + j);
            data.indices.emplace_back(baseIndex + i * ringVertexCount + j + 1);
            data.indices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j);

            data.indices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j);
            data.indices.emplace_back(baseIndex + i * ringVertexCount + j + 1);
            data.indices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    for (int i = 0; i < CAPSULE_DIVISION_NUM; ++i) {
        data.indices.emplace_back(static_cast<uint32_t>(data.verticesDefault3D.size() - 1));
        data.indices.emplace_back(baseIndex + (CAPSULE_DIVISION_NUM - 2) * ringVertexCount + i);
        data.indices.emplace_back(baseIndex + (CAPSULE_DIVISION_NUM - 2) * ringVertexCount + i + 1);
    }

	return data;
}
