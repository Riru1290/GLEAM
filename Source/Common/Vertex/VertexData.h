#pragma once

enum class VERTEX_TYPE
{
    DEFAULT,
    ANIMATION
};


/// @brief 頂点データ
struct VERTEX_3D
{
    XMFLOAT3 pos = {};
    XMFLOAT3 normal = {};
    XMFLOAT2 uv = {};
    XMFLOAT3 tangent = {};
    XMFLOAT3 biNormal = {};
};

/// @brief 頂点データ(アニメーション)
struct VERTEX_ANIMATION
{
    XMFLOAT3 pos = {};
    XMFLOAT3 normal = {};
    XMFLOAT2 uv = {};
    XMFLOAT3 tangent = {};
    XMFLOAT3 biNormal = {};
    float boneWeights[4] = { 0.0f,0.0f,0.0f,0.0f };
    int boneIndices[4] = { 0,0,0,0 };
};

/// @brief 頂点データ
struct VERTEX_2D
{
    XMFLOAT2 pos = {};
    XMFLOAT2 uv = {};
};
