#pragma once

// CConfigDlg 对话框

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CConfigDlg();
	BOOL m_DlgExists;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	BOOL CConfigDlg::PreTranslateMessage(MSG* pMsg)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
		return CDialog::PreTranslateMessage(pMsg);
	}


	// 浏览路径控件
	CMFCEditBrowseCtrl m_PathEditor;
	// 热键
	CHotKeyCtrl m_HotKey1;
	afx_msg void OnBnClickedMfcbutton1();
	afx_msg void OnBnClickedMfcbutton2();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
};

extern CConfigDlg g_myConfigDlgObj;
