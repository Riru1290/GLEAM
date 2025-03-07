#include "../Header/RenderType.hlsli"

#define RENDER_TYPE POST_EFFECT

// ‹¤’Ê
#include "../Header/Common.hlsli"

#include "../Header/PixelShader.hlsli"

// IN
#include "../Header/VertexToPixel.hlsli"
#define PS_INPUT VertexToPixel2D

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = postEffectTexture.Sample(postEffectMapSampler, input.uv);
    
    if (color.a < 0.01f)
    {
        discard;
    }
    
    return color;
}