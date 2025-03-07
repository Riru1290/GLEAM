#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_MODEL

// ‹¤’Ê
#include "../Header/Common.hlsli"

#include "../Header/PixelShader.hlsli"

// IN
#include "../Header/VertexToPixel.hlsli"
#define PS_INPUT VertexToPixel

cbuffer TVScreen : register(b4)
{
    float channel;
    float preChannel;
    float currentTime;
    float changeTime;

    float screenDivideXNum;
    float screenDivideYNum;
    float paddingTV;
    float padding2TV;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 offsetUV = float2(1.0 / screenDivideXNum, 1.0 / screenDivideYNum);
    
    float xNum = (channel % screenDivideXNum);
    float yNum = (trunc(channel / screenDivideXNum));
    
    float2 frontUV = input.uv;
    frontUV.x = (xNum * offsetUV.x) + (input.uv.x * offsetUV.x);
    frontUV.y = (yNum * offsetUV.y) + (input.uv.y * offsetUV.y);
    
    xNum = (preChannel % screenDivideXNum);
    yNum = (trunc(preChannel / screenDivideXNum));
    
    float2 backUV = input.uv;
    backUV.x = (xNum * offsetUV.x) + (input.uv.x * offsetUV.x);
    backUV.y = (yNum * offsetUV.y) + (input.uv.y * offsetUV.y);
    
    float4 frontColor = diffuseMapTexture.Sample(diffuseMapSampler, frontUV);
    float4 backColor = diffuseMapTexture.Sample(diffuseMapSampler, backUV);
    
    frontColor.a = saturate(currentTime / changeTime);
    frontColor.rgb *= frontColor.a;
    
    backColor.a = saturate(1.0f - (currentTime / changeTime));
    backColor.rgb *= backColor.a;
    
    float4 color = float4(frontColor.rgb + backColor.rgb,1.0f);
    
    if (color.a < 0.01f)
    {
        discard;
    }
 
    return color;
}
