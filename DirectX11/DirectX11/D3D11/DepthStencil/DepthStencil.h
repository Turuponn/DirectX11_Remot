#pragma once


struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

class DepthStencil {
public:
	DepthStencil();
	virtual ~DepthStencil();

	
	/**
	@brief �[�x�X�e���V���쐬
	@param[in] device 
	@param[out] depthstencilview �[�x�o�b�t�@�̏ꏊ��Ԃ�
	*/
	void CreateDepthStencil(ID3D11Device*& device, ID3D11DepthStencilView** depthstencilview);

private:
	ID3D11Texture2D* depthstencilTex;

};
