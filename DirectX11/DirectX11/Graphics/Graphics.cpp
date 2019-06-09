#include "Graphics.h"
#include "constance.h"
#include <d3d11.h>
#include "D3D11Manager\DepthStencil\DepthStencilManager.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Rasterizer\RasterizerManager.h"
#include "D3D11Manager\ViewPort\ViewPort.h"
#include "D3D11Manager\RenderTarget\RenderTargetManager.h"


float ClearColor[4] = {0,0,0,1 };//�N���A�F

Graphics::Graphics() {

}
Graphics::~Graphics() {

}



void Graphics::Init(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	
	//�[�x�X�e���V���̐ݒ�
	std::shared_ptr<DepthStencilManager> depthstencil(new DepthStencilManager());
	depthstencil->CreateDepthStencilView(device);
	_depthstencilManager = depthstencil;

	//���X�^���C�U�̐ݒ�
	std::shared_ptr<RasterizerManager> rasterizer(new RasterizerManager());
	rasterizer->RasterizerInit(device);
	_rasterizermanager = rasterizer;

	//�r���[�|�[�g�̐ݒ�
	std::shared_ptr<ViewPort> viewport(new ViewPort());
	viewport->ViewPortInit(device);
	_viewportmanager = viewport;

	//�����_�[�^�[�Q�b�g�̏�����
	std::shared_ptr<RenderTargetManager> rendertarget(new RenderTargetManager());
	rendertarget->CreateRenderTarget(device);
	_rendertargetmanager = rendertarget;


	device->GetDeviceContext()->OMSetRenderTargets(1, &rendertarget->GetRenderTargetView(), depthstencil->GetDepthStencilView());

}
void Graphics::Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	device->GetDeviceContext()->ClearRenderTargetView(_rendertargetmanager->GetRenderTargetView(),ClearColor );//�`��Ώۂ�window�n���h���̐�
	device->GetDeviceContext()->ClearDepthStencilView(_depthstencilManager->GetDepthStencilView(),D3D11_CLEAR_DEPTH,1.0f,0.0f);//�[�x�o�b�t�@�N���A
	device->GetDeviceContext()->RSSetState(_rasterizermanager->GetRasterizerState());//�e�X�g�I�ɒǉ�







	
}

void Graphics::ScreenFlip(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {

	device->GetSwapChain()->Present(0, 0);
}