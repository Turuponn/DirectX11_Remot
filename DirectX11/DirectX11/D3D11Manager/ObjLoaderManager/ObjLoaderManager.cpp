#include "ObjLoaderManager.h"
#include <d3d11.h>
#include "Loader\ObjLoader.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Texture\WICTextureLoderManager.h"

#include "D3D11Manager\Shada\InputLayout\InputLayoutManager.h"
#include "D3D11Manager\Shada\VS\VertexShadaManager.h"
#include "D3D11Manager\Shada\PS\PixelShadaManager.h"
#include "D3D11Manager\VertexBuffer\VertexBufferManager.h"
#include "D3D11Manager\TextureSampler\TextureSamplerManager.h"
#include "D3D11Manager\IndexBuffer\IndexBufferManager.h"

namespace OBJShadaNS {
	const TCHAR* VSfilepath = _T("resoce/Shada/Obj/VS/ObjVertexShader.hlsl");
	const char* vsfuncname = "ObjVS";
	const TCHAR* PSfilepath = _T("resoce/Shada/Obj/PS/ObjPixelShader.hlsl");
	const char* psfuncname = "ObjPS";
}


D3D11_INPUT_ELEMENT_DESC obj_vertexlayout[] =
{
	{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,//���[ A���܂߂�Ƃ������Ȃ�
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,//���_�P�ʂƂ���
		0
	},
	{
		"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,//�ǉ�
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,//�ǉ�
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"COLOR",
		0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,//�ǉ�
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
};

ObjLoaderManager::ObjLoaderManager() {
	_materialnum = 0;
	 _mesh = nullptr;
	_material = nullptr;
	_vertexBuffer = nullptr;
}
ObjLoaderManager::~ObjLoaderManager() {
	
}


void ObjLoaderManager::LoadModel(const char* filepath, std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {

	//�e�N�X�`��
	std::shared_ptr<WICTextureLoderManager> wictex(new WICTextureLoderManager());
	_wictexloder = wictex;




	//���f�����[�h
	std::shared_ptr<ObjLoader> o(new ObjLoader());
	o->InitStaticMesh(device, filepath, &_mesh);
	_objloader = o;
	
	 


	//�V�F�[�_�n
	CreateShada(device);
	//���_�o�b�t�@
	//CreateVertex(device);
	//���_�C���f�b�N�X
	//CreateIndex(device);
}

void ObjLoaderManager::CreateIndex(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	std::shared_ptr<IndexBufferManager> _indexbuffcreate(new IndexBufferManager());
	//sizeof(int) * dwPartFCount * 3 �E�E�E1size
	int size = 0;
	for (int i = 0; i < _indexdata.size(); i++) {
		size += sizeof(int)*_indexdata[i].fecenum * 3;
	}
	_indexbuffcreate->CreateIndexBuffer(device, size,&_indexdata[0]);
	_indexbuff = _indexbuffcreate;
}

void ObjLoaderManager::CreateShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	

	
	

	//���_�V�F�[�_
	std::shared_ptr<VertexShadaManager> v(new VertexShadaManager());
	v->CreateVSShada(device, OBJShadaNS::VSfilepath, OBJShadaNS::vsfuncname);
	_vs = v;

	//�s�N�Z���V�F�[�_
	std::shared_ptr<PixelShadaManager> p(new PixelShadaManager());
	p->CreatePSShada(device, OBJShadaNS::PSfilepath, OBJShadaNS::psfuncname);
	_ps = p;

	//���_���C�A�E�g
	std::shared_ptr<InputLayoutManager> input(new InputLayoutManager());
	_inputlayoutmanager = input;
	int num = sizeof(obj_vertexlayout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	_inputlayoutmanager->InputLaoutCreate(device, obj_vertexlayout, num, _vs);
}

void ObjLoaderManager::DrawModel(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::shared_ptr<TextureSamplerManager>& smpler) {


	//�g�p����V�F�[�_�[�̓o�^	
	device->GetDeviceContext()->VSSetShader(_vs->GetVertexShada(), nullptr, 0);
	device->GetDeviceContext()->PSSetShader(_ps->GetPSShada(), nullptr, 0);


	//���_�C���v�b�g���C�A�E�g���Z�b�g
	device->GetDeviceContext()->IASetInputLayout(_inputlayoutmanager->GetInputLayout());
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	device->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	_objloader->Render(device, _mesh);

}
void ObjLoaderManager::CreateVertex(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	HRESULT result = E_FAIL;
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(OBJ_VERTEX) * (_mesh->faceindexnum * 3);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &_vertexdata[0];
	result = device->GetDevice()->CreateBuffer(&bd, &InitData, &_vertexBuffer);
	if (result != S_OK) {
		throw(1);
	}
}