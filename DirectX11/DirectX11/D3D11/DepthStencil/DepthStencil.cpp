#include "DepthStencil.h"
#include <d3d11.h>
#include "constance.h"



DepthStencil::DepthStencil() {

}
DepthStencil::~DepthStencil() {

}

void DepthStencil::CreateDepthStencil(ID3D11Device*& device, ID3D11DepthStencilView** depthstencilview) {
	HRESULT result = E_FAIL;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = SCREENSIZE_X;
	descDepth.Height = SCREENSIZE_Y;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	//ID3D11Texture2D* depthstencilTex = nullptr;
	result = device->CreateTexture2D(&descDepth, nullptr, &depthstencilTex);
	if (result != S_OK) {
		throw(1);
	}
	result = E_FAIL;


	result = device->CreateDepthStencilView(depthstencilTex, nullptr, depthstencilview);
	if (result != S_OK) {
		throw(1);
	}
	result = E_FAIL;
}
