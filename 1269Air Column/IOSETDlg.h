#pragma once
#include"G.h"

// CIOSETDlg 对话框

class CIOSETDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIOSETDlg)

public:
	CIOSETDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIOSETDlg();

// 对话框数据
	enum { IDD = IDD_IOSET_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	HBRUSH      m_brush_green;
	HBRUSH      m_brush_red;
	HBRUSH      m_brush_yellow;


public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 将IO端口名称显示到EDIT控件中
	int LoadPortName();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedSaveioButton();
	//使用皮肤时 更新控件颜色
	void RefreshColor();
};
