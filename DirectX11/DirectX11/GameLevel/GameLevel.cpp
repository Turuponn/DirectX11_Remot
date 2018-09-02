#include "GameLevel.h"
#include "WinManager.h"

int planehandle = 0;
int mainCameraHandle = 0;
int tree_handle = 0;
int modelhandle = 0;

const std::string filepath_fbx = "resoce/Cube/Test3Cube.fbx";

GameLevel::GameLevel() {
}
GameLevel::~GameLevel() {

}

void GameLevel::Update() {
	GameEngine::Run();

	//update�n��Unity�ł����Ƃ��̃n�C�h�̃C���[�W
	GameEngine::UpdateCamera(mainCameraHandle);
	//���`��
	GameEngine::UpdatePlane(planehandle, mainCameraHandle);
	

	GameEngine::DrawFBXModel(modelhandle);

	GameEngine::ScreenFilp();
}
void GameLevel::Init(WinHWND& hwnd) {
	GameEngine::Init(hwnd);

	
	//�J�����쐬
	mainCameraHandle = GameEngine::CameraCreate();

	//���쐬
	planehandle = GameEngine::CreatePrimitivPlane();

	modelhandle = GameEngine::LoadFBXModel(filepath_fbx);
}