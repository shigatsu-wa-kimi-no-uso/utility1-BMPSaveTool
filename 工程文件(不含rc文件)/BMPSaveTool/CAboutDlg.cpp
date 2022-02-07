// CAboutDlg.cpp: 实现文件
//

#include "pch.h"
#include "BMPSaveTool.h"
#include "CAboutDlg.h"
#include "afxdialogex.h"


// CAboutDlg 对话框

IMPLEMENT_DYNAMIC(CAboutDlg, CDialogEx)

CAboutDlg::CAboutDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUT, pParent)
{
	m_DlgExists = FALSE;
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CAboutDlg::OnBnClickedMfcbutton1)
END_MESSAGE_MAP()


// CAboutDlg 消息处理程序


void CAboutDlg::OnBnClickedMfcbutton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

