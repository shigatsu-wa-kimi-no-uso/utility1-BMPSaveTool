
// BMPSaveToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "BMPSaveTool.h"
#include "BMPSaveToolDlg.h"
#include "afxdialogex.h"
#include "CBMPExtractor.h"
#include "CConfigDlg.h"
#include "CAboutDlg.h"
#include "CHelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBMPSaveToolDlg 对话框

BOOL g_AboutDlgExists;
BOOL g_ConfigDlgExists;
BOOL g_HelpDlgExists;
BOOL g_RegisterHotKeyState = FALSE;


CBMPSaveToolDlg::CBMPSaveToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BMPSAVETOOL_DIALOG, pParent)
{
	m_hIcon = NULL;
}


void CBMPSaveToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CBMPSaveToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_MESSAGE(WM_SYSTRAY, OnSystemTray)
	ON_COMMAND(ID_MENU_EXIT, &CBMPSaveToolDlg::OnMenuExit)
	ON_COMMAND(ID_MENU_ABOUT, &CBMPSaveToolDlg::OnMenuAbout)
	ON_COMMAND(ID_MENU_HELP, &CBMPSaveToolDlg::OnMenuHelp)
	ON_COMMAND(ID_MENU_CONFIG, &CBMPSaveToolDlg::OnMenuConfig)
END_MESSAGE_MAP()




// CBMPSaveToolDlg 消息处理程序

BOOL CBMPSaveToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	wp.flags = WPF_RESTORETOMAXIMIZED;
	wp.showCmd = SW_HIDE;
	SetWindowPlacement(&wp);

	TCHAR szBuf_path[PATH_LENGTH] = { 0 };
	WORD nBuf_Key[2] = { 0 };
	g_hWndMain = GetSafeHwnd();
	//创建托盘
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	NotifyIcon.hIcon = ExtractIcon(NULL, _T("Shell32.dll"), 2);
	NotifyIcon.hWnd = g_hWndMain;
	lstrcpy(NotifyIcon.szTip, _T("BMP自动保存工具"));
	NotifyIcon.uCallbackMessage = WM_SYSTRAY;
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);   //添加系统托盘

	if (!(g_myObj.ReadConfig() && g_myObj.GetHotKeyCfg(nBuf_Key)))
	{
		::MessageBox(NULL,TEXT("获取配置信息失败"), g_tszMsgTitle, MB_OK | MB_ICONERROR);		//读取配置
		nBuf_Key[0] = VK_NUMPAD9;
		nBuf_Key[1] = HOTKEYF_CONTROL;
	}

	
	if (nBuf_Key[1] == 0 || !RegisterHotKey(g_hWndMain, g_nHotKeyID, nBuf_Key[1], nBuf_Key[0]))
	{
		::MessageBox(NULL, TEXT("注册热键失败, 热键可能已被其他程序占用, 请设置其他热键"), g_tszMsgTitle, MB_OK | MB_ICONERROR);
		g_RegisterHotKeyState = FALSE;
	}
	g_RegisterHotKeyState = TRUE;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CBMPSaveToolDlg::OnPaint()
{
	ShowWindow(SW_HIDE);
}

//有关托盘的操作
afx_msg LRESULT CBMPSaveToolDlg::OnSystemTray(WPARAM wParam, LPARAM lParam)
{
	//wParam接收的是图标的ID，而lParam接收的是鼠标的行为

	switch (lParam)
	{
	case  WM_RBUTTONDOWN://右键起来时弹出快捷菜单
	{
		CMenu myMenuTray;					
		myMenuTray.LoadMenu(IDR_MENU_TRAY);				//加载菜单
		CMenu* pPopup = myMenuTray.GetSubMenu(0);
		CPoint myCursorPoint;
		GetCursorPos(&myCursorPoint);			
		SetForegroundWindow();
		PostMessage(WM_NULL, 0, 0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN, myCursorPoint.x, myCursorPoint.y, this);	//显示右键菜单，由视类窗口拥有。
	}
	break;
	case  WM_LBUTTONDOWN://左键单击的处理     
	{
		SetForegroundWindow();
		OnMenuConfig();
	}
	break;
	}
	return 0;
}

LRESULT CBMPSaveToolDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	TCHAR result[SZRESULTLENGTH] = { 0 };
	DWORD dwExitcode;
	DWORD WINAPI Implementation(TCHAR * _Out_ result);
	HANDLE hImplement = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Implementation , result, NULL, NULL);
	if (!hImplement)
	{
		::MessageBox(NULL,TEXT("创建线程失败"), g_tszMsgTitle, MB_OK | MB_ICONERROR);							//使用全局函数的msgbox, 防止弹窗阻塞
		return 1;
	}

	do
	{
		dwExitcode = 1;
		GetExitCodeThread(hImplement, &dwExitcode);
	} while (dwExitcode == STILL_ACTIVE);

	if (dwExitcode != 0)
	{
		::MessageBox(NULL, TEXT("程序发生致命性错误, 您可以通过程序的\"关于\"窗口获得作者的联系方式, 反馈问题"), g_tszMsgTitle, MB_OK | MB_ICONERROR);
		CloseHandle(hImplement);
		return 1;
	}
	CloseHandle(hImplement);

	Sleep(50);			//鲁棒性

	if (_tcscmp(result, _T("NOERROR")))		//线程返回信息判断
	{
		::MessageBox(NULL,result, g_tszMsgTitle, MB_OK | MB_ICONERROR);
	}
	return 0;
}

void CBMPSaveToolDlg::OnMenuAbout()
{
	// TODO: 在此添加命令处理程序代码
	
	CAboutDlg myAboutDlgObj;
	if (!g_AboutDlgExists)
	{
		g_AboutDlgExists = TRUE;
		myAboutDlgObj.DoModal();				//直到窗口退出才执行下面的内容
		g_AboutDlgExists = FALSE;
	}

}

void CBMPSaveToolDlg::OnMenuConfig()
{
	// TODO: 在此添加命令处理程序代码

	CConfigDlg myConfigDlgObj;
	if (!g_ConfigDlgExists)				//限制只能创建一个窗口
	{
		g_ConfigDlgExists = TRUE;
		myConfigDlgObj.DoModal();					//直到窗口退出才执行下面的内容
		g_ConfigDlgExists = FALSE;
	}

}


void CBMPSaveToolDlg::OnMenuHelp()
{
	// TODO: 在此添加命令处理程序代码
	CHelpDlg myHelpDlgObj;
	if (!g_HelpDlgExists)
	{
		g_HelpDlgExists = TRUE;
		myHelpDlgObj.DoModal();					//直到窗口退出才执行下面的内容
		g_HelpDlgExists = FALSE;
	}

}


//整个程序的退出操作
void CBMPSaveToolDlg::OnMenuExit()
{
	// TODO: 在此添加命令处理程序代码
	if (g_RegisterHotKeyState)						//注销热键
	{
		UnregisterHotKey(g_hWndMain, g_nHotKeyID);
	}
	Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);   //删除系统托盘
	ReleaseMutex(theApp.m_hMutex);

	PostQuitMessage(0);						//终止进程
}




