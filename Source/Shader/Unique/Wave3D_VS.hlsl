#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_MODEL

// ‹¤’Ê
#include "../Header/Common.hlsli"

// IN
#include "../Header/VertexShader.hlsli"

// OUT
#include "../Header/VertexToPixel.hlsli"
#define VS_OUTPUT VertexToPixel



VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    input.position = mul(local, input.position);
    input.position = mul(world, input.position);
    


    // “Š‰es—ñ‚ğ‚©‚¯‚ÄÀ•W‚ğ•ÏŠ·
    output.position = mul(viewproj, input.position);
    
    // –@ü
    input.normal.w = 0;
    input.tangent.w = 0;
    input.biNormal.w = 0;
    
    output.tangent = mul(local, input.tangent);
    output.tangent = mul(world, output.tangent);
    
    output.biNormal = mul(local, input.biNormal);
    output.biNormal = mul(world, output.biNormal);
    
    output.normal = mul(local, input.normal);
    output.normal = mul(world, output.normal);
    output.uv = input.uv;
    
    output.tangent = input.tangent;
    output.biNormal = input.biNormal;
    
    output.worldPos = input.position;
    
    return output;
}

