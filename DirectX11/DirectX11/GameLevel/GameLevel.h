#pragma once

#include "GameEngine\GameEngine.h"

struct WinHWND;

class GameLevel : public GameEngine{
private:
	
public:
	GameLevel();
	virtual ~GameLevel();

	/**
	@brief 初期化
	@param[in] hwnd ウィンドウハンドルを入れる
	*/
	void Init(WinHWND& hwnd);

	///更新
	void Update();

private:
};
