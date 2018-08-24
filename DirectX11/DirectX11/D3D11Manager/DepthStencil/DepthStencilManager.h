#pragma once


#include <memory>

class D3D11DeviceAndSwapChainAndContextManager;

struct ID3D11DepthStencilView;


class DepthStencilManager {
public:
	DepthStencilManager();
	virtual ~DepthStencilManager();


	/**
	@brief 深度ステンシル作成
	@param[in] device
	@param[out] depthstencilview 深度バッファの場所を返す
	*/
	void CreateDepthStencilView(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

	/**
	@brief 深度ステンシルビューを返す
	*/
	ID3D11DepthStencilView*& GetDepthStencilView();
private:
	
	ID3D11DepthStencilView* _depthStencilView;
};