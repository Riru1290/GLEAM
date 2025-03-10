#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_2D

// ����
#include "../Header/Common.hlsli"

#include "../Header/PixelShader.hlsli"

// IN
#include "../Header/VertexToPixel.hlsli"
#define PS_INPUT VertexToPixel2D

cbuffer Fade : register(b2)
{
    float fadePow;
    float3 paddingFade;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, input.uv);
    
    if (color.a < 0.01f)
    {
        discard;
    }
    
    if (fadePow < 0.01f)
    {
        discard;
    }
    
    return float4(saturate(color.rgb), fadePow);
}
