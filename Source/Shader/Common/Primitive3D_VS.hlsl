#include "../Header/RenderType.hlsli"

#define RENDER_TYPE INSTANCING_PRIMITIVE_3D

// 共通
#include "../Header/Common.hlsli"

// IN
#include "../Header/VertexShader.hlsli"

// OUT
#include "../Header/VertexToPixel.hlsli"
#define VS_OUTPUT VertexToPixelPrimitive

VS_OUTPUT main(VS_INPUT input, uint instanceID : SV_InstanceID)
{
    VS_OUTPUT output;
    
    PRIMITIVE_DATA buf = instanceBuffer[instanceID];
   
    
    input.position = mul(buf.world, input.position);
	
    // 投影行列をかけて座標を変換
    output.position = mul(viewproj, input.position);
    input.normal.w = 0;
    output.normal = mul(buf.world, input.normal);
    
    output.color = buf.color;
    
    return output;
}