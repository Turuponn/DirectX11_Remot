#include "../FBXHeader.hlsli"
#include "../../Camera/Camera.hlsli"
#include "../../TextureSmpler/TextureSmpler.hlsli"

float4 FbxPS(OutputFbxModel o) : SV_Target
{
	float3 lightdir = float3(1,-1,1);//平行光線
	float diffese = dot(o.normal, lightdir);//ランバート


	float color = diffese;
	

	return float4(color, color, color, 1);
}