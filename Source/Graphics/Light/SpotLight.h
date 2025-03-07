#pragma once

/// @brief スポットライト
struct SPOT_LIGHT
{
    Position3D pos = { 0.0f,0.0f,0.0f };        /// @brief ライトの位置
    float range = 0.0f;                         /// @brief 光の影響範囲
    Vector3 direction = { 0.0f,-1.0f,0.0f };    /// @brief ライトの方向
    float angle = 0.0f;                         /// @brief 照射角度
    Vector3 color = { 0.0f,0.0f,0.0f };         /// @brief ライトの色
    float padding = 0.0f;                       /// @brief メモリの整列用
};
