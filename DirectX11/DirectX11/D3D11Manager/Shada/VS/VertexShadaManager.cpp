#include "VertexShadaManager.h"
#include "D3D11\Shada\VS\VertexShada.h"
#include <d3d11.h>
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "constance.h"


VertexShadaManager::VertexShadaManager() {
	_vertexbuff = nullptr;
	_vertexCompileShada = nullptr;
}
VertexShadaManager::~VertexShadaManager() {
	SAFE_RELEASE(_vertexbuff);
	SAFE_RELEASE(_vertexCompileShada);
}

void VertexShadaManager::CreateVSShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,const TCHAR* filepath,const char* funcname) {
	std::shared_ptr<VertexShada> v(new VertexShada());
	v->CreateVertexShada(filepath, funcname, device->GetDevice(), &_vertexbuff, &_vertexCompileShada);
	
}

ID3DBlob*& VertexShadaManager::GetVertexCompileShada() {
	return _vertexCompileShada;
}

ID3D11VertexShader*& VertexShadaManager::GetVertexShada() {
	return _vertexbuff;
}