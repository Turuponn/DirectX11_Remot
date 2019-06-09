#pragma once



struct ID3D11Device;
struct ID3D11Buffer;

class ConstantBufferCreate {
public:
	ConstantBufferCreate();
	virtual ~ConstantBufferCreate();

	/**
	@brief 定数バッファ作成
	@param constantsize　定数サイズ
	@param [out] constantbuffer 定数バッファを返します
	*/
	void CreateConstantBuffer(ID3D11Device*& device, const size_t& constantsize, ID3D11Buffer** constantbuffer);


};
