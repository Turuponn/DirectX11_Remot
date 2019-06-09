#include "ConstantManager.h"
#include "D3D11\Shada\Constant\ConstantBufferCreate.h"
#include  "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include <d3d11.h>
#include "constance.h"




ConstantManager::ConstantManager() {
	_constantbuffer = nullptr;
}
ConstantManager::~ConstantManager() {
	SAFE_RELEASE(_constantbuffer);
}

void ConstantManager::CreateConstant(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,const size_t& constantstrctsize) {
	std::shared_ptr<ConstantBufferCreate> cbuff(new ConstantBufferCreate());
	cbuff->CreateConstantBuffer(device->GetDevice(), constantstrctsize, &_constantbuffer);
}

void ConstantManager::ConstantMap(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, D3D11_MAPPED_SUBRESOURCE* address) {
	HRESULT result = E_FAIL;
	result = device->GetDeviceContext()->Map(_constantbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, address);//定数バッファに間接的にアクセスするためにaddressを関連付ける

	if (result != S_OK) {
		throw(1);
	}
}

void ConstantManager::ConstantUnMap(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	device->GetDeviceContext()->Unmap(_constantbuffer, 0);
}
ID3D11Buffer*& ConstantManager::GetCbuff() {
	return _constantbuffer;
}