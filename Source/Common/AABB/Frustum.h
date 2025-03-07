#pragma once

/// @brief フラスタムを構成する平面
struct FrustumPlane {
    XMFLOAT3 normal; /// @brief 平面の法線
    float d;         /// @brief 原点からの距離
};

/// @brief フラスタム
struct Frustum {
    FrustumPlane planes[6]; // near, far, left, right, top, bottom
};
