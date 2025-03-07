#include "RenderType.hlsli"

#ifdef RENDER_TYPE

// 3D通常描画共通
#if(RENDER_TYPE != DEFAULT_2D && RENDER_TYPE != PRIMITIVE_2D && RENDER_TYPE != INSTANCING_PRIMITIVE_2D && RENDER_TYPE != POST_EFFECT)

// ビュープロジェクション行列
cbuffer vp : register(b0)
{
    matrix viewproj;
};

// ローカルワールド行列
cbuffer lw : register(b1)
{
    matrix local;   //ローカル変換行列
    matrix world;   //ワールド変換行列
    float alpha;    //透明度
    float3 paddingLW;
};

// カメラ
cbuffer Camera  : register(b2)
{
    float3 cameraPos;
    float paddingCamera;
    float3 cameraDir;
    float paddingCamera2;
}

// 半球ライト
struct HEMISPHERELIGHT
{
	float3 skyColor;        // 天球色
    float padding;          // メモリの整列用
	float3 groundColor;     // 地面色
    float padding2;         // メモリの整列用
	float3 groundNormal;    // 地面法線
    float padding3;         // メモリの整列用
};

// ディレクショナルライト
struct DIRECTIONALLIGHT
{
    float3 direction;       // ライトの方向
    float intensity;        // 光の強度
    float3 color;           // ライトの色
    float padding;          // メモリの整列用
};

// ポイントライト
struct POINTLIGHT
{
    float3 position;        // ライトの位置
    float intensity;        // 光の強度
    float3 color;           // ライトの色
    float radius;           // 影響半径
};

// スポットライト
struct SPOTLIGHT
{
    float3 position;        // ライトの位置
    float range;            // 光の影響範囲
    float3 direction;       // ライトの方向
    float angle;            // 照射角度
    float3 color;           // ライトの色
    float padding;          // メモリの整列用
};

StructuredBuffer<HEMISPHERELIGHT> hemisphereLights : register(t0);
StructuredBuffer<DIRECTIONALLIGHT> directionalLights : register(t1);
StructuredBuffer<POINTLIGHT> pointLights : register(t2);
StructuredBuffer<SPOTLIGHT> spotLights : register(t3);

// 各ライトの数
cbuffer LightCountBuffer : register(b3)
{
    uint hemisphereLightCount;
    uint directionalLightCount;
    uint pointLightCount;
    uint spotLightCount;
    uint useLightCameraCount;
    matrix lightCameraMat[10];      // ライトカメラ行列
    matrix lightCameraViewMat[10];  // ライトカメラビュー行列
    matrix lightCameraProjMat[10];  // ライトカメラプロジェクション行列
};


#elif (RENDER_TYPE == DEFAULT_2D || RENDER_TYPE == POST_EFFECT)

cbuffer Camera2D : register(b0)
{
    float2 cameraPos;       // カメラ座標
	float angle;            // カメラ角度
	float lens;             // カメラ倍率
    matrix screenMatrix;    // スクリーン座標変換行列
};

// ローカルワールド行列
cbuffer lw2D : register(b1)
{
    matrix local; //ローカル変換行列
    matrix world; //ワールド変換行列
    float alpha;    //透明度
    float3 paddingLW2D;
};

#endif

#endif
