#include "RasterizerManager.h"
#include <d3d11.h>
#include "constance.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
RasterizerManager::RasterizerManager() {
	_rasterizer = nullptr;
}
RasterizerManager::~RasterizerManager() {
	SAFE_RELEASE(_rasterizer);
}

void RasterizerManager::RasterizerInit(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	HRESULT result = E_FAIL;
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_NONE;//ƒJƒŠƒ“ƒO‚Ì‚±‚Æ‚¾‚ÆŽv‚¤
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	rdc.DepthClipEnable = TRUE;

	result = device->GetDevice()->CreateRasterizerState(&rdc, &_rasterizer);
	if (result != S_OK) {
		throw(1);
	}
	device->GetDeviceContext()->RSSetState(_rasterizer);
	//SAFE_RELEASE(_rasterizer);
}
ID3D11RasterizerState*& RasterizerManager::GetRasterizerState() {
	return _rasterizer;
}