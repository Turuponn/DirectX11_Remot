#include "TextureSamplerManager.h"
#include <d3d11.h>
#include "D3D11\TextureSampler\TextureSampler.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"



TextureSamplerManager::TextureSamplerManager() {

}
TextureSamplerManager::~TextureSamplerManager() {

}


void TextureSamplerManager::CreateTextureSampler(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	std::shared_ptr<TextureSampler> tex(new TextureSampler());
	tex->InitSampler(device->GetDevice(), &_texsampler);
}

ID3D11SamplerState*& TextureSamplerManager::GetTextureSampler() {
	return _texsampler;
}