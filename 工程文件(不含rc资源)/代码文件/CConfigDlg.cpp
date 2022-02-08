// CConfigDlg.cpp: 实现文件
//


#include "pch.h"
#include "framework.h"
#include "BMPSaveTool.h"
#include "CConfigDlg.h"
#include "afxdialogex.h"


// CConfigDlg 对话框


IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)



CConfigDlg::CConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONFIG, pParent)
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_PathEditor);
	DDX_Control(pDX, IDC_HOTKEY1, m_HotKey1);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CONFIG_BNCANCEL, &CConfigDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CONFIG_BNSAVE, &CConfigDlg::OnBnClickedSave)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()



BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	TCHAR szBuf_path[PATH_LENGTH] = { 0 };
	WORD nBuf_Key[2] = { 0 };
	CMFCEditBrowseCtrl* pPathEditor;
	CHotKeyCtrl* pHotKey1;

	g_myObj.ReadConfig();			//读取配置

	pPathEditor = (CMFCEditBrowseCtrl*)GetDlgItem(IDC_MFCEDITBROWSE1);
	pHotKey1 = (CHotKeyCtrl*)GetDlgItem(IDC_HOTKEY1);

	if (g_myObj.GetSavePathCfg(szBuf_path) && g_myObj.GetHotKeyCfg(nBuf_Key))
	{
		pPathEditor->SetWindowText(szBuf_path);				//初始化路径，热键设置信息显示
		pHotKey1->SetHotKey(nBuf_Key[0], nBuf_Key[1]);
		UnregisterHotKey(g_hWndMain, g_nHotKeyID);			//注销热键
		++g_nHotKeyID;
	}
	else
	{
		::MessageBox(NULL,TEXT("获取配置信息失败"), g_tszMsgTitle, MB_OK | MB_ICONERROR);
		nBuf_Key[0] = VK_NUMPAD9;
		nBuf_Key[1] = HOTKEYF_CONTROL;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL PathCheck(TCHAR* szPath)
{
	TCHAR substr_Dir[PATH_LENGTH] = { 0 };

	if (szPath[1] != ':' ||
		szPath[2] != '\\' ||
		szPath[_tcslen(szPath) - 1] == '.' ||
		_tcsstr(szPath, _T("\\\\")) ||
		_tcsstr(szPath, _T("..")) ||
		_tcsstr(szPath, _T("*")) ||
		_tcsstr(szPath, _T("?")) ||
		_tcsstr(szPath, _T("/")) ||
		_tcsstr(szPath, _T(".\\")))
	{
		return FALSE;
	}

	if (szPath[_tcslen(szPath) - 1] != '\\')
		_tcscat_s(szPath, PATH_LENGTH, _T("\\"));

	for (register int i = 0; i < _tcslen(szPath); ++i)
	{
		if (szPath[i] == '\\')
		{
			_tcsncpy_s(substr_Dir, PATH_LENGTH, szPath, i + 1);
			if (!(PathIsDirectory(substr_Dir) || CreateDirectory(substr_Dir, NULL)))		// 是目录或创建成功时，该条件式为假
			{
				if (GetLastError() != ERROR_SUCCESS && GetLastError() != ERROR_ACCESS_DENIED && GetLastError() != ERROR_ALREADY_EXISTS)				//文件路径合法性判断
					return FALSE;
			}
		}
	}
	return TRUE;
}


void CConfigDlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szBuf_path[PATH_LENGTH] = { 0 };
	WORD nBuf_Key[2];
	WORD nBuf_oldKey[2];
	CMFCEditBrowseCtrl* pPathEditor= (CMFCEditBrowseCtrl*)GetDlgItem(IDC_MFCEDITBROWSE1);
	CHotKeyCtrl* pHotKey1 = (CHotKeyCtrl*)GetDlgItem(IDC_HOTKEY1);


	pPathEditor->GetWindowText(szBuf_path, PATH_LENGTH);

	if (!PathCheck(szBuf_path))
	{
		::MessageBox(NULL,TEXT("路径不合法或没有权限访问该路径"), g_tszMsgTitle, MB_OK | MB_ICONERROR);
		return;
	}

	pHotKey1->GetHotKey(nBuf_Key[0], nBuf_Key[1]);

	if (nBuf_Key[1] == 0)
	{
		::MessageBox(NULL,TEXT("必须使用一个组合键作为热键"), g_tszMsgTitle, MB_OK | MB_ICONERROR);
		return;
	}


	if (!RegisterHotKey(g_hWndMain, g_nHotKeyID, nBuf_Key[1], nBuf_Key[0]))		//注册新热键
	{
		::MessageBox(NULL,TEXT("注册热键失败, 热键已被其他程序占用, 请设置其他热键"), g_tszMsgTitle, MB_OK | MB_ICONERROR);
		g_RegisterHotKeyState = FALSE;
		return;
	}
	g_RegisterHotKeyState = TRUE;
	


	if (!(g_myObj.SetHotKeyCfg(nBuf_Key) &&
		g_myObj.SetSavePathCfg(szBuf_path) &&
		g_myObj.SaveConfig()))
	{
		::MessageBox(NULL,TEXT("保存配置信息失败"), g_tszMsgTitle, MB_OK | MB_ICONERROR);
		return;
	}
	::MessageBox(NULL,TEXT("保存成功"), g_tszMsgTitle, MB_OK | MB_ICONASTERISK);
	CDialog::OnOK();
}


void CConfigDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	WORD nBuf_oldKey[2];						//以前的热键注册
	g_myObj.GetHotKeyCfg(nBuf_oldKey);
	if (!RegisterHotKey(g_hWndMain, g_nHotKeyID, nBuf_oldKey[1], nBuf_oldKey[0]))
	{
		::MessageBox(NULL, TEXT("注册热键失败, 热键已被其他程序占用, 请设置其他热键"), g_tszMsgTitle, MB_OK | MB_ICONERROR);
		g_RegisterHotKeyState = FALSE;
		return;
	}
	g_RegisterHotKeyState = TRUE;

	CDialog::OnCancel();
}



// CConfigDlg 消息处理程序


