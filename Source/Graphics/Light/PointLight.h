#pragma once

/// @brief �|�C���g���C�g
struct POINT_LIGHT
{
    Position3D pos = { 0.0f,0.0f,0.0f };        // ���C�g�̈ʒu
    float intensity = 0.0f;                     // ���̋��x
    Vector3 color = { 0.0f,0.0f,0.0f };         // ���C�g�̐F
    float radius = 0.0f;                        // �e�����a
};
