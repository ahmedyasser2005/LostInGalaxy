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
    float4 color : COLOR;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOut main(VSIn input)
{
    VSOut output;

    output.position = mul(mul(float4(input.position, 1.0f), world), viewproj);
    output.color = input.color;

    return output;
}
