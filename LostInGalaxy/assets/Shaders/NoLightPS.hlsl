Texture2D tx : register(t0);
SamplerState ss : register(s0);

cbuffer lightData : register(b3)
{
    float3 lightTint;
    float lightIntensity;
};

cbuffer materialData : register(b4)
{
    float3 materialColor;
    float materialShininess;
};

struct PSIn
{
    float4 position : SV_POSITION;
    float3 vertexViewPosition : vPOSITION;
    float3 lightViewPosition : lPOSITION;
    float3 normalViewDirection : NORMAL;
    float2 uv : TEXCOORD;
};

float4 main(PSIn psin) : SV_TARGET
{
    return tx.Sample(ss, psin.uv);
}
