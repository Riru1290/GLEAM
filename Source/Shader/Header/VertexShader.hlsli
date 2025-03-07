#include "RenderType.hlsli"

#ifdef RENDER_TYPE

#if (RENDER_TYPE == DEFAULT_MODEL)

struct VS_INPUT
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 tangent : TANGENT;
    float4 biNormal : BINORMAL;
};

#elif (RENDER_TYPE == ANIMATION_MODEL)

struct VS_INPUT
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 tangent : TANGENT;
    float4 biNormal : BINORMAL;
    float4 weight : WEIGHT;
    int4 index : INDEX;
};

cbuffer Bones : register(b4)
{
    matrix boneMatrices[128]; // アニメーション用のボーン行列
};

#elif (RENDER_TYPE == DEFAULT_2D || RENDER_TYPE == POST_EFFECT)

struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

#elif (RENDER_TYPE == PRIMITIVE_2D)

#elif (RENDER_TYPE == PRIMITIVE_3D)

#elif (RENDER_TYPE == INSTANCING_DEFAULT_MODEL)

#elif (RENDER_TYPE == INSTANCING_ANIMATION_MODEL)

#elif (RENDER_TYPE == INSTANCING_PRIMITIVE_2D)

#elif (RENDER_TYPE == INSTANCING_PRIMITIVE_3D)

struct VS_INPUT
{
    float4 position : POSITION;
    float4 normal : NORMAL;
};

struct PRIMITIVE_DATA
{
    matrix world; // ワールド行列（位置、回転、スケールなど）
    float4 color; // カラー
};

StructuredBuffer<PRIMITIVE_DATA> instanceBuffer : register(t4);

#endif

#endif