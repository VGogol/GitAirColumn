#pragma once
#include"G.h"

// CIOSETDlg �Ի���

class CIOSETDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIOSETDlg)

public:
	CIOSETDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIOSETDlg();

// �Ի�������
	enum { IDD = IDD_IOSET_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	HBRUSH      m_brush_green;
	HBRUSH      m_brush_red;
	HBRUSH      m_brush_yellow;


public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ��IO�˿�������ʾ��EDIT�ؼ���
	int LoadPortName();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedSaveioButton();
	//ʹ��Ƥ��ʱ ���¿ؼ���ɫ
	void RefreshColor();
};
