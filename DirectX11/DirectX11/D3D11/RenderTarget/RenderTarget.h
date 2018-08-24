#pragma once


struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11RenderTargetView;

class RenderTarget {
public:
	RenderTarget();
	virtual ~RenderTarget();

	/**
	 @brief レンダーターゲットを作成
	 @param[in] swapchain 
	 @param[in] device
	 @param[out] rendertargetview レンダーターゲットビューを返す
	*/
	void CreateRenderTarget(IDXGISwapChain*& swapchain, ID3D11Device*& device, ID3D11RenderTargetView** rendertargetview);

private:

};
