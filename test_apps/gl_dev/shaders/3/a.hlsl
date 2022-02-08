#include"c.h"

cbuffer Transform : register(b0)
{
	float4x4 transform;
	
};

VSOut main(float2 coord : COORD_2D, float2 tex_coord : TEX_COORD)
{
	float4 c1 = mul(transform, float4(coord.x, coord.y, 0, 1));
	//coord *= a;
	coord = c1.xy;

	VSOut o;
	o.pos = float4(coord.x, coord.y, 0, 1);
	o.org = coord;
	o.tex = tex_coord;
	return o;
}
