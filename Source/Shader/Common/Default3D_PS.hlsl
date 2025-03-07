#include "../Header/RenderType.hlsli"

#define RENDER_TYPE DEFAULT_MODEL

// ����
#include "../Header/Common.hlsli"


#include "../Header/PixelShader.hlsli"

// IN
#include "../Header/VertexToPixel.hlsli"
#define PS_INPUT VertexToPixel

float3 CalcLambertDiffuse(float3 lightDir,float3 lightColor,float3 normal)
{
    float t = dot(lightDir, normal);
    t *= -1.0;
    t = max(0.0, t);
    
    // �g�U���ˌ�
    float3 difLig = lightColor * t;
    
    return difLig;
}

float3 CalcPhongSpecular(float3 lightDir, float3 lightColor,float3 pos ,float3 normal, float3 toEye)
{
    // ���˃x�N�g�������߂�
    float3 ref = reflect(lightDir, normal);
    
    float t = dot(ref, toEye);
    t = max(0.0, t);
    
    t = pow(t, 5.0);
    
    float3 specLig = lightColor * t;
    
    return specLig;
}


float4 main(PS_INPUT input) : SV_TARGET
{
    
    // LOD�̌v�Z
    float lod = diffuseMapTexture.CalculateLevelOfDetail(diffuseMapSampler, input.uv);
    
    float4 color = diffuseMapTexture.SampleLevel(diffuseMapSampler, input.uv, lod);
    
    float a = color.a * alpha;
    
    if (a < 0.01)
    {
        discard;
    }
    
    
    //return color;
    
    float3 localNormal = normalMapTexture.SampleLevel(normalMapSampler, input.uv, lod).xyz;

    
    float3 normal = input.normal.xyz;
    
    normal = input.tangent.xyz * localNormal.x 
           + input.biNormal.xyz * localNormal.y 
           + input.normal.xyz * localNormal.z;
    

    float specPow = specularMapTexture.SampleLevel(specularMapSampler, input.uv, lod).r;

    // �����x�N�g��
    float3 toEye = normalize(cameraPos.xyz - input.worldPos.xyz);
    
    float3 lig = 0.0;
    
    
    // �f�B���N�V���i�����C�g
    float3 dDifLig = 0.0;
    float3 dSpecLig = 0.0;
    for (uint i = 0; i < directionalLightCount; i++)
    {
        float3 lightDir = directionalLights[i].direction;
        float3 lightColor = directionalLights[i].color;
   
        // �g�U���ˌ�
        float3 difLig = CalcLambertDiffuse(lightDir, lightColor, normal);
        
        // ���ʔ��ˌ�
        float3 specLig = CalcPhongSpecular(lightDir, lightColor, input.worldPos.xyz, normal, toEye);
        
        dDifLig += difLig;
        dSpecLig += specLig;
    }
    
    // �|�C���g���C�g
    float3 pDifLig = 0.0;
    float3 pSpecLig = 0.0;
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
        
            // �g�U���ˌ�
            float3 difLig = CalcLambertDiffuse(ligDir, lightColor, normal);
        
            // ���ʔ��ˌ�
            float3 specLig = CalcPhongSpecular(ligDir, lightColor, input.worldPos.xyz, normal, toEye);
    
            float distance = length(input.worldPos.xyz - lightPos);
        
            float affect = 1.0 - 1.0 / lightRange * distance;
            affect = max(affect, 0.0);
        
            affect = pow(affect, 5.0);
        
            difLig *= affect;
            specLig *= affect;
        
            pDifLig += difLig;
            pSpecLig += specLig;
            

        }
    }
    
    // �X�|�b�g���C�g
    float3 sDifLig = 0.0;
    float3 sSpecLig = 0.0;
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
        
            // �g�U���ˌ�
            float3 difLig = CalcLambertDiffuse(ligDir, lightColor, normal);
        
            // ���ʔ��ˌ�
            float3 specLig = CalcPhongSpecular(ligDir, lightColor, input.worldPos.xyz, normal, toEye);
    
            float distance = length(input.worldPos.xyz - lightPos);
        
            float affect = 1.0 - 1.0 / lightRange * distance;
            affect = max(affect, 0.0);
        
            affect = pow(affect, 5.0);
        
            difLig *= affect;
            specLig *= affect;
        
            float angle = dot(ligDir, lightDir);
        
            angle = abs(acos(angle));
        
        
            affect = 1.0 - 1.0 / lightAngle * angle;
            affect = max(affect, 0.0);
        
            affect = pow(affect, 0.6);

            difLig *= affect;
            specLig *= affect;
        
            sDifLig += difLig;
            sSpecLig += specLig;
            
        }

    }
    
    // �������C�g
    float3 ambient = 0.5;
    for (uint l = 0; l < hemisphereLightCount; l++)
    {
        float3 groundColor = hemisphereLights[l].groundColor;
        float3 skyColor = hemisphereLights[l].skyColor;
        float3 groundNormal = hemisphereLights[l].groundNormal;
        
        float t = dot(input.normal.xyz, groundNormal);
        t = (t + 1.0) / 2.0;
        
        ambient = lerp(groundColor, skyColor, t);
    }
    
    lig = dDifLig + pDifLig + sDifLig +
          ((dSpecLig + pSpecLig + sSpecLig) * specPow);
    
    // �e�v�Z
    float shadowWeight = 1.0f;
    [unroll]
    for (uint m = 0; m < useLightCameraCount; m++)
    {
        // ���C�g���W�n(P)
        float4 lcPos = mul(lightCameraMat[m], input.worldPos);
        
        float2 shadowUV = lcPos.xy / lcPos.w;
        shadowUV *= float2(0.5, -0.5);
        shadowUV += 0.5;
        
        float zInLVP = lcPos.z / lcPos.w;
        
        float3 posFromLightVP = lcPos.xyz / lcPos.w;

        if (shadowUV.x > 0.0 && shadowUV.x < 1.0 &&
            shadowUV.y > 0.0 && shadowUV.y < 1.0)
        {
            
            float depthFromLight = shadowMaps.Sample(shadowMapSampler, float3(shadowUV, m)).r;
        
            if (zInLVP > depthFromLight + 0.0001)
            {
                shadowWeight = 0.25;
                break;
            }
        }
    }

    //float shadowWeight = 1.0f;
    //for (uint m = 0; m < useLightCameraCount; m++)
    //{
    //// ���C�g��Ԃł̈ʒu�v�Z
    //    float4 lcPos = mul(lightCameraProj[m], float4(input.worldPos.xyz, 1.0f));
    //    float3 posFromLightVP = lcPos.xyz / lcPos.w;
    
    //// UV���W�֕ϊ�
    //    float2 shadowUV = (posFromLightVP.xy + float2(1.0, -1.0)) * float2(0.5, -0.5);
    
    //// �V���h�E�}�b�v�͈͓̔��`�F�b�N
    //    if (shadowUV.x > 0.0f && shadowUV.x < 1.0f &&
    //    shadowUV.y > 0.0f && shadowUV.y < 1.0f &&
    //    posFromLightVP.z > 0.0f && posFromLightVP.z < 1.0f) // �N���b�v�X�y�[�X�͈͓̔�
    //    {
    //    // �V���h�E�}�b�v����l���T���v�����O
    //        float depthFromLight = shadowMaps[m].Sample(shadowMapSampler, shadowUV).r;

    //    // ��r���ăV���h�E����
    //        if (depthFromLight < posFromLightVP.z - 0.001f)
    //        {
    //            shadowWeight = 0.5f; // �e�̉e���x
    //            break;
    //        }
    //    }
    //}
    
    //float shadowWeight = 1.0f;
    //for (uint m = 0; m < useLightCameraCount; m++)
    //{
    //    float4 lcPos = mul(lightCameraProj[m], float4(input.worldPos.xyz, 1.0f));
    //    float3 posFromLightVP = lcPos.xyz / lcPos.w;

    //// UV���W�֕ϊ�
    //    float2 shadowUV = (posFromLightVP.xy + float2(1.0, 1.0)) * 0.5;

    //    if (shadowUV.x > 0.0f && shadowUV.x < 1.0f &&
    //    shadowUV.y > 0.0f && shadowUV.y < 1.0f &&
    //    posFromLightVP.z > 0.0f && posFromLightVP.z < 1.0f)
    //    {
    //        float depthFromLight = shadowMaps[m].Sample(shadowMapSampler, shadowUV).r;
        
    //        if (depthFromLight < 1.0f)
    //        {
    //            shadowWeight = 0.5f;
    //            break;
    //        }
    //    }
    //}
    
    lig.xyz += ambient * 0.15;
    lig.xyz *= shadowWeight;
    
    color.rgb *= lig;
    
    
    return float4(saturate(color.rgb), a);

}