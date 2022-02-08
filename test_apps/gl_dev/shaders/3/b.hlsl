#include"c.h"

Texture2D tex;
SamplerState samp;

static const float3 color = float3(1, 1, 1);

float2 trans(float2 pos)
{
	return float2((pos.x + 1) / 2, (pos.y + 1) / 2);
}

float4 main(VSOut data) : SV_TARGET
{
	float2 tex_coord = data.tex;
	
	float4 color = float4(0, 0, 0, 0);
	return tex.Sample(samp, tex_coord);
	//for (int i = 0; i < 4; i++)
	//	color += tex.Load(int2(2000.0f * tex_coord.x, 2000.0f * tex_coord.y), i);
	//return (color / 4) + float4(data.tex.x, data.tex.y, 0, 0);
	//float4 first_color = tex.Sample(samp, float3(tex_coord.x, tex_coord.y, 0));
	//float4 second_color = tex.Sample(samp, float3(tex_coord.x, tex_coord.y, 1));
	//return first_color;
	//return (first_color + second_color) / 2;
}
