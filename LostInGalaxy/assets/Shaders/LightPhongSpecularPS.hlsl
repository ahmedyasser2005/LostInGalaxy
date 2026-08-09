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
    float4 color = tx.Sample(ss, psin.uv);
    
    float3 N = normalize(psin.normalViewDirection);
    float3 L = normalize(psin.lightViewPosition - psin.vertexViewPosition);
    float3 V = normalize(-psin.vertexViewPosition);
    float3 rL = reflect(-L, N);
    
    float3 specular = lightTint * materialColor * lightIntensity * pow(max(dot(rL, V), 0.0f), materialShininess);
    
    float3 finalColor = color.rgb * specular;
    
    return float4(finalColor, color.a);
}
