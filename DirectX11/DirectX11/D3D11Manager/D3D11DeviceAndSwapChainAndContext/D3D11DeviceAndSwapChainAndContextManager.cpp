#include "D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContext.h"
#include <d3d11.h>
#include "WinManager.h"
#include "constance.h"
#include <dxgi.h>

D3D11DeviceAndSwapChainAndContextManager::D3D11DeviceAndSwapChainAndContextManager() {
	_device = nullptr;
	_swapchain = nullptr;
	_context = nullptr;
}
D3D11DeviceAndSwapChainAndContextManager::~D3D11DeviceAndSwapChainAndContextManager() {
	SAFE_RELEASE(_device);
	SAFE_RELEASE(_swapchain);
	SAFE_RELEASE(_context);
}
void D3D11DeviceAndSwapChainAndContextManager::CreateDevice(WinHWND& hwnd) {
	std::shared_ptr<D3D11DeviceAndSwapChainAndContext> _d(new D3D11DeviceAndSwapChainAndContext());
	_d->CreateDeviceAndSwapChainAndContext(hwnd,&_device, &_context, &_swapchain);//デバイス,スワップチェイン,コンテキストを生成



}

ID3D11Device*& D3D11DeviceAndSwapChainAndContextManager::GetDevice() {
	return _device;
}
ID3D11DeviceContext*& D3D11DeviceAndSwapChainAndContextManager::GetDeviceContext() {
	return _context;
}
IDXGISwapChain*& D3D11DeviceAndSwapChainAndContextManager::GetSwapChain() {
	return _swapchain;
}

