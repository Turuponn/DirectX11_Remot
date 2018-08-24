#include "VertexBufferCreate.h"
#include <d3d11.h>


VertexBufferCreate::VertexBufferCreate() {

}
VertexBufferCreate::~VertexBufferCreate() {

}

void VertexBufferCreate::CreateVertexBuffer(ID3D11Device*& device, const int& buffersize,void* vertexdata,ID3D11Buffer** vertexbuffer) {
	HRESULT result = E_FAIL;
	
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = buffersize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//頂点バッファとして作成
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertexdata;//生データを入れる(なにか必ずデータが必要。バッファサイズを先に作ることは不可能かと思われる)
	result = device->CreateBuffer(&bd, &InitData, vertexbuffer);
	if (result != S_OK) {
		throw(1);
	}


	//TODO: 頂点はMap,Unmapが必要　後で作る
}
