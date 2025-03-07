#pragma once

/// @brief ディレクショナルライト
struct DIRECTIONAL_LIGHT
{
    Vector3 direction = { 1.0f,-1.0f,1.0f };    // ライトの方向
    float intensity = 1.0f;                     // 光の強度
    Vector3 color = { 1.0f,1.0f,1.0f };         // ライトの色
    float padding = 0.0f;                       // メモリの整列用
};
