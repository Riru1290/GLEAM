
struct VertexToPixel
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 tangent : TANGENT;
    float4 biNormal : BINORMAL;
    float4 worldPos : TEXCOORD1;
};

struct VertexToPixel2D
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct VertexToPixelPrimitive
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float4 color : TEXCOORD;
};

struct VertexToShadow
{
    float4 position : SV_POSITION;
};
