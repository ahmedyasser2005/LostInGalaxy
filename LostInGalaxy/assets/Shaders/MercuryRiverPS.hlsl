Texture2D tx : register(t0);
SamplerState ss : register(s0);

cbuffer lightProperties : register(b3)
{
    float3 lightTint;
    float padding1;
    float lightIntensity;
    float lightShininess;
    float2 padding2;
};

struct PSIn
{
    float4 position : SV_POSITION;
    float3 vertexViewPosition : vPOSITION;
    float3 lightViewPosition : lPOSITION;
    float3 normalViewDirection : NORMAL;
    float2 uv : TEXCOORD;
};

////////////////////////////////////////////////////
// SOURCE: https://www.shadertoy.com/view/NcK3D3
// NOTE: AI is used to regenerate the code in HLSL
////////////////////////////////////////////////////


// --- Math, HSV & Matrix Constants ---

static const float4 hsv2rgb_K = float4(1.0f, 2.0f / 3.0f, 1.0f / 3.0f, 3.0f);

float3 HSV2RGB(float3 c)
{
    float3 p = abs(frac(c.xxx + hsv2rgb_K.xyz) * 6.0f - hsv2rgb_K.www);
    return c.z * lerp(hsv2rgb_K.xxx, saturate(p - hsv2rgb_K.xxx), c.y);
}

// 2x2 Rotation/Scaling Matrix used in FBM (Row-Major format)
static const float2x2 R = float2x2(
     6.0f / 5.0f, 8.0f / 5.0f,
    -8.0f / 5.0f, 6.0f / 5.0f
);

// --- Global Variables (Evaluated per pixel using position) ---

static float2 g_off0;
static float2 g_off1;
static float2x2 g_rot0;
static float2x2 g_rot1;

float2x2 rot(float a)
{
    float c = cos(a);
    float s = sin(a);
    return float2x2(c, s, -s, c);
}

// --- FBM Core Math ---

float fbm(float2 p)
{
    float a = 1.0f;
    float h = 0.0f;
    float2 D = float2(0.0f, 0.0f);
    float3 w;
    float4 C;

    [unroll(7)]
    for (int i = 0; i < 7; ++i)
    {
        C = cos(p.xxyy + float4(11.0f, 0.0f, 11.0f, 0.0f));
        w = C.yxx * C.zwz;
        D += w.xy;
        h += a * w.z / (3.0f * dot(D, D) + 1.0f);
        a *= 0.45f;
        p = mul(p, R);
        p += 1.23f;
    }

    return 2.0f * h;
}

float warp_fbm(float2 p)
{
    float2 q = float2(fbm(p - g_off0), fbm(p - g_off1));
    
    float2 r = float2(
        fbm(p + 3.0f * q + mul(float2(2.0f, 3.0f), g_rot0)),
        fbm(p + 3.0f * q + mul(float2(3.0f, 4.0f), g_rot1))
    );
    
    return fbm(p - 3.0f * r / (1.0f + 0.125f * dot(q, q)));
}

float hf(float2 p)
{
    return 0.005f * warp_fbm(2.0f * p);
}

float3 nf(float2 p, float aa)
{
    float2 eps = float2(2.0f * aa, 0.0f);

    return normalize(float3(
        hf(p + eps.xy) - hf(p - eps.xy),
        hf(p + eps.yx) - hf(p - eps.yx),
        -2.0f * eps.x
    ));
}

// --- Tone Mapping & Color Conversion ---

float3 aces_approx(float3 v)
{
    const float a = 2.51f;
    const float b = 0.03f;
    const float c = 2.43f;
    const float d = 0.59f;
    const float e = 0.14f;

    v = max(v, 0.0f);
    v *= 0.6f;
    return saturate((v * (a * v + b)) / (v * (c * v + d) + e));
}

float3 sRGB(float3 t)
{
    float3 higher = 1.055f * pow(max(t, 0.0f), 1.0f / 2.4f) - 0.055f;
    float3 lower = 12.92f * t;
    return lerp(higher, lower, step(t, 0.0031308f));
}

// --- Main Pixel Shader Entry Point ---

float4 main(PSIn psin) : SV_TARGET
{
    // 1. Remap texture coordinates to [-1, 1] range centered on the mesh
    float2 p = psin.uv * 2.0f - 1.0f;

    // 2. Derive spatial pseudo-time (T) from the interpolated 3D vertex position
    // As the camera or mesh moves, the mercury warp flows over the surface based on position.
    float spatialSeed = length(psin.vertexViewPosition.xyz) * 0.75f + psin.vertexViewPosition.z * 0.25f;
    float T = spatialSeed;

    // 3. Setup dynamic warping state tied to spatial position
    g_off0 = T * float2(2.0f, 1.0f);
    g_off1 = T * float2(1.0f, 2.0f);
    g_rot0 = rot(T);
    g_rot1 = rot(0.707f * T);

    // 4. Construct light and sky colors dynamically
    float3 SKY = HSV2RGB(float3(0.578f, 0.9f, 0.015f));
    float3 SUN = HSV2RGB(float3(0.033f, 0.9f, 0.010f));

    // 5. Compute view-space ray directions using interpolated view normal & positions
    float3 P = float3(p, 0.0f);
    float3 RO = float3(0.0f, 0.0f, -3.0f);
    float3 RD = normalize(P - RO);
    float3 LD0 = normalize(float3(1.0f, 1.0f, -2.0f) - P);
    float3 LD1 = normalize(float3(0.0f, 1.0f, -2.0f) - P);

    float aa = 0.0025f; // Constant anti-aliasing step
    float h = hf(p);

    float3 N = nf(p, aa);
    float3 R_refl = reflect(RD, N);

    // 6. Calculate liquid specular lighting & sky reflection
    float3 col = float3(0.0f, 0.0f, 0.0f);
    col += SUN / (1.001f - dot(R_refl, LD0));
    col += SKY / (1.040f - dot(R_refl, LD1));
    col *= 300000.0f * h * h + 0.4f;

    // 7. Modulate with lightProperties tint from your cbuffer b3
    col *= lightTint * lightIntensity;

    // 8. Tonemap & Gamma Correction
    col = aces_approx(col);
    col = sRGB(col);

    return float4(col, 1.0f);
}
