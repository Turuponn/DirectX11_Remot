

cbuffer fbx:register(b1) {
	float4 diffusecolor;
	float4 ambientcolor;
	float4 specularcolor;
}

//FBX用出力
struct OutputFbxModel {
	float4 svpos : SV_POSITION;//システム座標
	float4 pos : POSITION;//座標(補完あり)
	float4 normal : NORMAL;//法線情報
	float2 uv : TEXCOORD;//UV座標(補完あり)
	float4 color : COLOR;//影響度色
};
