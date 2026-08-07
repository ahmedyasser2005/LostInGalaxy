Texture2D tex : register(t0);
SamplerState sam : register(s0);

struct PSIn
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 light : LIGHT;
    //float diffuse : DIFFUSE;
    float2 uv : UV;
};


static const float4 ambient = { 0.2f, 0.2f, 0.2f, 1.0f };

float4 main(PSIn input) : SV_TARGET
{
    float3 L = normalize(input.normal);
    float3 N = normalize(input.light);
    float diffuse = max(dot(N, L), 0.0f);
    
    
    
    float4 color = tex.Sample(sam, input.uv);
    //return max(color * input.diffuse, color * ambient);
    return max(color * diffuse, color * ambient);
}
