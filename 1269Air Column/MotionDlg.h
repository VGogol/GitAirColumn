#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include"G.h"

// CMotionDlg �Ի���

class CMotionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMotionDlg)

public:
	CMotionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMotionDlg();

// �Ի�������
	enum { IDD = IDD_MOTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CImageList  m_imagelist;
	CString        AxisName[2];                 //�����

	BOOL          RSTing;                         //��λ��

public:
	CListCtrl m_AxisState_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_Axis_combo;
	afx_msg void OnBnClickedAxisenableButton();
	afx_msg void OnBnClickedRstButton();
	// ��ʹ��
	BOOL AxisEnable(UINT  AxisNo);
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnBnClickedPostozeroButton();
	afx_msg void OnBnClickedPosrunButton();
	float m_speed_edit;
	long m_distance_edit;
	afx_msg void OnBnClickedNegrunButton();
	// ˢ����״̬�б�
	void RefreshAxisStateList();
	afx_msg void OnBnClickedMovetoposButton();
	afx_msg void OnBnClickedSaveposButton();
};
