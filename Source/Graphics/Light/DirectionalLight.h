#pragma once

/// @brief �f�B���N�V���i�����C�g
struct DIRECTIONAL_LIGHT
{
    Vector3 direction = { 1.0f,-1.0f,1.0f };    // ���C�g�̕���
    float intensity = 1.0f;                     // ���̋��x
    Vector3 color = { 1.0f,1.0f,1.0f };         // ���C�g�̐F
    float padding = 0.0f;                       // �������̐���p
};
