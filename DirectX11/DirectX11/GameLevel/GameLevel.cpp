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
	//カメラの更新をします
	GameEngine::UpdateCamera(mainCameraHandle);
	//床を描画します
	GameEngine::UpdatePlane(planehandle, mainCameraHandle);
	//モデルを表示します
	GameEngine::DrawFBXModel(modelhandle);
	//バックバッファーとフロントバッファーを更新します
	GameEngine::ScreenFilp();
}
void GameLevel::Init(WinHWND& hwnd) {
	GameEngine::Init(hwnd);
	//カメラ作成
	mainCameraHandle = GameEngine::CameraCreate();
	//床作成
	planehandle = GameEngine::CreatePrimitivPlane();
	//モデルをロードします
	modelhandle = GameEngine::LoadFBXModel(filepath_fbx);
}