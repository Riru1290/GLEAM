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

    // ��~�̒��S�_
    VERTEX_3D topCenterVertex = { XMFLOAT3(0.0f, DEFAULT_HEIGHT / 2.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f) };
    data.verticesDefault3D.emplace_back(topCenterVertex);

    // ��~�̒��_
    for (int i = 0; i <= CYLINDER_DIVISION_NUM; ++i) {
        float theta = i * thetaStep;
        XMFLOAT3 position(DEFAULT_RADIUS * cosf(theta), DEFAULT_HEIGHT / 2.0f, DEFAULT_RADIUS * sinf(theta));
        XMFLOAT3 normal(0.0f, 1.0f, 0.0f); // �@���͏����
        XMFLOAT2 uv(0.5f + 0.5f * cosf(theta), 0.5f - 0.5f * sinf(theta));
        data.verticesDefault3D.emplace_back(VERTEX_3D(position, normal, uv));
    }

    // ���~�̒��S�_
    VERTEX_3D bottomCenterVertex = { XMFLOAT3(0.0f, -DEFAULT_HEIGHT / 2.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.5f, 1.0f) };
    data.verticesDefault3D.emplace_back(bottomCenterVertex);

    int bottomCenterIndex = static_cast<int>(data.verticesDefault3D.size()) - 1;

    // ���~�̒��_
    for (int i = 0; i <= CYLINDER_DIVISION_NUM; ++i) {
        float theta = i * thetaStep;
        XMFLOAT3 position(DEFAULT_RADIUS * cosf(theta), -DEFAULT_HEIGHT / 2.0f, DEFAULT_RADIUS * sinf(theta));
        XMFLOAT3 normal(0.0f, -1.0f, 0.0f); // �@���͉�����
        XMFLOAT2 uv(0.5f + 0.5f * cosf(theta), 0.5f + 0.5f * sinf(theta));
        data.verticesDefault3D.emplace_back(VERTEX_3D(position, normal, uv));
    }

    // ���ԕ����̒��_
    for (int i = 0; i <= CYLINDER_DIVISION_NUM; ++i) {
        float theta = i * thetaStep;
        float x = DEFAULT_RADIUS * cosf(theta);
        float z = DEFAULT_RADIUS * sinf(theta);

        // �㑤�̒��ԕ�
        data.verticesDefault3D.emplace_back(VERTEX_3D(
            XMFLOAT3(x, DEFAULT_HEIGHT / 2.0f, z),
            XMFLOAT3(x, 0.0f, z),
            XMFLOAT2(i / static_cast<float>(CYLINDER_DIVISION_NUM), 0.0f)));

        // �����̒��ԕ�
        data.verticesDefault3D.emplace_back(VERTEX_3D(
            XMFLOAT3(x, -DEFAULT_HEIGHT / 2.0f, z),
            XMFLOAT3(x, 0.0f, z),
            XMFLOAT2(i / static_cast<float>(CYLINDER_DIVISION_NUM), 1.0f)));
    }

    // �C���f�b�N�X�̐ݒ�
    // ��~
    for (int i = 1; i <= CYLINDER_DIVISION_NUM; ++i) {
        data.indices.emplace_back(0);
        data.indices.emplace_back(i);
        data.indices.emplace_back(i + 1);
    }

    // ���~
    for (int i = 1; i <= CYLINDER_DIVISION_NUM; ++i) {
        data.indices.emplace_back(bottomCenterIndex);
        data.indices.emplace_back(bottomCenterIndex + i + 1);
        data.indices.emplace_back(bottomCenterIndex + i);
    }

    // ���ԕ���
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
