#include "Plane.h"
#include <d3d11.h>
#include "D3D11Manager\VertexBuffer\VertexBufferManager.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "D3D11Manager\Shada\VS\VertexShadaManager.h"
#include "D3D11Manager\Shada\PS\PixelShadaManager.h"
#include "D3D11Manager\Shada\InputLayout\InputLayoutManager.h"
#include "D3D11Manager\Shada\Constant\ConstantManager.h"
#include "constance.h"
#include "D3D11Manager\Texture\WICTextureLoderManager.h"
#include "D3D11Manager\TextureSampler\TextureSamplerManager.h"




//VS
const TCHAR* shadafilepath_VS = _T("resoce/Shada/Primitiv/VS/PrimitivVS.hlsl");
const char* shadafuncname_VS = "BasicVS";
//PS
const TCHAR* shadafilepath_PS = _T("resoce/Shada/Primitiv/PS/PrimitivPS.hlsl");
const char* shadafuncname_PS = "BasicPS";


//Test texturePath
const TCHAR* texFilepath = _T("resoce/Texture/81y8fpUAp2L._SL1500_.jpg");


////���_���`
//PrimitivVertex vertices[] =
//{
//	DirectX::XMFLOAT3(-5.5f,-5.5f,0),//���_1	
//	DirectX::XMFLOAT3(-5.5f,5.5f,0), //���_2
//	DirectX::XMFLOAT3(5.5f,-5.5f,0.0f),  //���_3
//	DirectX::XMFLOAT3(5.5f,5.5f,0),
//};


//���_���C�A�E�g: TODO:�@���ʂƂ���Ȃ猩����悤�ɍ��ׂ�
D3D11_INPUT_ELEMENT_DESC vertex_layout[] =
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



Plane::Plane() {
}
Plane::~Plane() {

}


void Plane::CreatePlane(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	//�e�N�X�`�������ݒ�
	InitTexture(device);
	//���_�쐬
	CreateVertex(device);
	//�V�F�[�_�쐬
	CreateShada(device);
	//TODO: �e�N�X�`�������[�h����(�}�e���A���Ƃ��Ă��邱�ƂɂȂ�@3D�Ȃ�̘b����)
	LoadTexture(device);
	
}
void Plane::InitTexture(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	std::shared_ptr<WICTextureLoderManager> tex(new WICTextureLoderManager());
	_textureManager = tex;

	std::shared_ptr<TextureSamplerManager> sampler(new TextureSamplerManager());
	sampler->CreateTextureSampler(device);
	_texSamplerManager = sampler;


	
}
void Plane::LoadTexture(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	_textureManager->LoadTexture(device, texFilepath);
}

void Plane::CreateVertex(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {

	//���_��` 
	_primitiv_vertexList.push_back(PrimitivVertex(-500.0f, -10.2f, 500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f));
	_primitiv_vertexList.push_back(PrimitivVertex(500.0f, -10.2f, 500.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f));
	_primitiv_vertexList.push_back(PrimitivVertex(-500.0f, -10.2f, -500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));
	_primitiv_vertexList.push_back(PrimitivVertex(500.0f, -10.2f, -500.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f));

	std::shared_ptr<VertexBufferManager> vm(new VertexBufferManager());
	_vertexbuffermanager = vm;
	_vertexbuffermanager->CreateVertexBuffer(
		device,
		static_cast<void*>(_primitiv_vertexList.data()),//Note: �O�l�������ɂ�empty�`�F�b�N���K�v��(void*)�@��肢���炵��data�����̃`�F�b�N�����˂Ă����炵��
		sizeof(PrimitivVertex),
		_primitiv_vertexList.size()
	);

}

void Plane::CreateShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	//VS
	std::shared_ptr<VertexShadaManager> vs(new VertexShadaManager());
	vs->CreateVSShada(device, shadafilepath_VS, shadafuncname_VS);
	_vsshada = vs;


	//PS
	std::shared_ptr<PixelShadaManager> ps(new PixelShadaManager());
	ps->CreatePSShada(device, shadafilepath_PS, shadafuncname_PS);
	_psshada = ps;


	//inputlayout
	int num = sizeof(vertex_layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	std::shared_ptr<InputLayoutManager> in(new InputLayoutManager());
	in->InputLaoutCreate(device, vertex_layout, num, _vsshada);
	_inputlayoutManager = in;


}

void Plane::DrawModel(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, std::shared_ptr<ConstantManager>& cameraCManager) {
	

	////�g�p����V�F�[�_�[�̓o�^
	device->GetDeviceContext()->VSSetShader(_vsshada->GetVertexShada(), nullptr, 0);
	device->GetDeviceContext()->PSSetShader(_psshada->GetPSShada(), nullptr, 0);
	//�R���X�^���g�o�b�t�@�[���A�ǂ̃V�F�[�_�[�Ŏg�������w��F�萔���̂��̂��K�vDx12�Ƃ͈Ⴄ
	device->GetDeviceContext()->VSSetConstantBuffers(0, 1, &cameraCManager->GetCbuff());//TODO: �����̃X���b�g�̓��[�g�V�O�l�`�����X�y�N�g�ɂ��悤
	device->GetDeviceContext()->PSSetConstantBuffers(0, 1, &cameraCManager->GetCbuff());
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(PrimitivVertex);
	UINT offset = 0;
	device->GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffermanager->GetVertexBuffer(), &stride, &offset);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	device->GetDeviceContext()->IASetInputLayout(_inputlayoutManager->GetInputLayout());
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	device->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`����GPU��
	device->GetDeviceContext()->PSSetShaderResources(0, 1, &_textureManager->GetTexView());
	device->GetDeviceContext()->PSSetSamplers(0, 1, &_texSamplerManager->GetTextureSampler());
	//�v���~�e�B�u�������_�����O
	device->GetDeviceContext()->Draw(_primitiv_vertexList.size(), 0);
}

std::shared_ptr<TextureSamplerManager>& Plane::GetSmpler() {
	return _texSamplerManager;
}