Texture2D tx : register(t0);
SamplerState ss : register(s0);

struct LightData
{
    float3 lightTint;
    float lightIntensity;
};

cbuffer lightData : register(b3)
{
    LightData light[4];
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
    float3 lightViewPosition[4] : lPOSITION;
    float3 normalViewDirection : NORMAL;
    float2 uv : TEXCOORD;
};

float4 main(PSIn psin) : SV_TARGET
{
    float4 color = tx.Sample(ss, psin.uv);
    
    float3 N = normalize(psin.normalViewDirection);
    float3 L[4];
    for (int i = 0; i < 4; ++i)
    {
        L[i] = normalize(psin.lightViewPosition[i] - psin.vertexViewPosition);
    }
    float3 V = normalize(-psin.vertexViewPosition);

    float3 H[4];
    for (int i = 0; i < 4; ++i)
    {
        H[i] = normalize(L[i] + V);
    }

    float3 finalColor = { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < 4; ++i)
    {
        float3 specular = light[i].lightTint * materialColor * light[i].lightIntensity * pow(max(dot(H[i], N), 0.0f), materialShininess);
        float3 diffuse = light[i].lightTint * materialColor * light[i].lightIntensity * max(dot(L[i], N), 0.0f);
        float3 ambient = light[i].lightTint * materialColor * 0.2;

        finalColor += color.rgb * (specular + ambient + diffuse);
    }
    
    return float4(finalColor, color.a);
}
