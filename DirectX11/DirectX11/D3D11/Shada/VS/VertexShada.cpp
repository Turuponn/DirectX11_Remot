#include "VertexShada.h"
#include <d3d11.h>
#include <d3dCompiler.h>
#include "constance.h"


VertexShada::VertexShada() {

}
VertexShada::~VertexShada() {

}


void VertexShada::CreateVertexShada(const TCHAR* filenamepath, const char* funcname,ID3D11Device*& device,ID3D11VertexShader** vertexbuffer, ID3DBlob** compileshada) {
	HRESULT result = E_FAIL;
	ID3DBlob* error = nullptr;
	result = D3DCompileFromFile(
		filenamepath,
		nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//include���Ă���K�v
		funcname,
		"vs_5_0",
		D3DCOMPILE_DEBUG,//�R���p�C���𗘗p����
		0,
		compileshada,
		&error
	);
	if (result != S_OK) {
		if (error != NULL) {
			OutputDebugStringA((char*)error->GetBufferPointer());
		}
		throw(1);
	}
	SAFE_RELEASE(error);

	result = device->CreateVertexShader((*compileshada)->GetBufferPointer(), (*compileshada)->GetBufferSize(), nullptr, vertexbuffer);
	
	if (result != S_OK) {
		if (error != NULL) {
			OutputDebugStringA((char*)error->GetBufferPointer());
		}
		SAFE_RELEASE((*compileshada));
		throw(1);
	}
	//���_���C�A�E�g�ŃR���p�C���V�F�[�_�͂���
	
}