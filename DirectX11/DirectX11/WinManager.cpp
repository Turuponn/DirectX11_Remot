#include "WinManager.h"
#include "GameLevel\GameLevel.h"
#include <tchar.h>

WinManager* WinManager::mInstance = 0;


LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

WinManager::WinManager() {
	_hwnd.hwnd = nullptr;
	
}
WinManager::~WinManager() {
	
}

void WinManager::create() {

	if (mInstance != nullptr) {
		
	}

	mInstance = new WinManager();
}
void WinManager::destroy() {
	delete mInstance;
	mInstance = 0;
}

WinManager* WinManager::Instance() {
	create();
	return mInstance;
}
void WinManager::Initialize(const TCHAR* classname, const unsigned int& screensizeX, const unsigned int& screensizeY) {
	Instance();
	mInstance->WindowInit(classname,screensizeX,screensizeY);
	ShowWindow(mInstance->_hwnd.hwnd, SW_SHOW);
	UpdateWindow(mInstance->_hwnd.hwnd);

	

}
//メインループ
void WinManager::Run() {
	GameLevel* _gamelevel = new GameLevel();

	_gamelevel->Init(mInstance->_hwnd);

	MSG msg = {};
	while (true) {//基本無限ループ
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			break;
		}

		_gamelevel->Update();

	}

	delete(_gamelevel);
	//メモリリークをチェック-------------
#if defined(_DEBUG) + defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF + _CRTDBG_LEAK_CHECK_DF);
#endif
}













void WinManager::WindowInit(const TCHAR* classname, const unsigned int& screensizeX, const unsigned int& screensizeY) {

	//ウィンドウ系初期化

	//コンソール版限定かと思われうる
	HINSTANCE hInst = GetModuleHandle(nullptr);

	//Windowクラス登録:Windowをまとめた情報らしい
	WNDCLASSEX wc = {};//EX版を使えとのこと MSDNより　もとのやつはドキュメントによると非推奨
	wc.cbSize = sizeof(WNDCLASSEX);//EX
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;//コールバック関数の指定
	wc.lpszClassName = classname;
	wc.hInstance = GetModuleHandle(0);//ハンドルの取得
	auto atom = RegisterClassEx(&wc);//ウインドウ登録する　Ex版


	//ウィンドウサイズを決める
	RECT wrcs = { 0,0,screensizeX ,screensizeY };
	_windowrect = wrcs;
	AdjustWindowRect(&_windowrect, WS_OVERLAPPEDWINDOW, false);//フレーム分を補正 領域を合わせる


	//ハンドル作成												 
	_hwnd.hwnd = CreateWindow(
		wc.lpszClassName,//クラス名とかいうやつの指定らしい・・・タイトルとRegisterに登録したやつと同じやつが必要らしい
		wc.lpszClassName,//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//タイトルバーと境界線があるウインドウ
		CW_USEDEFAULT,//表示X座標はOSに任せる命令
		CW_USEDEFAULT,//表示Y座標はOSに任せる命令
		wrcs.right - wrcs.left,//ウインドウ幅
		wrcs.bottom - wrcs.top,//ウインドウ高さ
		nullptr,//親ウインドウハンドル、ウインドウをいくつでも出すとき親を消したら子を消したいときに使うらしい
		nullptr,//メニューハンドル、メニューを使いたいときらしい
		wc.hInstance,//呼び出しアプリケーションハンドル
		nullptr //追加パラメータ
	);

	if (_hwnd.hwnd == nullptr) {
		throw(1);
	}



}

//コールバック関数　クラスに含めない
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//ここでWM_KEY_DOWNは遅いらしい ドライバーから直接取ってきたほうがいいらしい
	//OSを待つためらしいが
	//0を返すのは終了を意味してるのか messageにでも入るのか？
	switch (msg) {
	case WM_DESTROY://終了リクエスト //Window閉じるリクエストがきたら
		PostQuitMessage(0);//OSに対してアプリ終了を伝えるらしい
		return 0;
		break;
	case WM_CLOSE://Alt+F4対応らしい
		if (MessageBox(hwnd, _T("Really quit?"), _T("My application"), MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}
		// Else: ユーザーがキャンセルし、何もしない
		return 0;
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//デフォルトの処理とかいうらしい
}
