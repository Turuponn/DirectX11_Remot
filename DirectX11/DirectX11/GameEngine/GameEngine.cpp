#include "GameEngine.h"
#include "D3D11Manager\D3D11DeviceAndSwapChainAndContext\D3D11DeviceAndSwapChainAndContextManager.h"
#include "Graphics\Graphics.h"
#include "WinManager.h"
#include "Primitiv\Plane\Plane.h"
#include "Camera\CameraManager.h"
#include "D3D11Manager\ObjLoaderManager\ObjLoaderManager.h"
#include "D3D11Manager\FBX\FBXLoaderManager.h"


#pragma comment(lib,"d3dcompiler.lib")//shada
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"libfbxsdk-mt.lib")


GameEngine::GameEngine() {

}
GameEngine::~GameEngine() {

}
void GameEngine::Init(WinHWND& hwnd) {



	

	//デバイス生成
	std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager> d(new D3D11DeviceAndSwapChainAndContextManager());
	d->CreateDevice(hwnd);
	_device = d;
	
	//renderInit
	std::shared_ptr<Graphics> g(new Graphics());
	g->Init(_device);
	_graphic = g;
	
	
}

void GameEngine::Run() {
	_graphic->Render(_device);

}


void GameEngine::ScreenFilp() {
	_graphic->ScreenFlip(_device);
}

#pragma region Plane
const int& GameEngine::CreatePrimitivPlane() {
	std::shared_ptr<Plane> plane(new Plane());
	int handle = (int)&plane;
	plane->CreatePlane(_device);
	_planehandles.insert(std::make_pair(handle, plane));
	return handle;
}
void GameEngine::UpdatePlane(const int& planehandle, const int& camerahandle) {
	auto plane = _planehandles.at(planehandle);
	auto camera = _camerahandles.at(camerahandle);
	if (plane != nullptr && camera != nullptr) {
		plane->DrawModel(_device, camera->GetConstantManager());
	}
	
}
#pragma endregion


#pragma region Camera
const int& GameEngine::CameraCreate() {
	std::shared_ptr<CameraManager> camera(new CameraManager());
	camera->CreateCamera(_device);
	int handle = (int)&camera;
	_camerahandles.insert(std::make_pair(handle, camera));
	return handle;
}
void GameEngine::UpdateCamera(const int& handle) {
	auto camera = _camerahandles.at(handle);
	if (camera != nullptr) {//TODO: メインカメラとして作りたいが
		camera->Update(_device);
	}
}


#pragma endregion


int GameEngine::LoadTexture(const std::wstring& filepath) {
	//TODO: マテリアル以外の2D用として使いたい
	return 1;
}
#pragma endregion


#pragma region FBX
int GameEngine::LoadFBXModel(const std::string& filepath) {
	std::shared_ptr<FBXLoaderManager> fbx(new FBXLoaderManager());
	fbx->LoadModel(filepath,_device);
	int handle = (int)&fbx;
	_fbxmodels.insert(std::make_pair(handle, fbx));
	return handle;
}
void GameEngine::DrawFBXModel(const int handle) {
	auto fbx = _fbxmodels.at(handle);
	if (fbx != nullptr) {
		fbx->Render(_device);
	}
}

#pragma endregion
