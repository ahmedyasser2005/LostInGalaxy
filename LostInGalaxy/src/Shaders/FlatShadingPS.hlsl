Texture2D tex : register(t0);
SamplerState sam : register(s0);

struct PSIn
{
    float4 position : SV_POSITION;
    float diffuse : DIFFUSE;
    float2 uv : UV;
};

static const float4 ambient = { 0.2f, 0.2f, 0.2f, 1.0f };

float4 main(PSIn input) : SV_TARGET
{
    float4 color = tex.Sample(sam, input.uv);
    return max(color * input.diffuse, color * ambient);
}
