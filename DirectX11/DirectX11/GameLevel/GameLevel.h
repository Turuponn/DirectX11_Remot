#pragma once

#include "GameEngine\GameEngine.h"

struct WinHWND;

class GameLevel : public GameEngine{
private:
	
public:
	GameLevel();
	virtual ~GameLevel();

	/**
	@brief ������
	@param[in] hwnd �E�B���h�E�n���h��������
	*/
	void Init(WinHWND& hwnd);

	///�X�V
	void Update();

private:
};
