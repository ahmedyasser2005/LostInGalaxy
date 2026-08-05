Texture2D tex : register(t0);
SamplerState sam : register(s0);

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 color = tex.Sample(sam, input.uv);
    return color;
}
