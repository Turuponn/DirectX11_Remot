#include "CameraManager.h"
#include <time.h>
#include "constance.h"
#include "D3D11Manager\Shada\Constant\ConstantManager.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include <d3d11.h>
#include <stdio.h>


CameraManager::CameraManager() {

}
CameraManager::~CameraManager() {
	SAFE_DELETE(_address);
}


void CameraManager::CreateCamera(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {

	//�s��쐬
	_eye = { 0.0,10.8f,-10.8f };//���_�x�N�g���@����n
	DirectX::XMVECTOR target = { 0,0,0 };//�^�[�Q�b�g:�œ_
	DirectX::XMVECTOR upper_vec = { 0, 1, 0 };//�A�b�p�[�x�N�g��
	_camera = DirectX::XMMatrixLookAtLH(_eye, target, upper_vec);
	_projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, static_cast<float>(SCREENSIZE_X) / static_cast<float>(SCREENSIZE_Y), 0.1f, 5000.0f);
	_viewProj = _camera*_projection;
	CreateConstant(device);
}

void CameraManager::CreateConstant(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	std::shared_ptr<ConstantManager> cbuff(new ConstantManager());
	_constant = cbuff;
	_constant->CreateConstant(device, sizeof(SCENE_MATRIX));
	_address = new D3D11_MAPPED_SUBRESOURCE();
	_constant->ConstantMap(device, _address);
	_scenecameramat.viewproje = _viewProj;
	_scenecameramat.world = DirectX::XMMatrixRotationY(DirectX::XM_PIDIV4);
	//�o�b�t�@�ɏ�������
	memcpy_s(
		_address->pData,
		_address->RowPitch,
		(void*)(&_scenecameramat),//�f�[�^��
		sizeof(_scenecameramat)//�T�C�Y��
	);
	_constant->ConstantUnMap(device);
}
float counter = 0.0f;
void CameraManager::Update(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device) {
	_constant->ConstantMap(device, _address);
	SCENE_MATRIX newmat = {};
	counter = counter + 0.0001f;
	newmat.world = DirectX::XMMatrixRotationY(counter);
	newmat.viewproje = _scenecameramat.viewproje;
	*(SCENE_MATRIX*)_address->pData = newmat;
	_constant->ConstantUnMap(device);
}


DirectX::XMMATRIX& CameraManager::GetViewProjMat() {
	return _viewProj;
}
std::shared_ptr<ConstantManager>& CameraManager::GetConstantManager() {
	return _constant;
}

D3D11_MAPPED_SUBRESOURCE*& CameraManager::GetAddress() {
	return _address;
}