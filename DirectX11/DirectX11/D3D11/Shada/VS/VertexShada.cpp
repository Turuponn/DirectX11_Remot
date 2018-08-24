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
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//includeしてたら必要
		funcname,
		"vs_5_0",
		D3DCOMPILE_DEBUG,//コンパイルを利用する
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
	//頂点レイアウトでコンパイルシェーダはつかう
	
}