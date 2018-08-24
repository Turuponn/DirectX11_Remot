#include "../FBXHeader.hlsli"
#include "../../Camera/Camera.hlsli"
#include "../../TextureSmpler/TextureSmpler.hlsli"

float4 FbxPS(OutputFbxModel o) : SV_Target
{
	float3 lightdir = float3(1,-1,1);//���s����
	float diffese = dot(o.normal, lightdir);//�����o�[�g


	float4 color = tex.Sample(texsampler, o.uv);
	

	return float4(color.x, color.y, color.z, 1);
}