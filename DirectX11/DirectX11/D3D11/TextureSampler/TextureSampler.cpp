#include "TextureSampler.h"
#include <d3d11.h>


TextureSampler::TextureSampler() {

}
TextureSampler::~TextureSampler() {

}




void TextureSampler::InitSampler(ID3D11Device*& device, ID3D11SamplerState** sampler) {
	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc = {};
	
	

	//TODO: アドレッシングモードはいつでも変更できるような設計にする
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;//繰り返す
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&SamDesc, sampler);
}