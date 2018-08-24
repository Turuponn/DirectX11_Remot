#include "RenderTargetManager.h"
#include <d3d11.h>
#include "D3D11\RenderTarget\RenderTarget.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "constance.h"


RenderTargetManager::RenderTargetManager() {
	_rendertargetview = nullptr;
}
RenderTargetManager::~RenderTargetManager() {
	SAFE_RELEASE(_rendertargetview);
}

void RenderTargetManager::CreateRenderTarget(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	std::shared_ptr<RenderTarget> r(new RenderTarget());
	r->CreateRenderTarget(device->GetSwapChain(), device->GetDevice(),&_rendertargetview);
	_rendertarget = r;
}

ID3D11RenderTargetView*& RenderTargetManager::GetRenderTargetView() {
	return _rendertargetview;
}
