cbuffer worldTransformationMatrix : register(b0)
{
    float4x4 worldMat;
};
cbuffer viewProjectionTransformationMatrix : register(b1)
{
    float4x4 viewMat;
    float4x4 projMat;
};
cbuffer lightWorldPosition : register(b2)
{
    float3 lightWorldPosition;
    float padding;
};

struct VSIn
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float3 vertexViewPosition : vPOSITION;
    float3 lightViewPosition : lPOSITION;
    float3 normalViewDirection : NORMAL;
    float2 uv : TEXCOORD;
};

VSOut main(VSIn vsin)
{
    VSOut vsout;
    
    float4x4 worldViewMat = mul(worldMat, viewMat);
    float4x4 worldViewProjMat = mul(worldViewMat, projMat);
    
    vsout.position = mul(float4(vsin.position, 1.0f), worldViewProjMat);
    vsout.uv = vsin.uv;

    vsout.vertexViewPosition    = mul(float4(vsin.position,         1.0f), worldViewMat).xyz;
    vsout.lightViewPosition     = mul(float4(lightWorldPosition,    1.0f), viewMat).xyz;
    vsout.normalViewDirection   = mul(float4(vsin.normal,           0.0f), worldViewMat).xyz;
    
    return vsout;
}