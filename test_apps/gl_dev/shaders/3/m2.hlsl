
struct In
{
    float3 color : COLOR;
    float2 tc : TEX_COORD;
};

SamplerState samp : register(s0);
Texture2D tex : register(t0);

float4 main(In i) : SV_TARGET
{
    return tex.Sample(samp, i.tc);
    return float4(i.tc.x, i.tc.y, 0, 1);
    return float4(i.color.x, i.color.y, i.color.z ,1) + tex.Sample(samp, i.tc);
}
