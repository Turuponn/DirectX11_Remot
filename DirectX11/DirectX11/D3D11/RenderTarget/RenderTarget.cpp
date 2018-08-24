#include "RenderTarget.h"
#include <d3d11.h>




RenderTarget::RenderTarget() {

}
RenderTarget::~RenderTarget() {

}

void RenderTarget::CreateRenderTarget(IDXGISwapChain*& swapchain,ID3D11Device*& device, ID3D11RenderTargetView** rendertargetview) {
	ID3D11Texture2D* pBackBuffer = nullptr;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);//swapchain����o�b�N�o�b�t�@�ւ̃|�C���^���擾
	
	//�����_�[�^�[�Q�b�g�쐬
	device->CreateRenderTargetView(pBackBuffer, nullptr, rendertargetview);

}
