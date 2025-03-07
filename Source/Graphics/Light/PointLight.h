#pragma once

/// @brief ポイントライト
struct POINT_LIGHT
{
    Position3D pos = { 0.0f,0.0f,0.0f };        // ライトの位置
    float intensity = 0.0f;                     // 光の強度
    Vector3 color = { 0.0f,0.0f,0.0f };         // ライトの色
    float radius = 0.0f;                        // 影響半径
};
