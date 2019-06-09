#include "Graphics.h"
#include "constance.h"
#include <d3d11.h>
#include "D3D11Manager\DepthStencil\DepthStencilManager.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Rasterizer\RasterizerManager.h"
#include "D3D11Manager\ViewPort\ViewPort.h"
#include "D3D11Manager\RenderTarget\RenderTargetManager.h"


float ClearColor[4] = {0,0,0,1 };//クリア色

Graphics::Graphics() {

}
Graphics::~Graphics() {

}



void Graphics::Init(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	
	//深度ステンシルの設定
	std::shared_ptr<DepthStencilManager> depthstencil(new DepthStencilManager());
	depthstencil->CreateDepthStencilView(device);
	_depthstencilManager = depthstencil;

	//ラスタライザの設定
	std::shared_ptr<RasterizerManager> rasterizer(new RasterizerManager());
	rasterizer->RasterizerInit(device);
	_rasterizermanager = rasterizer;

	//ビューポートの設定
	std::shared_ptr<ViewPort> viewport(new ViewPort());
	viewport->ViewPortInit(device);
	_viewportmanager = viewport;

	//レンダーターゲットの初期化
	std::shared_ptr<RenderTargetManager> rendertarget(new RenderTargetManager());
	rendertarget->CreateRenderTarget(device);
	_rendertargetmanager = rendertarget;


	device->GetDeviceContext()->OMSetRenderTargets(1, &rendertarget->GetRenderTargetView(), depthstencil->GetDepthStencilView());

}
void Graphics::Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	device->GetDeviceContext()->ClearRenderTargetView(_rendertargetmanager->GetRenderTargetView(),ClearColor );//描画対象はwindowハンドルの先
	device->GetDeviceContext()->ClearDepthStencilView(_depthstencilManager->GetDepthStencilView(),D3D11_CLEAR_DEPTH,1.0f,0.0f);//深度バッファクリア
	device->GetDeviceContext()->RSSetState(_rasterizermanager->GetRasterizerState());//テスト的に追加







	
}

void Graphics::ScreenFlip(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {

	device->GetSwapChain()->Present(0, 0);
}