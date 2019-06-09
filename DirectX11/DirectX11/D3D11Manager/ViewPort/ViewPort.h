#pragma once

#include <memory>
class D3D11DeviceAndSwapChainAndContextManager;

class ViewPort {
public:
	ViewPort();
	virtual ~ViewPort();

	/**
	@brief ビューポートを設定
	*/
	void ViewPortInit(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

private:
};
