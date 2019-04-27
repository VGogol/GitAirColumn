#pragma once
#include"G.h"
#include "afxcmn.h"
#include "afxwin.h"
#include"FileRW.h"

// CParaSetDlg �Ի���

class CParaSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParaSetDlg)

public:
	CParaSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParaSetDlg();

// �Ի�������
	enum { IDD = IDD_PARA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	int row, col;                //�б�����
	int  No;                      //���ĵĲ����±��

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
	// ���б�ؼ��в�������
	//����0,1,2�ֱ��ʾ�˶�����������������Ӿ�����
	int ListLoadItem(CListCtrl* pListCtrl,int nType);
	// �����������ļ�
//	int ParaToFile();
	// ������Ĳ���д��ȫ�ֲ�������
	int ChangedToPara();
	afx_msg void OnBnClickedSaveparaButton();
	// ��ȡ�ı�Ĳ�����
	int GetChangeNo();
};
