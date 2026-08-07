cbuffer WMat : register(b0)
{
    float4x4 world;
};

cbuffer VPMat : register(b1)
{
    float4x4 view;
    float4x4 proj;
};

struct VSIn
{
    float3 position : POSITION;
    float3 normal : NORMAL;
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

    float4x4 worldView = mul(world, view);

    output.position = mul(mul(float4(input.position, 1.0f), worldView), proj);
    output.uv = input.uv;

    return output;
}
