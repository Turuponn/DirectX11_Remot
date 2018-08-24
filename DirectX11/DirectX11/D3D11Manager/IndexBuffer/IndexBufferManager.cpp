#include "IndexBufferManager.h"
#include "D3D11\IndexBuffer\IndexBuffer.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include <d3d11.h>
#include "constance.h"

IndexBufferManager::IndexBufferManager() {
	_indexbuffer = nullptr;
}
IndexBufferManager::~IndexBufferManager() {
	SAFE_RELEASE(_indexbuffer);
}
void IndexBufferManager::CreateIndexBuffer(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,const unsigned int buffsize,void* fecedata) {
	std::shared_ptr<IndexBuffer> index(new IndexBuffer());
	index->CreateIndexBuffer(device->GetDevice(), buffsize, fecedata, &_indexbuffer);
}

ID3D11Buffer*& IndexBufferManager::GetIndexBuffer() {
	return _indexbuffer;
}