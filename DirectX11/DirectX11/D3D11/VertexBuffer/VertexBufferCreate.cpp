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
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���_�o�b�t�@�Ƃ��č쐬
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertexdata;//���f�[�^������(�Ȃɂ��K���f�[�^���K�v�B�o�b�t�@�T�C�Y���ɍ�邱�Ƃ͕s�\���Ǝv����)
	result = device->CreateBuffer(&bd, &InitData, vertexbuffer);
	if (result != S_OK) {
		throw(1);
	}


	//TODO: ���_��Map,Unmap���K�v�@��ō��
}
