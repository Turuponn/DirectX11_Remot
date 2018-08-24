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

	//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;//�\��
	sd.BufferDesc.Width = SCREENSIZE_X;
	sd.BufferDesc.Height = SCREENSIZE_Y;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd.hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;//�o�͂��E�B���h�E ���[�h�̏ꍇ�� TRUE ����ȊO�̏ꍇ�� FALSE


	//��x��Ԃ���swapchain�P�͍̂��Ȃ��H
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, swapchain, device, &fLevel, context);

	if (result != S_OK) {
		throw(1);
	}
}

void D3D11DeviceAndSwapChainAndContext::FutureLevel(D3D_FEATURE_LEVEL& scsessLevel) {
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;

	//�g�p�\�R���e�L�X�g�̃t���[�`���[���x���𒲂ׂ�
	D3D_FEATURE_LEVEL levelTable[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};

	//�f�o�C�X
	HRESULT result = E_FAIL;
	int counter = 0;//�t���[�`���[���x�������������ǂ���
	//�g�p�\�ȍō��t���[�`���[���x����T��
	for (auto lev : levelTable) {
		//�f�o�C�X�����Ȃ��������@�\���x����T��
		result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &device, &lev, &context);
		if (result == S_OK) {
			scsessLevel = lev;
			counter = 1;
			break;
		}
	}

	//�f�o�b�O�̂݃t���[�`���[���x����������
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


	//�g�p�\�ȃt���[�`���[���x�����Ȃ�������A���Ƃ�����
	if (counter <= 0) {
		throw(1);
	}
}