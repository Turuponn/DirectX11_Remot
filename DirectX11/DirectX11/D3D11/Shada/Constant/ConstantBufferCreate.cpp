#include "ConstantBufferCreate.h"
#include <d3d11.h>



ConstantBufferCreate::ConstantBufferCreate() {

}
ConstantBufferCreate::~ConstantBufferCreate() {

}


void ConstantBufferCreate::CreateConstantBuffer(ID3D11Device*& device,const size_t& constantsize,ID3D11Buffer** constantbuffer) {
	HRESULT result = E_FAIL;
	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb = {};
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//バッファは定数として
	cb.ByteWidth = constantsize;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//動的変更に対応
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	result = device->CreateBuffer(&cb, nullptr,constantbuffer);
	
	if (result != S_OK) {
		throw(1);
	}
}