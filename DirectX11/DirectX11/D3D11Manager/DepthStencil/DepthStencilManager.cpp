#include "DepthStencilManager.h"
#include "D3D11\DepthStencil\DepthStencil.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"

DepthStencilManager::DepthStencilManager() {

}
DepthStencilManager::~DepthStencilManager() {

}


void DepthStencilManager::CreateDepthStencilView(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	std::shared_ptr<DepthStencil> d(new DepthStencil());
	d->CreateDepthStencil(device->GetDevice(), &_depthStencilView);
}

ID3D11DepthStencilView*& DepthStencilManager::GetDepthStencilView() {
	return _depthStencilView;
}