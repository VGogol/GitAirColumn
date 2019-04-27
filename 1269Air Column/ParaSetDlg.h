#pragma once
#include"G.h"
#include "afxcmn.h"
#include "afxwin.h"
#include"FileRW.h"

// CParaSetDlg 对话框

class CParaSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParaSetDlg)

public:
	CParaSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParaSetDlg();

// 对话框数据
	enum { IDD = IDD_PARA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int row, col;                //列表行列
	int  No;                      //更改的参数下标号

public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_paratab;
	CListCtrl m_paralist;
	afx_msg void OnHoverParaList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkParaList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeParaTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillfocusParachangeEdit();
	afx_msg void OnSetfocusParachangeEdit();
	CEdit m_parachangeedit;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 向列表控件中插入内容
	//类型0,1,2分别表示运动参数，相机参数，视觉参数
	int ListLoadItem(CListCtrl* pListCtrl,int nType);
	// 参数保存至文件
//	int ParaToFile();
	// 将变更的参数写入全局参数数组
	int ChangedToPara();
	afx_msg void OnBnClickedSaveparaButton();
	// 获取改变的参数号
	int GetChangeNo();
};
