#pragma once

/// @brief �X�|�b�g���C�g
struct SPOT_LIGHT
{
    Position3D pos = { 0.0f,0.0f,0.0f };        /// @brief ���C�g�̈ʒu
    float range = 0.0f;                         /// @brief ���̉e���͈�
    Vector3 direction = { 0.0f,-1.0f,0.0f };    /// @brief ���C�g�̕���
    float angle = 0.0f;                         /// @brief �Ǝˊp�x
    Vector3 color = { 0.0f,0.0f,0.0f };         /// @brief ���C�g�̐F
    float padding = 0.0f;                       /// @brief �������̐���p
};
