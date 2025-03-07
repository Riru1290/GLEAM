#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_MODEL

// ‹¤’Ê
#include "../Header/Common.hlsli"

// IN
#include "../Header/VertexShader.hlsli"

// OUT
#include "../Header/VertexToPixel.hlsli"
#define VS_OUTPUT VertexToShadow

cbuffer LightCamera : register(b4)
{
    uint lightCameraIndex;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    input.position = mul(local, input.position);
    input.position = mul(world, input.position);
    output.position = mul(lightCameraMat[lightCameraIndex], input.position);
    
    return output;
}
