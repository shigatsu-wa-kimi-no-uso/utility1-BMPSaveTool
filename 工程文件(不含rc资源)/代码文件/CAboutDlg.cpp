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

}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ABOUT_BNCLOSE, &CAboutDlg::OnBnClickedClose)
END_MESSAGE_MAP()


// CAboutDlg 消息处理程序


void CAboutDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

