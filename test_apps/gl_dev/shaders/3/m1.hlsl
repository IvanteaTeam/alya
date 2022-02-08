
cbuffer Transform : register(b0)
{
    float4x4 model, view, proj;
    float3 color;
};

struct Out
{
    float3 color : COLOR;
    float2 tc : TEX_COORD;
    float4 pos : SV_POSITION;
};

Out main(float3 pos : POSITION, float2 tc : TEX_COORD)
{
    float4 p = float4(pos.x, pos.y, pos.z, 1);
    p  = mul(mul(mul(proj, view), model), p);

    Out o;
    o.color = color;
    o.pos = p;
    o.tc = tc;
    return o;
}
