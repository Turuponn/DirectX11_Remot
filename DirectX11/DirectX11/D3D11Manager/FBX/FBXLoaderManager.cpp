#include "FBXLoaderManager.h"
#include <d3d11.h>
#include "Loader\FBX\FBXLoader.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Shada\InputLayout\InputLayoutManager.h"
#include "D3D11Manager\Shada\VS\VertexShadaManager.h"
#include "D3D11Manager\Shada\PS\PixelShadaManager.h"
#include "D3D11Manager\TextureSampler\TextureSamplerManager.h"


namespace FBXShadaNS {
	const TCHAR* VSfilepath = _T("resoce/Shada/FBX/VS/FBXVertexShader.hlsl");
	const char* vsfuncname = "FbxVS";
	const TCHAR* PSfilepath = _T("resoce/Shada/FBX/PS/FBXPixelShader.hlsl");
	const char* psfuncname = "FbxPS";
}

#pragma region InputLaout
D3D11_INPUT_ELEMENT_DESC fbx_vertexlayout[] =
{
	{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"COLOR",
		0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
};

#pragma endregion

FBXLoaderManager::FBXLoaderManager() {

}
FBXLoaderManager::~FBXLoaderManager() {

}


void FBXLoaderManager::LoadModel(const std::string& filepath,std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	std::shared_ptr<FBXLoader> fbx(new FBXLoader());
	if (fbx->Load(filepath,device) != true) {
		throw(1);
	}
	_fbx = fbx;

	CreateShada(device);
	CreateTextureSampler(device);
}


void FBXLoaderManager::CreateTextureSampler(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	std::shared_ptr<TextureSamplerManager> texsmpler(new TextureSamplerManager());
	texsmpler->CreateTextureSampler(device);
	_texsmpler = texsmpler;
}
void FBXLoaderManager::CreateShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {


	//頂点シェーダ
	std::shared_ptr<VertexShadaManager> v(new VertexShadaManager());
	v->CreateVSShada(device, FBXShadaNS::VSfilepath, FBXShadaNS::vsfuncname);
	_vertexshada = v;

	//ピクセルシェーダ
	std::shared_ptr<PixelShadaManager> p(new PixelShadaManager());
	p->CreatePSShada(device, FBXShadaNS::PSfilepath, FBXShadaNS::psfuncname);
	_pixelshada = p;

	//頂点レイアウト
	std::shared_ptr<InputLayoutManager> input(new InputLayoutManager());
	_inputlaout = input;
	_inputlaout->InputLaoutCreate(device, fbx_vertexlayout, _countof(fbx_vertexlayout), _vertexshada);


	
}
void FBXLoaderManager::Render(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {

	//使用するシェーダーの登録	
	device->GetDeviceContext()->VSSetShader(_vertexshada->GetVertexShada(), nullptr, 0);
	device->GetDeviceContext()->PSSetShader(_pixelshada->GetPSShada(), nullptr, 0);


	//頂点インプットレイアウトをセット
	device->GetDeviceContext()->IASetInputLayout(_inputlaout->GetInputLayout());
	
	_fbx->Render(device, _texsmpler);
}