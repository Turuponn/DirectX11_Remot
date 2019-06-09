#include "GameLevel.h"
#include "WinManager.h"

int planehandle = 0;
int mainCameraHandle = 0;
int tree_handle = 0;
int modelhandle = 0;

const std::string filepath_fbx = "resoce/Pencil/Pencel3.fbx";

GameLevel::GameLevel() {
}
GameLevel::~GameLevel() {

}

void GameLevel::Update() {
	GameEngine::Run();
	//�J�����̍X�V�����܂�
	GameEngine::UpdateCamera(mainCameraHandle);
	//����`�悵�܂�
	GameEngine::UpdatePlane(planehandle, mainCameraHandle);
	//���f����\�����܂�
	GameEngine::DrawFBXModel(modelhandle);
	//�o�b�N�o�b�t�@�[�ƃt�����g�o�b�t�@�[���X�V���܂�
	GameEngine::ScreenFilp();
}
void GameLevel::Init(WinHWND& hwnd) {
	GameEngine::Init(hwnd);
	//�J�����쐬
	mainCameraHandle = GameEngine::CameraCreate();
	//���쐬
	planehandle = GameEngine::CreatePrimitivPlane();
	//���f�������[�h���܂�
	modelhandle = GameEngine::LoadFBXModel(filepath_fbx);
}