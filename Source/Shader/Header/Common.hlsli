#include "RenderType.hlsli"

#ifdef RENDER_TYPE

// 3D�ʏ�`�拤��
#if(RENDER_TYPE != DEFAULT_2D && RENDER_TYPE != PRIMITIVE_2D && RENDER_TYPE != INSTANCING_PRIMITIVE_2D && RENDER_TYPE != POST_EFFECT)

// �r���[�v���W�F�N�V�����s��
cbuffer vp : register(b0)
{
    matrix viewproj;
};

// ���[�J�����[���h�s��
cbuffer lw : register(b1)
{
    matrix local;   //���[�J���ϊ��s��
    matrix world;   //���[���h�ϊ��s��
    float alpha;    //�����x
    float3 paddingLW;
};

// �J����
cbuffer Camera  : register(b2)
{
    float3 cameraPos;
    float paddingCamera;
    float3 cameraDir;
    float paddingCamera2;
}

// �������C�g
struct HEMISPHERELIGHT
{
	float3 skyColor;        // �V���F
    float padding;          // �������̐���p
	float3 groundColor;     // �n�ʐF
    float padding2;         // �������̐���p
	float3 groundNormal;    // �n�ʖ@��
    float padding3;         // �������̐���p
};

// �f�B���N�V���i�����C�g
struct DIRECTIONALLIGHT
{
    float3 direction;       // ���C�g�̕���
    float intensity;        // ���̋��x
    float3 color;           // ���C�g�̐F
    float padding;          // �������̐���p
};

// �|�C���g���C�g
struct POINTLIGHT
{
    float3 position;        // ���C�g�̈ʒu
    float intensity;        // ���̋��x
    float3 color;           // ���C�g�̐F
    float radius;           // �e�����a
};

// �X�|�b�g���C�g
struct SPOTLIGHT
{
    float3 position;        // ���C�g�̈ʒu
    float range;            // ���̉e���͈�
    float3 direction;       // ���C�g�̕���
    float angle;            // �Ǝˊp�x
    float3 color;           // ���C�g�̐F
    float padding;          // �������̐���p
};

StructuredBuffer<HEMISPHERELIGHT> hemisphereLights : register(t0);
StructuredBuffer<DIRECTIONALLIGHT> directionalLights : register(t1);
StructuredBuffer<POINTLIGHT> pointLights : register(t2);
StructuredBuffer<SPOTLIGHT> spotLights : register(t3);

// �e���C�g�̐�
cbuffer LightCountBuffer : register(b3)
{
    uint hemisphereLightCount;
    uint directionalLightCount;
    uint pointLightCount;
    uint spotLightCount;
    uint useLightCameraCount;
    matrix lightCameraMat[10];      // ���C�g�J�����s��
    matrix lightCameraViewMat[10];  // ���C�g�J�����r���[�s��
    matrix lightCameraProjMat[10];  // ���C�g�J�����v���W�F�N�V�����s��
};


#elif (RENDER_TYPE == DEFAULT_2D || RENDER_TYPE == POST_EFFECT)

cbuffer Camera2D : register(b0)
{
    float2 cameraPos;       // �J�������W
	float angle;            // �J�����p�x
	float lens;             // �J�����{��
    matrix screenMatrix;    // �X�N���[�����W�ϊ��s��
};

// ���[�J�����[���h�s��
cbuffer lw2D : register(b1)
{
    matrix local; //���[�J���ϊ��s��
    matrix world; //���[���h�ϊ��s��
    float alpha;    //�����x
    float3 paddingLW2D;
};

#endif

#endif
