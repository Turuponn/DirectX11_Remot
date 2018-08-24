#include "VertexBufferManager.h"
#include <d3d11.h>
#include "D3D11\VertexBuffer\VertexBufferCreate.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "constance.h"

VertexBufferManager::VertexBufferManager() {
	_vertexbuffer = nullptr;
}
VertexBufferManager::~VertexBufferManager() {
	SAFE_RELEASE(_vertexbuffer);
}
void VertexBufferManager::CreateVertexBuffer(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,void* vertexdata,const size_t& onevertexsize,const int& vertexarrysize) {
	std::shared_ptr<VertexBufferCreate> v(new VertexBufferCreate());
	v->CreateVertexBuffer(device->GetDevice(), (int)(onevertexsize* vertexarrysize), vertexdata,&_vertexbuffer);
}

ID3D11Buffer*& VertexBufferManager::GetVertexBuffer() {
	return _vertexbuffer;
}
