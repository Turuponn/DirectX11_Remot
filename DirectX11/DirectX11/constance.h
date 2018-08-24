#pragma once

#include <tchar.h>

//// winnt.h�ŕK�v(���i��windows.h����`���Ă���)
//#include <windef.h>     // ��{�I�Ȓ�`��
//#include <stdarg.h>     // winbase.h�ɕK�v
//#include <winbase.h>    // ���̊e��API�����w�b�_�ɕK�v����
//#include <winnt.h>      // windef, winbase�ő���Ȃ���(����windef��������include���Ă�)


//==============================================
//�֗��}�N��
#define SAFE_DELETE(ptr){ if(ptr) { delete(ptr); (ptr)=nullptr; } }//���S��delete
#define SAFE_RELEASE(ptr){ if(ptr) { (ptr)->Release(); (ptr)=nullptr; } } //���S��Release
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=nullptr; } }//���S�Ȕz��pdelete
//�Z�L�����e�B�G���[�𖳎�����@��������Ȃ��G���[�n�𖳎�
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




