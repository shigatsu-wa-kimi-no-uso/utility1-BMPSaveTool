
// BMPSaveTool.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "BMPSaveTool.h"
#include "BMPSaveToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBMPSaveToolApp

BEGIN_MESSAGE_MAP(CBMPSaveToolApp, CWinApp)

END_MESSAGE_MAP()


// CBMPSaveToolApp 构造

CBMPSaveToolApp::CBMPSaveToolApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

 
// 唯一的 CBMPSaveToolApp 对象

CBMPSaveToolApp theApp;
int g_nHotKeyID = 1000;
TCHAR g_tszMsgTitle[30] = _T("剪切板BMP保存工具");
BOOL g_AboutDlgExist;
BOOL g_CfgDlgExist;
BOOL g_HelpDlgExist;
HWND g_hWndMain;

// CBMPSaveToolApp 初始化


BOOL CBMPSaveToolApp::InitInstance()
{
	CWinApp::InitInstance();

	m_hMutex= CreateMutex(NULL, FALSE,m_pszAppName);	//向系统添加互斥体,防止多个程序实例运行
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if(m_hMutex)
			CloseHandle(m_hMutex);
		return FALSE;
	}

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CBMPSaveToolDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;

}

