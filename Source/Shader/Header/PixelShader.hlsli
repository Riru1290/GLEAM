#include "RenderType.hlsli"


#ifdef RENDER_TYPE

#if (RENDER_TYPE != DEFAULT_2D && RENDER_TYPE != INSTANCING_PRIMITIVE_2D && RENDER_TYPE != INSTANCING_PRIMITIVE_2D && RENDER_TYPE != INSTANCING_PRIMITIVE_3D && RENDER_TYPE != POST_EFFECT)

Texture2D diffuseMapTexture : register(t4);
SamplerState diffuseMapSampler : register(s0);

Texture2D specularMapTexture : register(t5);
SamplerState specularMapSampler : register(s1);

Texture2D normalMapTexture : register(t6);
SamplerState normalMapSampler : register(s2);

Texture2D aoMapTexture : register(t7);
SamplerState aoMapSampler : register(s3);

Texture2DArray shadowMaps : register(t8);
SamplerState shadowMapSampler : register(s4);


#elif (RENDER_TYPE == DEFAULT_2D || RENDER_TYPE == POST_EFFECT)

Texture2D diffuseMapTexture : register(t0);
SamplerState diffuseMapSampler : register(s0);

Texture2D specularMapTexture : register(t1);
SamplerState specularMapSampler : register(s1);

Texture2D normalMapTexture : register(t2);
SamplerState normalMapSampler : register(s2);

Texture2D aoMapTexture : register(t3);
SamplerState aoMapSampler : register(s3);

Texture2DArray shadowMaps : register(t4);
SamplerState shadowMapSampler : register(s4);

#if (RENDER_TYPE == POST_EFFECT)
Texture2D postEffectTexture : register(t5);
SamplerState postEffectMapSampler : register(s5);
#endif

#endif

#endif