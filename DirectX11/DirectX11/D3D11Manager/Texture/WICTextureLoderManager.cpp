#include "WICTextureLoderManager.h"
#include <d3d11.h>
#include "D3D11\WICTextureLoaderCreate\WICTextureLoaderCreate.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "constance.h"

WICTextureLoderManager::WICTextureLoderManager() {
	_texbuff = nullptr;
	_texSrvView = nullptr;
}
WICTextureLoderManager::~WICTextureLoderManager() {
	SAFE_RELEASE(_texbuff);
	SAFE_RELEASE(_texSrvView);
}

void WICTextureLoderManager::LoadTexture(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,const std::wstring& filepath) {
	
	//ÉçÅ[É_
	std::shared_ptr<WICTextureLoaderCreate> ctex(new WICTextureLoaderCreate());
	ctex->LoadTexture(device->GetDevice(), device->GetDeviceContext(), filepath, &_texbuff,&_texSrvView);
}
ID3D11ShaderResourceView*& WICTextureLoderManager::GetTexView() {
	return _texSrvView;
}
ID3D11Resource*& WICTextureLoderManager::GetTexbuff() {
	return _texbuff;
}