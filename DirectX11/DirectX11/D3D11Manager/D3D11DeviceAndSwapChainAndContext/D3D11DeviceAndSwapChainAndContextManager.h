#pragma once


#include <memory>




struct ID3D11DeviceContext;
struct ID3D11Device;
struct IDXGISwapChain;
struct WinHWND;

class D3D11DeviceAndSwapChainAndContextManager {
public:
	D3D11DeviceAndSwapChainAndContextManager();
	virtual ~D3D11DeviceAndSwapChainAndContextManager();

	/**
	 @brief �f�o�C�X�𐶐�����
	 @param[in] hwnd �E�B���h�E�n���h��������
	*/
	void CreateDevice(WinHWND& hwnd);





	//�A�N�Z�b�T===============================

	/**
	@brief �f�o�C�X��Ԃ�
	*/
	ID3D11Device*& GetDevice();
	/**
	@brief �f�o�C�X�R���e�L�X�g��Ԃ�
	*/
	ID3D11DeviceContext*& GetDeviceContext();
	/**
	@brief �X���b�v�`�F�C����Ԃ�
	*/
	IDXGISwapChain*& GetSwapChain();

private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _context;
	IDXGISwapChain* _swapchain;
};
