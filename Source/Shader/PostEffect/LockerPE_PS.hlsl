#include "../Header/RenderType.hlsli"

#define RENDER_TYPE POST_EFFECT

// 共通
#include "../Header/Common.hlsli"

#include "../Header/PixelShader.hlsli"

// IN
#include "../Header/VertexToPixel.hlsli"
#define PS_INPUT VertexToPixel2D

float4 main(PS_INPUT input) : SV_TARGET
{
    // 元のテクスチャカラーを取得
    float4 color = postEffectTexture.Sample(postEffectMapSampler, input.uv);
    
    // 透明度がほぼゼロなら破棄
    if (color.a < 0.01f)
    {
        discard;
    }
    
    // UV座標の中心を (0.5, 0.5) として正規化
    float2 uv = input.uv - float2(0.5, 0.5);

    // 中心からの距離を計算
    float dist = length(uv) * 2.0; // 2.0 を掛けて範囲を調整

    // ビネット効果の強度を決定
    float vignette = saturate(1.0 - dist * dist);

    // ビネット効果の強度を調整（数値を変更するとエフェクトの強さを変えられる）
    float strength = 0.5; // 0.0 に近いほど強いビネット効果
    vignette = lerp(strength, 1.0, vignette);

    // ビネット効果を適用
    color.rgb *= vignette;

    return color;

}
