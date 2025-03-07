#include "../Header/RenderType.hlsli"

#define RENDER_TYPE ANIMATION_MODEL

// ã§í 
#include "../Header/Common.hlsli"



// IN
#include "../Header/VertexShader.hlsli"

// OUT
#include "../Header/VertexToPixel.hlsli"
#define VS_OUTPUT VertexToPixel

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
    


    // ìäâeçsóÒÇÇ©ÇØÇƒç¿ïWÇïœä∑
    output.position = mul(viewproj, input.position);
    
    
    input.normal.w = 0;
    input.tangent.w = 0;
    input.biNormal.w = 0;
    
    output.tangent = mul(clmat, input.tangent);
    output.tangent = mul(local, output.tangent);
    output.tangent = mul(world, output.tangent);
    
    output.biNormal = mul(clmat, input.biNormal);
    output.biNormal = mul(local, output.biNormal);
    output.biNormal = mul(world, output.biNormal);
    
    output.normal = mul(clmat, input.normal);
    output.normal = mul(local, output.normal);
    output.normal = mul(world, output.normal);
    
    output.uv = input.uv;
    

    
    output.worldPos = input.position;
    
    return output;
}