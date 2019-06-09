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
	 @brief デバイスを生成する
	 @param[in] hwnd ウィンドウハンドルを入れる
	*/
	void CreateDevice(WinHWND& hwnd);





	//アクセッサ===============================

	/**
	@brief デバイスを返す
	*/
	ID3D11Device*& GetDevice();
	/**
	@brief デバイスコンテキストを返す
	*/
	ID3D11DeviceContext*& GetDeviceContext();
	/**
	@brief スワップチェインを返す
	*/
	IDXGISwapChain*& GetSwapChain();

private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _context;
	IDXGISwapChain* _swapchain;
};
