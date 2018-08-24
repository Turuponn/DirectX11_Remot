#include "WICTextureLoaderCreate.h"
#include <d3d11.h>
#include "MicrosoftLiblary\WICTextureLoader\WICTextureLoader.h"
#include "MicrosoftLiblary\WICTextureLoader\DirectXTex.h"
#include "constance.h"



WICTextureLoaderCreate::WICTextureLoaderCreate() {
}
WICTextureLoaderCreate::~WICTextureLoaderCreate() {
}
void WICTextureLoaderCreate::LoadTexture(ID3D11Device*& device,ID3D11DeviceContext*& comand,const std::wstring& filepath, ID3D11Resource** texbuffer, ID3D11ShaderResourceView** texsrvview){
	HRESULT result = E_FAIL;

	result = DirectX::CreateWICTextureFromFile(device, comand, filepath.c_str(), texbuffer, texsrvview);//Žg‚¦‚é
	if (result != S_OK) {
		throw(1);
	}
}
