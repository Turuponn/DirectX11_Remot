#pragma once


struct WinHWND;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
enum D3D_FEATURE_LEVEL;


class D3D11DeviceAndSwapChainAndContext {
private:
	/**
	* @brief フューチャーレベル調べる
	* @param[out] scsessLevel　コンテキストの機能レベルを返す
	*/
	void FutureLevel(D3D_FEATURE_LEVEL& scsessLevel);


public:
	D3D11DeviceAndSwapChainAndContext();
	virtual ~D3D11DeviceAndSwapChainAndContext();
	
	/**
	* @fn void CreateDevice()
	* @brief デバイスとスワップチェインとコンテキストを作成する
	* @param[in] hwnd
	* @param[out]  device
	* @param[out]  context
	* @param[out]  swapchain
	*/
	void CreateDeviceAndSwapChainAndContext(WinHWND& hwnd, ID3D11Device** device, ID3D11DeviceContext** context, IDXGISwapChain** swapchain);

private:

};
