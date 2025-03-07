#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_2D

// ‹¤’Ê
#include "../Header/Common.hlsli"

// IN
#include "../Header/VertexShader.hlsli"

// OUT
#include "../Header/VertexToPixel.hlsli"
#define VS_OUTPUT VertexToPixel2D

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    input.position.xy -= cameraPos;
    
    // “Š‰es—ñ‚ğ‚©‚¯‚ÄÀ•W‚ğ•ÏŠ·
    output.position = mul(screenMatrix, input.position);
    output.uv = input.uv;
    
    return output;
}
