#pragma once
#include"G.h"
#include"ParaSetDlg.h"

// CPSWDlg �Ի���

class CPSWDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPSWDlg)

public:
	CPSWDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPSWDlg();

// �Ի�������
	enum { IDD = IDD_PSW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//CParaSetDlg  *parasetdlg;
public:
	afx_msg void OnBnClickedOk();
	CString m_psw;
};
