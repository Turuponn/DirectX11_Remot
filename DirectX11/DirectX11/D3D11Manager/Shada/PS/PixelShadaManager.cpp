#include "PixelShadaManager.h"
#include <d3d11.h>
#include "constance.h"
#include "D3D11\Shada\PS\PixelShada.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"

PixelShadaManager::PixelShadaManager() {
	_pixelshada = nullptr;
}
PixelShadaManager::~PixelShadaManager() {
	SAFE_RELEASE(_pixelshada);
}

void PixelShadaManager::CreatePSShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,const TCHAR* filepath,const char* funcname) {
	std::shared_ptr<PixelShada> ps(new PixelShada());
	ps->CreatePixelShada(filepath, funcname, device->GetDevice(), &_pixelshada);
	
}

ID3D11PixelShader*& PixelShadaManager::GetPSShada() {
	return _pixelshada;
}