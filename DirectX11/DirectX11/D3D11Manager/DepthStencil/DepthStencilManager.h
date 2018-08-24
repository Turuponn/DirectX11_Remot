#pragma once


#include <memory>

class D3D11DeviceAndSwapChainAndContextManager;

struct ID3D11DepthStencilView;


class DepthStencilManager {
public:
	DepthStencilManager();
	virtual ~DepthStencilManager();


	/**
	@brief �[�x�X�e���V���쐬
	@param[in] device
	@param[out] depthstencilview �[�x�o�b�t�@�̏ꏊ��Ԃ�
	*/
	void CreateDepthStencilView(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device);

	/**
	@brief �[�x�X�e���V���r���[��Ԃ�
	*/
	ID3D11DepthStencilView*& GetDepthStencilView();
private:
	
	ID3D11DepthStencilView* _depthStencilView;
};