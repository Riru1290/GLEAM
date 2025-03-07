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
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, input.uv);

    if (color.a < 0.01f || alpha < 0.01f)
    {
        discard;
    }
    
    return float4(saturate(color.rgb), color.a * alpha);
}