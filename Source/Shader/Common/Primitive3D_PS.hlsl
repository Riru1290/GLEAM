#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_MODEL

// ‹¤’Ê
#include "../Header/Common.hlsli"

#include "../Header/PixelShader.hlsli"

// IN
#include "../Header/VertexToPixel.hlsli"
#define PS_INPUT VertexToPixelPrimitive

float4 main(PS_INPUT input) : SV_TARGET
{
    
    float4 color = input.color;
    
    if (color.a < 0.01f)
    {
        discard;
    }
    
    return color;
}