#include "WinManager.h"
#include "constance.h"


//�A�v���̖��O
const TCHAR* CLASSNAME = _T("DirectX11");




INT main() {

	WinManager::Initialize(CLASSNAME,SCREENSIZE_X, SCREENSIZE_Y);
	WinManager::Run();

	WinManager::destroy();

	return 0;
}