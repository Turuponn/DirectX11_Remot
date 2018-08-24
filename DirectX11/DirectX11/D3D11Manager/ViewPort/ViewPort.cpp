#include "ViewPort.h"
#include <d3d11.h>
#include "constance.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"

ViewPort::ViewPort() {

}
ViewPort::~ViewPort() {

}


void ViewPort::ViewPortInit(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	//ƒrƒ…[Îß°Ä‚ÌÝ’è
	D3D11_VIEWPORT vp = {};
	vp.Width = SCREENSIZE_X;
	vp.Height = SCREENSIZE_Y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	device->GetDeviceContext()->RSSetViewports(1,&vp);
}