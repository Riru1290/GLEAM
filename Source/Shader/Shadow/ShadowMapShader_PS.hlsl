#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_2D

// ‹¤’Ê
#include "../Header/Common.hlsli"

#include "../Header/PixelShader.hlsli"

// IN
#include "../Header/VertexToPixel.hlsli"
#define PS_INPUT VertexToPixel2D


float4 main(PS_INPUT input) : SV_TARGET
{
    float test = pow(abs(shadowMaps.Sample(shadowMapSampler, float3(input.uv, 0)).r), 20);
    return float4(test, 0, 0, 1.0);
}
