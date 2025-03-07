#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_MODEL

// 共通
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

    // 投影行列をかけて座標を変換
    output.position = mul(viewproj, input.position);
    
    // 法線
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
    
    // alpha をダミー参照（影響しない操作で良い）
    float dummy = alpha;
    
    return output;
}