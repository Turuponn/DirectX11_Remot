#include "RenderTarget.h"
#include <d3d11.h>




RenderTarget::RenderTarget() {

}
RenderTarget::~RenderTarget() {

}

void RenderTarget::CreateRenderTarget(IDXGISwapChain*& swapchain,ID3D11Device*& device, ID3D11RenderTargetView** rendertargetview) {
	ID3D11Texture2D* pBackBuffer = nullptr;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);//swapchainからバックバッファへのポインタを取得
	
	//レンダーターゲット作成
	device->CreateRenderTargetView(pBackBuffer, nullptr, rendertargetview);

}
