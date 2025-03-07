#include "../Header/RenderType.hlsli"

#define RENDER_TYPE ANIMATION_MODEL

// ‹¤’Ê
#include "../Header/Common.hlsli"



// IN
#include "../Header/VertexShader.hlsli"

// OUT
#include "../Header/VertexToPixel.hlsli"
#define VS_OUTPUT VertexToShadow

cbuffer LightCamera : register(b5)
{
    uint lightCameraIndex;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    matrix clmat = 0.0f;

    clmat = boneMatrices[input.index.x] * input.weight.x;
    clmat += boneMatrices[input.index.y] * input.weight.y;
    clmat += boneMatrices[input.index.z] * input.weight.z;
    clmat += boneMatrices[input.index.w] * input.weight.w;
    
    input.position = mul(clmat, input.position);
    input.position = mul(local, input.position);
    input.position = mul(world, input.position);
    
    // “Š‰es—ñ‚ğ‚©‚¯‚ÄÀ•W‚ğ•ÏŠ·
    output.position = mul(lightCameraMat[lightCameraIndex], input.position);
    
    return output;
}
