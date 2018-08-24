#pragma once

/*
Singleton
*/

#include <Windows.h>

struct WinHWND {
	//!window�n���h��
	HWND hwnd;
};


class WinManager {
private:
	WinManager();//�����֎~
	WinManager(const WinManager&);//�R�s�[�R���X�g���N�^�֎~
	WinManager& operator=(const WinManager&);//����֎~
private:
	//���Ԑ���
	static void create();
	//�C���X�^���X����
	static WinManager* Instance();

private:
	//�E�B���h�E�n������
	void WindowInit(const TCHAR* classname, const unsigned int& screensizeX, const unsigned int& screensizeY);

public:
	virtual ~WinManager();
	//���ԏ���
	static void destroy();
	//������
	static void Initialize(const TCHAR* classname, const unsigned int& screensizeX, const unsigned int& screensizeY);
	//���C�����[�v
	static void Run();

	

private:
	static WinManager* mInstance;//����

	WinHWND _hwnd;
	
	RECT _windowrect;//�E�B���h�E��`


	
};
