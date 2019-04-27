#pragma once
#include"G.h"
#include"ParaSetDlg.h"

// CPSWDlg 对话框

class CPSWDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPSWDlg)

public:
	CPSWDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPSWDlg();

// 对话框数据
	enum { IDD = IDD_PSW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	//CParaSetDlg  *parasetdlg;
public:
	afx_msg void OnBnClickedOk();
	CString m_psw;
};
