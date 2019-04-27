#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include"G.h"

// CMotionDlg 对话框

class CMotionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMotionDlg)

public:
	CMotionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMotionDlg();

// 对话框数据
	enum { IDD = IDD_MOTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CImageList  m_imagelist;
	CString        AxisName[2];                 //轴号名

	BOOL          RSTing;                         //复位中

public:
	CListCtrl m_AxisState_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_Axis_combo;
	afx_msg void OnBnClickedAxisenableButton();
	afx_msg void OnBnClickedRstButton();
	// 轴使能
	BOOL AxisEnable(UINT  AxisNo);
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnBnClickedPostozeroButton();
	afx_msg void OnBnClickedPosrunButton();
	float m_speed_edit;
	long m_distance_edit;
	afx_msg void OnBnClickedNegrunButton();
	// 刷新轴状态列表
	void RefreshAxisStateList();
	afx_msg void OnBnClickedMovetoposButton();
	afx_msg void OnBnClickedSaveposButton();
};
