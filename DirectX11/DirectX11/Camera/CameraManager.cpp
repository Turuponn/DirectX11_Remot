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

	//行列作成
	_eye = { 0.0,10.8f,-10.8f };//視点ベクトル　左手系
	DirectX::XMVECTOR target = { 0,0,0 };//ターゲット:焦点
	DirectX::XMVECTOR upper_vec = { 0, 1, 0 };//アッパーベクトル
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
	//バッファに書き込み
	memcpy_s(
		_address->pData,
		_address->RowPitch,
		(void*)(&_scenecameramat),//データ部
		sizeof(_scenecameramat)//サイズ部
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