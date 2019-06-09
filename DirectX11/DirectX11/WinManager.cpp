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
//���C�����[�v
void WinManager::Run() {
	GameLevel* _gamelevel = new GameLevel();

	_gamelevel->Init(mInstance->_hwnd);

	MSG msg = {};
	while (true) {//��{�������[�v
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
	//���������[�N���`�F�b�N-------------
#if defined(_DEBUG) + defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF + _CRTDBG_LEAK_CHECK_DF);
#endif
}













void WinManager::WindowInit(const TCHAR* classname, const unsigned int& screensizeX, const unsigned int& screensizeY) {

	//�E�B���h�E�n������

	//�R���\�[���Ō��肩�Ǝv��ꂤ��
	HINSTANCE hInst = GetModuleHandle(nullptr);

	//Window�N���X�o�^:Window���܂Ƃ߂����炵��
	WNDCLASSEX wc = {};//EX�ł��g���Ƃ̂��� MSDN���@���Ƃ̂�̓h�L�������g�ɂ��Ɣ񐄏�
	wc.cbSize = sizeof(WNDCLASSEX);//EX
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;//�R�[���o�b�N�֐��̎w��
	wc.lpszClassName = classname;
	wc.hInstance = GetModuleHandle(0);//�n���h���̎擾
	auto atom = RegisterClassEx(&wc);//�E�C���h�E�o�^����@Ex��


	//�E�B���h�E�T�C�Y�����߂�
	RECT wrcs = { 0,0,screensizeX ,screensizeY };
	_windowrect = wrcs;
	AdjustWindowRect(&_windowrect, WS_OVERLAPPEDWINDOW, false);//�t���[������␳ �̈�����킹��


	//�n���h���쐬												 
	_hwnd.hwnd = CreateWindow(
		wc.lpszClassName,//�N���X���Ƃ�������̎w��炵���E�E�E�^�C�g����Register�ɓo�^������Ɠ�������K�v�炵��
		wc.lpszClassName,//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,//�^�C�g���o�[�Ƌ��E��������E�C���h�E
		CW_USEDEFAULT,//�\��X���W��OS�ɔC���閽��
		CW_USEDEFAULT,//�\��Y���W��OS�ɔC���閽��
		wrcs.right - wrcs.left,//�E�C���h�E��
		wrcs.bottom - wrcs.top,//�E�C���h�E����
		nullptr,//�e�E�C���h�E�n���h���A�E�C���h�E�������ł��o���Ƃ��e����������q�����������Ƃ��Ɏg���炵��
		nullptr,//���j���[�n���h���A���j���[���g�������Ƃ��炵��
		wc.hInstance,//�Ăяo���A�v���P�[�V�����n���h��
		nullptr //�ǉ��p�����[�^
	);

	if (_hwnd.hwnd == nullptr) {
		throw(1);
	}



}

//�R�[���o�b�N�֐��@�N���X�Ɋ܂߂Ȃ�
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//������WM_KEY_DOWN�͒x���炵�� �h���C�o�[���璼�ڎ���Ă����ق��������炵��
	//OS��҂��߂炵����
	//0��Ԃ��̂͏I�����Ӗ����Ă�̂� message�ɂł�����̂��H
	switch (msg) {
	case WM_DESTROY://�I�����N�G�X�g //Window���郊�N�G�X�g��������
		PostQuitMessage(0);//OS�ɑ΂��ăA�v���I����`����炵��
		return 0;
		break;
	case WM_CLOSE://Alt+F4�Ή��炵��
		if (MessageBox(hwnd, _T("Really quit?"), _T("My application"), MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}
		// Else: ���[�U�[���L�����Z�����A�������Ȃ�
		return 0;
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//�f�t�H���g�̏����Ƃ������炵��
}
