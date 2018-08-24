#pragma once

/*
Singleton
*/

#include <Windows.h>

struct WinHWND {
	//!windowハンドル
	HWND hwnd;
};


class WinManager {
private:
	WinManager();//生成禁止
	WinManager(const WinManager&);//コピーコンストラクタ禁止
	WinManager& operator=(const WinManager&);//代入禁止
private:
	//実態生成
	static void create();
	//インスタンス生成
	static WinManager* Instance();

private:
	//ウィンドウ系初期化
	void WindowInit(const TCHAR* classname, const unsigned int& screensizeX, const unsigned int& screensizeY);

public:
	virtual ~WinManager();
	//実態消す
	static void destroy();
	//初期化
	static void Initialize(const TCHAR* classname, const unsigned int& screensizeX, const unsigned int& screensizeY);
	//メインループ
	static void Run();

	

private:
	static WinManager* mInstance;//実体

	WinHWND _hwnd;
	
	RECT _windowrect;//ウィンドウ矩形


	
};
