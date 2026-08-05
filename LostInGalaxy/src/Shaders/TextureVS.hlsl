cbuffer WMat : register(b0)
{
    float4x4 world;
};

cbuffer VPMat : register(b1)
{
    float4x4 viewproj;
};

struct VSIn
{
    float3 position : POSITION;
    float2 uv : UV;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

VSOut main(VSIn input)
{
    VSOut output;

    output.position = mul(mul(float4(input.position, 1.0f), world), viewproj);
    output.uv = input.uv;

    return output;
}
