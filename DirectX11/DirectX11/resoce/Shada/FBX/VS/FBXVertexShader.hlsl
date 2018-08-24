#include "../FBXHeader.hlsli"
#include "../../Camera/Camera.hlsli"
#include "../../TextureSmpler/TextureSmpler.hlsli"

OutputFbxModel FbxVS(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, float4 color : COLOR)
{

	OutputFbxModel output;

	pos = mul(mul(viewproj, world), pos);


	output.svpos = pos;
	output.pos = pos;
	output.normal = mul(world, normal);
	output.uv = uv;
	output.color = color;


	return output;
}