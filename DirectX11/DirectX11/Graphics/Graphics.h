#pragma once



#include <memory>

class DepthStencilManager;
class D3D11DeviceAndSwapChainAndContextManager;
class RasterizerManager;
class ViewPort;
class RenderTargetManager;

class Graphics {
public:
	Graphics();
	virtual ~Graphics();

	/**
	@brief ������
	*/
	void Init(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

	/**
	@brief �`��v���Z�X
	*/
	void Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

	/**
	@brief �`��X�V
	*/
	void ScreenFlip(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

private:
	std::shared_ptr<DepthStencilManager> _depthstencilManager;
	std::shared_ptr<RasterizerManager> _rasterizermanager;
	std::shared_ptr<ViewPort> _viewportmanager;
	std::shared_ptr<RenderTargetManager> _rendertargetmanager;
};
