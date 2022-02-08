
SamplerState samplerState : register(s0);
Texture2D tex : register(t0);


float4 main(float4 pos : SV_POSITION, float2 tc : TEX_COORD) : SV_TARGET
{
	return tex.Sample(samplerState, tc);
}
