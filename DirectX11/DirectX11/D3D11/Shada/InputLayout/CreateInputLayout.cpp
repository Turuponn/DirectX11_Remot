#include "CreateInputLayout.h"
#include <d3d11.h>

CreateInputLayout::CreateInputLayout() {

}
CreateInputLayout::~CreateInputLayout() {

}
void CreateInputLayout::Create(ID3D11Device*& device,const D3D11_INPUT_ELEMENT_DESC* inputlaout,const unsigned int& numelement, ID3DBlob*& vertex_compileshada, ID3D11InputLayout** vertex_inputlaout) {
	//頂点インプットレイアウトを作成
	HRESULT result = E_FAIL;
	result = device->CreateInputLayout(inputlaout, numelement, vertex_compileshada->GetBufferPointer(), vertex_compileshada->GetBufferSize(), vertex_inputlaout);
	if (result != S_OK) {
		throw(1);
	}
}

