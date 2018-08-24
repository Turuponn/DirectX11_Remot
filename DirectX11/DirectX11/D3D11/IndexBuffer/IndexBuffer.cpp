#include "IndexBuffer.h"
#include <d3d11.h>


IndexBuffer::IndexBuffer() {

}
IndexBuffer::~IndexBuffer() {

}

void IndexBuffer::CreateIndexBuffer(ID3D11Device*& device,const unsigned int buffsize,void* fecedata, ID3D11Buffer** indexbuffer) {
	//インデックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = buffsize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = fecedata;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	device->CreateBuffer(&bd, &InitData, indexbuffer);
}
