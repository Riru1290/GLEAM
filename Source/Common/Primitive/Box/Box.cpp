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

    // ���_�̏����ݒ�i8�̒��_�j
    VERTEX_3D vertices[] = {
        // �O��
        { XMFLOAT3(-halfSize, -halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) }, // �����O
        { XMFLOAT3(-halfSize,  halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) }, // ����O
        { XMFLOAT3(halfSize,  halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) }, // �E��O
        { XMFLOAT3(halfSize, -halfSize, -halfSize), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) }, // �E���O

        // �w��
        { XMFLOAT3(-halfSize, -halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) }, // ������
        { XMFLOAT3(halfSize, -halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) }, // �E����
        { XMFLOAT3(halfSize,  halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }, // �E���
        { XMFLOAT3(-halfSize,  halfSize,  halfSize), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) }  // �����
    };

    // ���_�f�[�^��ݒ�
    for (const auto& vertex : vertices) {
        data.verticesDefault3D.push_back(vertex);
    }

    // �C���f�b�N�X�̐ݒ�i12���̎O�p�`�Ń{�b�N�X���\���j
    uint32_t indices[] = {
        // �O��
        0, 1, 2,  0, 2, 3,
        // �w��
        4, 5, 6,  4, 6, 7,
        // ������
        0, 4, 7,  0, 7, 1,
        // �E����
        3, 2, 6,  3, 6, 5,
        // ���
        1, 7, 6,  1, 6, 2,
        // ���
        0, 3, 5,  0, 5, 4
    };

    // �C���f�b�N�X�f�[�^��ݒ�
    for (const auto& index : indices) {
        data.indices.push_back(index);
    }

    return data;
}
