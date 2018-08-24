#pragma once


#include <memory>

class RenderTarget;
class D3D11DeviceAndSwapChainAndContextManager;
struct ID3D11RenderTargetView;

class RenderTargetManager {
public:
	RenderTargetManager();
	virtual ~RenderTargetManager();

	/**
	@brief �����_�[�^�[�Q�b�g���쐬
	@param[in] device
	*/
	void CreateRenderTarget(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

	/**
	@brief �����_�[�^�[�Q�b�g��Ԃ�
	*/
	ID3D11RenderTargetView*& GetRenderTargetView();


private:

	std::shared_ptr<RenderTarget> _rendertarget;
	ID3D11RenderTargetView* _rendertargetview;
};
