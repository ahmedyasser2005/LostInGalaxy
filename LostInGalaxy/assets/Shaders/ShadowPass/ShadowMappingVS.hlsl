cbuffer Object : register(b0)
{
    float4x4 worldMatrix;
};
cbuffer LightCamera : register(b5)
{
    float4x4 lightView;
    float4x4 lightProjection;
};

struct VSIn
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

float4 main(VSIn vsin) : SV_POSITION
{
    const float4x4 wvp = worldMatrix * lightView * lightProjection;
    return mul(float4(vsin.position, 1.0f), wvp);
}
