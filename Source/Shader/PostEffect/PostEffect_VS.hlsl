#include "../Header/RenderType.hlsli"

#define RENDER_TYPE POST_EFFECT

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
    
    // “Š‰es—ñ‚ğ‚©‚¯‚ÄÀ•W‚ğ•ÏŠ·
    output.position = input.position;
    output.uv = input.uv;
    
    return output;
}
