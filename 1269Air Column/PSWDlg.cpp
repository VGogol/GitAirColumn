// PSWDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "PSWDlg.h"
#include "afxdialogex.h"


// CPSWDlg �Ի���

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


// CPSWDlg ��Ϣ�������


void CPSWDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	UpdateData(TRUE);                //�ӿؼ�������
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

		////////////////////////////////////////���ʺ��÷�ģ̬��////////////////////////////////
		//if (NULL == parasetdlg)
		//{
		//	parasetdlg = new  CParaSetDlg();
		//	parasetdlg->Create(IDD_PARA_DIALOG, this);
		//}
		////��ʾ��ģ̬�Ի���
		//parasetdlg->ShowWindow(SW_SHOW);
		//////////////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		AfxMessageBox(L"�������!");
	}
}
