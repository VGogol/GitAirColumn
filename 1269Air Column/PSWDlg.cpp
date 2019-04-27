// PSWDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "PSWDlg.h"
#include "afxdialogex.h"


// CPSWDlg 对话框

IMPLEMENT_DYNAMIC(CPSWDlg, CDialogEx)

CPSWDlg::CPSWDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPSWDlg::IDD, pParent)
	, m_psw(_T(""))
{

}

CPSWDlg::~CPSWDlg()
{
}

void CPSWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PSW_EDIT, m_psw);
}


BEGIN_MESSAGE_MAP(CPSWDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPSWDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPSWDlg 消息处理程序


void CPSWDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	UpdateData(TRUE);                //从控件到变量
	if (L"admin" == m_psw||(L"ADMIN"==m_psw)||("gogol"==m_psw)||("GOGOL"==m_psw))
	{
		if (("gogol" == m_psw) || ("GOGOL" == m_psw))
		{
			G::UserAuthority = 10;
		}
		else
			G::UserAuthority = 5;
		CDialogEx::OnOK();

		CParaSetDlg  parasetdlg;
		parasetdlg.DoModal();

		////////////////////////////////////////不适合用非模态框////////////////////////////////
		//if (NULL == parasetdlg)
		//{
		//	parasetdlg = new  CParaSetDlg();
		//	parasetdlg->Create(IDD_PARA_DIALOG, this);
		//}
		////显示非模态对话框
		//parasetdlg->ShowWindow(SW_SHOW);
		//////////////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		AfxMessageBox(L"密码错误!");
	}
}
