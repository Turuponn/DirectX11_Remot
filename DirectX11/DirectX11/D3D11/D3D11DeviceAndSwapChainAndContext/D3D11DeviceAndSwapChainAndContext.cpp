#include "D3D11DeviceAndSwapChainAndContext.h"
#include <Windows.h>
#include <stdio.h>
#include <d3d11.h>
#include <dxgi.h>
#include "constance.h"
#include "WinManager.h"





#pragma comment( lib, "d3d11.lib")

D3D11DeviceAndSwapChainAndContext::D3D11DeviceAndSwapChainAndContext() {

}
D3D11DeviceAndSwapChainAndContext::~D3D11DeviceAndSwapChainAndContext() {

}
void D3D11DeviceAndSwapChainAndContext::CreateDeviceAndSwapChainAndContext(WinHWND& hwnd, ID3D11Device** device, ID3D11DeviceContext** context, IDXGISwapChain** swapchain) {
	HRESULT result = E_FAIL;
	D3D_FEATURE_LEVEL fLevel = {};
	FutureLevel(fLevel);

	//デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;//表裏
	sd.BufferDesc.Width = SCREENSIZE_X;
	sd.BufferDesc.Height = SCREENSIZE_Y;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd.hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;//出力がウィンドウ モードの場合は TRUE それ以外の場合は FALSE


	//二度手間だがswapchain単体は作れない？
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, swapchain, device, &fLevel, context);

	if (result != S_OK) {
		throw(1);
	}
}

void D3D11DeviceAndSwapChainAndContext::FutureLevel(D3D_FEATURE_LEVEL& scsessLevel) {
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;

	//使用可能コンテキストのフューチャーレベルを調べる
	D3D_FEATURE_LEVEL levelTable[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};

	//デバイス
	HRESULT result = E_FAIL;
	int counter = 0;//フューチャーレベルがあったかどうか
	//使用可能な最高フューチャーレベルを探す
	for (auto lev : levelTable) {
		//デバイスを作りながらつかえる機能レベルを探す
		result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &device, &lev, &context);
		if (result == S_OK) {
			scsessLevel = lev;
			counter = 1;
			break;
		}
	}

	//デバッグのみフューチャーレベルを見せる
#ifdef _DEBUG
	switch (scsessLevel) {
	case D3D_FEATURE_LEVEL_12_1:
		printf("USE_FEATURE_LEVEL_12_1");
		break;
	case D3D_FEATURE_LEVEL_12_0:
		printf("USE_FEATURE_LEVEL_12_0");
		break;
	case D3D_FEATURE_LEVEL_11_1:
		printf("USE_FEATURE_LEVEL_11_1");
		break;
	case D3D_FEATURE_LEVEL_11_0:
		printf("USE_FEATURE_LEVEL_11_0");
		break;
	case D3D_FEATURE_LEVEL_10_0:
		printf("D3D_FEATURE_LEVEL_10_0");
		break;
	case D3D_FEATURE_LEVEL_9_3:
		printf("D3D_FEATURE_LEVEL_9_3");
		break;
	default:
		break;
	}
#endif


	//使用可能なフューチャーレベルがなかったら、落とす処理
	if (counter <= 0) {
		throw(1);
	}
}