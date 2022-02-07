
// BMPSaveTool.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CBMPSaveToolApp:
// 有关此类的实现，请参阅 BMPSaveTool.cpp
//

class CBMPSaveToolApp : public CWinApp
{
public:
	CBMPSaveToolApp();

// 重写
public:
	virtual BOOL InitInstance();
	HANDLE m_hMutex;						//互斥锁

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CBMPSaveToolApp theApp;
extern CBMPExtractor g_myObj;
extern int g_nHotKeyID;
extern TCHAR g_tszMsgTitle[30];
extern HWND g_hWndMain;

