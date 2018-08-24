#pragma once


struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

class DepthStencil {
public:
	DepthStencil();
	virtual ~DepthStencil();

	
	/**
	@brief 深度ステンシル作成
	@param[in] device 
	@param[out] depthstencilview 深度バッファの場所を返す
	*/
	void CreateDepthStencil(ID3D11Device*& device, ID3D11DepthStencilView** depthstencilview);

private:
	ID3D11Texture2D* depthstencilTex;

};
