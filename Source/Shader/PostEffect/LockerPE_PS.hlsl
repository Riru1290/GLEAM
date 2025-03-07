#include "../Header/RenderType.hlsli"

#define RENDER_TYPE POST_EFFECT

// ����
#include "../Header/Common.hlsli"

#include "../Header/PixelShader.hlsli"

// IN
#include "../Header/VertexToPixel.hlsli"
#define PS_INPUT VertexToPixel2D

float4 main(PS_INPUT input) : SV_TARGET
{
    // ���̃e�N�X�`���J���[���擾
    float4 color = postEffectTexture.Sample(postEffectMapSampler, input.uv);
    
    // �����x���قڃ[���Ȃ�j��
    if (color.a < 0.01f)
    {
        discard;
    }
    
    // UV���W�̒��S�� (0.5, 0.5) �Ƃ��Đ��K��
    float2 uv = input.uv - float2(0.5, 0.5);

    // ���S����̋������v�Z
    float dist = length(uv) * 2.0; // 2.0 ���|���Ĕ͈͂𒲐�

    // �r�l�b�g���ʂ̋��x������
    float vignette = saturate(1.0 - dist * dist);

    // �r�l�b�g���ʂ̋��x�𒲐��i���l��ύX����ƃG�t�F�N�g�̋�����ς�����j
    float strength = 0.5; // 0.0 �ɋ߂��قǋ����r�l�b�g����
    vignette = lerp(strength, 1.0, vignette);

    // �r�l�b�g���ʂ�K�p
    color.rgb *= vignette;

    return color;

}
