#pragma once

#include <tchar.h>



//==============================================
//便利マクロ
#define SAFE_DELETE(ptr){ if(ptr) { delete(ptr); (ptr)=nullptr; } }//安全にdelete
#define SAFE_RELEASE(ptr){ if(ptr) { (ptr)->Release(); (ptr)=nullptr; } } //安全なRelease
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=nullptr; } }//安全な配列用delete
//セキュリティエラーを無視する　推奨されないエラー系を無視
#ifdef _MSC_VER
#define DISABLE_C4996   __pragma(warning(push)) __pragma(warning(disable:4996))
#define ENABLE_C4996    __pragma(warning(pop))
#else
#define DISABLE_C4996
#define ENABLE_C4996
#endif

//=================================================================
//windowsize
#define SCREENSIZE_X 1024
#define SCREENSIZE_Y 720

//=================================================================





