#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_MODEL

// ‹¤’Ê
#include "../Header/Common.hlsli"


#include "../Header/PixelShader.hlsli"

// IN
#include "../Header/VertexToPixel.hlsli"
#define PS_INPUT VertexToPixel


float4 main(PS_INPUT input) : SV_TARGET
{
    
    // LOD‚ÌŒvŽZ
    float lod = diffuseMapTexture.CalculateLevelOfDetail(diffuseMapSampler, input.uv);
    
    float4 color = diffuseMapTexture.SampleLevel(diffuseMapSampler, input.uv, lod);

    if (color.a < 0.01f || alpha < 0.01f)
    {
        discard;
    }
    color.a *= alpha;
    
    return color;

}