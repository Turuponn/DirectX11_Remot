#include "../PrimitivGlobal.hlsli"
#include "../../Camera/Camera.hlsli"
#include "../../TextureSmpler/TextureSmpler.hlsli"

float4 BasicPS(OutputPrimitivModel o) : SV_Target
{
	float3 color = tex.Sample(texsampler,o.uv).rgb;


	return float4(color, 1.0f);

	//return float4(1,1,1,1);
}