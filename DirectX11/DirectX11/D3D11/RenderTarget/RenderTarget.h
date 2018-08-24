#pragma once


struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11RenderTargetView;

class RenderTarget {
public:
	RenderTarget();
	virtual ~RenderTarget();

	/**
	 @brief �����_�[�^�[�Q�b�g���쐬
	 @param[in] swapchain 
	 @param[in] device
	 @param[out] rendertargetview �����_�[�^�[�Q�b�g�r���[��Ԃ�
	*/
	void CreateRenderTarget(IDXGISwapChain*& swapchain, ID3D11Device*& device, ID3D11RenderTargetView** rendertargetview);

private:

};
