#include "PixelShada.h"
#include <d3d11.h>
#include <d3dCompiler.h>
#include "constance.h"

PixelShada::PixelShada() {

}
PixelShada::~PixelShada() {

}

void PixelShada::CreatePixelShada(const TCHAR* filenamepath, const char* funcname, ID3D11Device*& device, ID3D11PixelShader** pixelbuffer) {
	HRESULT result = E_FAIL;
	ID3DBlob* compiledShader = nullptr;
	ID3DBlob* error = nullptr;

	//ピクセルシェーダー作成
	result = D3DCompileFromFile(
		filenamepath,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcname,
		"ps_5_0",
		D3DCOMPILE_DEBUG,//コンパイルを利用する
		0,
		&compiledShader,
		&error
	);
	if (result != S_OK) {
		if (error != NULL){
			OutputDebugStringA((char*)error->GetBufferPointer());
		}
		throw(1);
	}
	SAFE_RELEASE(error);
	result = device->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, pixelbuffer);
	if(result != S_OK){
		throw(1);
	}

	SAFE_RELEASE(compiledShader);
}

