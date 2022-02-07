// CHelpDlg.cpp: 实现文件
//

#include "pch.h"
#include "BMPSaveTool.h"
#include "CHelpDlg.h"
#include "afxdialogex.h"


// CHelpDlg 对话框

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HELP, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
	ON_BN_CLICKED(IDC_HELP_BNCLOSE, &CHelpDlg::OnBnClickedClose)
END_MESSAGE_MAP()


// CHelpDlg 消息处理程序


void CHelpDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
