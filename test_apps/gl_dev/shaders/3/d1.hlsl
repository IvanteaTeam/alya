
struct OUT
{
	float4 pos : SV_POSITION;
	float2 coord : TEX_COORD;
};

struct VSOut
{
	float4 pos : SV_POSITION;
	float2 tc : TEX_COORD;
};

VSOut main(uint i : SV_VertexId)
{
	/*
	OUT o;
	if (i == 0)
	{
		o.pos = float4(-1, 1, 0, 1);
		o.coord = float2(0, 0);
	}
	else if (i == 2)
	{
		o.pos = float4(1, 1, 0, 1);
		o.coord = float2(1, 0);
	}
	else if(i == 3)
	{
		o.pos = float4(1, -1, 0, 1);
		o.coord = float2(1, 1);
	}
	else
	{
		o.pos = float4(-1, -1, 0, 1);
		o.coord = float2(0, 1);
	}
	*/

	VSOut output;

	const float2 rect[] = {
		float2(-1, 1),
		float2(1, 1),
		float2(1, -1),
		float2(-1, -1)
	};

	float2 self = float2(0, 0);

	switch (i)
	{
	case 0:
		self = rect[0];
		break;
	case 1:
		self = rect[1];
		break;
	case 2:
		self = rect[3];
		break;
	case 3:
		self = rect[3];
		break;
	case 4:
		self = rect[1];
		break;
	case 5:
		self = rect[2];
		break;
	}
	output.pos = float4(self, 0, 1);
	
	output.tc = clamp(float2(output.pos.x, -output.pos.y), 0, 1);
	
	return output;
}
