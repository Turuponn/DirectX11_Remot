#include "InputLayoutManager.h"
#include <d3d11.h>
#include "D3D11\Shada\InputLayout\CreateInputLayout.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Shada\VS\VertexShadaManager.h"
#include "constance.h"
InputLayoutManager::InputLayoutManager() {
	_inputlayout = nullptr;
}
InputLayoutManager::~InputLayoutManager() {
	SAFE_RELEASE(_inputlayout);
}

void InputLayoutManager::InputLaoutCreate(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device,const D3D11_INPUT_ELEMENT_DESC* inputlaout,const  unsigned int& inputlaoutarrysize, std::shared_ptr<VertexShadaManager>& vertexshadamanager) {
	std::shared_ptr<CreateInputLayout> il(new CreateInputLayout());
	il->Create(device->GetDevice(), inputlaout, inputlaoutarrysize, vertexshadamanager->GetVertexCompileShada(), &_inputlayout);
}
ID3D11InputLayout*& InputLayoutManager::GetInputLayout() {
	return _inputlayout;
}