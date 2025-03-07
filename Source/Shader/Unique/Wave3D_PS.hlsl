#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_MODEL

// 共通
#include "../Header/Common.hlsli"


#include "../Header/PixelShader.hlsli"

// IN
#include "../Header/VertexToPixel.hlsli"
#define PS_INPUT VertexToPixel

float3 CalcLambertDiffuse(float3 lightDir, float3 lightColor, float3 normal)
{
    float t = dot(lightDir, normal);
    t *= -1.0f;
    t = max(0.0f, t);
    
    // 拡散反射光
    float3 difLig = lightColor * t;
    
    return difLig;
}

float3 CalcPhongSpecular(float3 lightDir, float3 lightColor, float3 pos, float3 normal, float3 toEye)
{
    // 反射ベクトルを求める
    float3 ref = reflect(lightDir, normal);
    
    float t = dot(ref, toEye);
    t = max(0.0f, t);
    
    t = pow(t, 5.0f);
    
    float3 specLig = lightColor * t;
    
    return specLig;
}

cbuffer Wave : register(b4)
{
    float time;
    float3 wavePadding;
};


Texture2D<float> heightMap : register(t18);
SamplerState heightSampler : register(s6);

float4 main(PS_INPUT input) : SV_TARGET
{
    
    float heightScale = 50.0f;
    float2 texelSize = float2(1.0f / 32.0f, 1.0f / 32.0f);
    
    input.uv.x = (input.uv.x * 31.0) / 32.0;
    input.uv.y = (input.uv.y * 31.0) / 32.0;
    
    
    float height = heightMap.Sample(heightSampler, input.uv);
    
    
    //return float4(height, height, height, 1);
    
        // 現在のUV座標
    float2 uv = input.uv;

    // 周囲の高さをサンプリング
    float heightL = heightMap.Sample(heightSampler, uv + float2(-texelSize.x, 0)); // 左
    float heightR = heightMap.Sample(heightSampler, uv + float2(texelSize.x, 0)); // 右
    float heightT = heightMap.Sample(heightSampler, uv + float2(0, -texelSize.y)); // 上
    float heightB = heightMap.Sample(heightSampler, uv + float2(0, texelSize.y)); // 下

    // 勾配を計算
    float Gx = (heightR - heightL) * heightScale; // X方向の勾配
    float Gy = (heightB - heightT) * heightScale; // Y方向の勾配

    // 法線ベクトルを計算
    float3 localNormal = normalize(float3(-Gx, -Gy, 1.0));
    
    localNormal *= 2.0;


    
    
    float4 color = float4(0.0, 183.0 / 255.0, 206 / 255.0, 0.5);
    
    if (color.a < 0.01f)
    {
        discard;
    }
    
    //float rate = 1.0 / 256.0;
    
    //int indexX = ((int) (input.uv.x / rate));
    //int indexY = ((int) (input.uv.y / rate));
    
    //float3 localNormal = waveBuffer[indexX].normalMap[indexY].xyz;
    
    
    input.uv.x += time;
    input.uv.y += time;

    input.uv *= 1.5;
    
    float3 texNormal = normalMapTexture.Sample(normalMapSampler, input.uv).xyz;

    float3 tNormal = input.tangent.xyz * texNormal.x 
           + input.biNormal.xyz * texNormal.y 
           + input.normal.xyz * texNormal.z;
    
    float3 normal = input.normal.xyz;
    
    normal = input.tangent.xyz * localNormal.x 
           + input.biNormal.xyz * localNormal.y 
           + input.normal.xyz * localNormal.z;
    
    normal += tNormal * 0.65f;
    
    normal = normalize(normal);
    

     // 視線ベクトル
    float3 toEye = normalize(cameraPos.xyz - input.worldPos.xyz);
    
    float3 lig = 0.0f;
    
    
    // ディレクショナルライト
    float3 dDifLig = 0.0f;
    float3 dSpecLig = 0.0f;
    for (uint i = 0; i < directionalLightCount; i++)
    {
        float3 lightDir = directionalLights[i].direction;
        float3 lightColor = directionalLights[i].color;
   
        // 拡散反射光
        float3 difLig = CalcLambertDiffuse(lightDir, lightColor, normal);
        
        // 鏡面反射光
        float3 specLig = CalcPhongSpecular(lightDir, lightColor, input.worldPos.xyz, normal, toEye);
        
        dDifLig += difLig;
        dSpecLig += specLig;
    }
    
    // ポイントライト
    float3 pDifLig = 0.0f;
    float3 pSpecLig = 0.0f;
    for (uint j = 0; j < pointLightCount; j++)
    {
        float3 lightColor = pointLights[j].color;
        float3 lightPos = pointLights[j].position;
        float lightPow = pointLights[j].intensity;
        float lightRange = pointLights[j].radius;
        
        float distance = length(input.worldPos.xyz - lightPos);
        
        if (distance < lightRange)
        {
            float3 ligDir = input.worldPos.xyz - lightPos;
            ligDir = normalize(ligDir);
        
            // 拡散反射光
            float3 difLig = CalcLambertDiffuse(ligDir, lightColor, normal);
        
            // 鏡面反射光
            float3 specLig = CalcPhongSpecular(ligDir, lightColor, input.worldPos.xyz, normal, toEye);
    
            float distance = length(input.worldPos.xyz - lightPos);
        
            float affect = 1.0f - 1.0f / lightRange * distance;
            affect = max(affect, 0.0f);
        
            affect = pow(affect, 5.0f);
        
            difLig *= affect;
            specLig *= affect;
        
            pDifLig += difLig;
            pSpecLig += specLig;
            

        }
    }
    
    // スポットライト
    float3 sDifLig = 0.0f;
    float3 sSpecLig = 0.0f;
    for (uint k = 0; k < spotLightCount; k++)
    {
        float3 lightColor = spotLights[k].color;
        float3 lightPos = spotLights[k].position;
        float lightRange = spotLights[k].range;
        float3 lightDir = spotLights[k].direction;
        float lightAngle = spotLights[k].angle;
        
        float distance = length(input.worldPos.xyz - lightPos);
 
        
        if (distance < lightRange)
        {
            float3 ligDir = input.worldPos.xyz - lightPos;
            ligDir = normalize(ligDir);
        
            // 拡散反射光
            float3 difLig = CalcLambertDiffuse(ligDir, lightColor, normal);
        
            // 鏡面反射光
            float3 specLig = CalcPhongSpecular(ligDir, lightColor, input.worldPos.xyz, normal, toEye);
    
            float distance = length(input.worldPos.xyz - lightPos);
        
            float affect = 1.0f - 1.0f / lightRange * distance;
            affect = max(affect, 0.0f);
        
            affect = pow(affect, 5.0f);
        
            difLig *= affect;
            specLig *= affect;
        
            float angle = dot(ligDir, lightDir);
        
            angle = abs(acos(angle));
        
        
            affect = 1.0f - 1.0f / lightAngle * angle;
            affect = max(affect, 0.0f);
        
            affect = pow(affect, 0.6f);

            difLig *= affect;
            specLig *= affect;
        
            sDifLig += difLig;
            sSpecLig += specLig;
            
        }


        
    }
    
    // 半球ライト
    float3 ambient = 1.0f;
    //for (uint l = 0; l < hemisphereLightCount; l++)
    //{
    //    float3 groundColor = hemisphereLights[l].groundColor;
    //    float3 skyColor = hemisphereLights[l].skyColor;
    //    float3 groundNormal = hemisphereLights[l].groundNormal;
        
    //    float t = dot(input.normal.xyz, groundNormal);
    //    t = (t + 1.0f) / 2.0f;
        
    //    ambient = lerp(groundColor, skyColor, t);
    //}
    
    
    lig = (dDifLig + pDifLig + sDifLig) +
          (dSpecLig + pSpecLig + sSpecLig);

    
    lig.xyz += ambient * 0.25f;
    
    
    color.rgb *= lig;
    
    return float4(saturate(color.rgb), color.a);

}