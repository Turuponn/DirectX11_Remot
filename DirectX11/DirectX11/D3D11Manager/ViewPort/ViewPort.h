#pragma once

#include <memory>
class D3D11DeviceAndSwapChainAndContextManager;

class ViewPort {
public:
	ViewPort();
	virtual ~ViewPort();

	/**
	@brief �r���[�|�[�g��ݒ�
	*/
	void ViewPortInit(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

private:
};
