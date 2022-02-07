
// BMPSaveToolDlg.h: 头文件
//

#pragma once
#include "framework.h"

#define WM_SYSTRAY (WM_USER + 3)					// 定义托盘消息, WM_USER以上为用户自定义消息区间

// CBMPSaveToolDlg 对话框
class CBMPSaveToolDlg : public CDialogEx
{
// 构造
public:
	CBMPSaveToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BMPSAVETOOL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);   //热键响应处理函数声明
	afx_msg LRESULT OnSystemTray(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	BOOL CBMPSaveToolDlg::PreTranslateMessage(MSG* pMsg)
	{
			return CDialog::PreTranslateMessage(pMsg);
	}


public:
	//系统托盘类
	NOTIFYICONDATA NotifyIcon;  
	// 浏览路径控件
	CMFCEditBrowseCtrl m_PathEditor;
	// 热键
	CHotKeyCtrl m_HotKey1;
	afx_msg void OnMenuExit();
	afx_msg void OnMenuConfig();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuHelp();
};
