#pragma once


#include <memory>

struct ID3D11RasterizerState;
class D3D11DeviceAndSwapChainAndContextManager;

class RasterizerManager {
public:
	RasterizerManager();
	virtual ~RasterizerManager();

	/**
	@brief ラスタライザ初期化
	*/
	void RasterizerInit(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

	//
	ID3D11RasterizerState*& GetRasterizerState();
private:
	ID3D11RasterizerState* _rasterizer;
};
