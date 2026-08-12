cbuffer viewProjectionTransformationMatrix : register(b0)
{
    float4x4 viewMat;
    float4x4 projMat;
};
cbuffer worldTransformationMatrix : register(b1)
{
    float4x4 worldMat;
};
cbuffer lightWorldPosition : register(b2)
{
    float4 lightWorldPosition[4];
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
    float3 lightViewPosition[4] : lPOSITION;
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

    vsout.vertexViewPosition = mul(float4(vsin.position, 1.0f), worldViewMat).xyz;
    
    for (int i = 0; i < 4; ++i)
    {
        /*************************************************************************************
        * the W component in the vector must be 1.0f for positions to allow translating,     *
        * however, the bug I found was when creating a DirectX::XMVECTOR(),                  *
        * it implicitly sets the W component to 0.0f so I have to explicitly set it to 1.0f. *
        **************************************************************************************/
        vsout.lightViewPosition[i] = mul(float4(lightWorldPosition[i].xyz, 1.0f), viewMat).xyz;
    }

    vsout.normalViewDirection = mul(float4(vsin.normal, 0.0f), worldViewMat).xyz;
    
    return vsout;
}