#pragma once

/// @brief �t���X�^�����\�����镽��
struct FrustumPlane {
    XMFLOAT3 normal; /// @brief ���ʂ̖@��
    float d;         /// @brief ���_����̋���
};

/// @brief �t���X�^��
struct Frustum {
    FrustumPlane planes[6]; // near, far, left, right, top, bottom
};
