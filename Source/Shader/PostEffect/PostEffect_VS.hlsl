#include "../Header/RenderType.hlsli"

#define RENDER_TYPE POST_EFFECT

// 共通
#include "../Header/Common.hlsli"

// IN
#include "../Header/VertexShader.hlsli"

// OUT
#include "../Header/VertexToPixel.hlsli"
#define VS_OUTPUT VertexToPixel2D

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // 投影行列をかけて座標を変換
    output.position = input.position;
    output.uv = input.uv;
    
    return output;
}
