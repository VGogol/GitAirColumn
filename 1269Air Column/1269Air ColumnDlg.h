
// 1269Air ColumnDlg.h : ͷ�ļ�
//

#pragma once
#include"G.h"
#include"PSWDlg.h"
#include"ParaSetDlg.h"
#include"IOSETDlg.h"
#include"MotionDlg.h"
#include"VisionDebugDlg.h"
#include"FileRW.h"
#include "afxwin.h"
#include "afxcmn.h"

// CMy1269AirColumnDlg �Ի���
class CMy1269AirColumnDlg : public CDialogEx
{
// ����
public:
	CMy1269AirColumnDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY1269AIRCOLUMN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIOSETDlg           *ioset;
	CFont                   font1, font2;
	COLORREF           m_crWhite;
	HBRUSH               m_hrWhite;
	CFileRW               filerw;
	 
public:
	CRect                   rect1, rect2, rect3, rect4,rect5,rect6;          //��ȡ�Ի����ĸ����ڵĴ�С

	//0,1,2,3,17,18����ץȡͼ��   4��λ�߳�  5ʵʱ�߳�
	//6���˶��߳�   7��ȡλ�ô�������߳�
	//8,9,10,11,20,21����ͼ�����߳�     12,13������������߳�
	//14,15����λ��ˢ�´����߳�   16ˢ�°忨״̬����д���ļ���
	//19�ߴ�����д�����ݿ��̣߳��ĳ���Excel�����ô��̣߳�  
	HANDLE               m_hThread[25];
	int                        m_exitThread[25];
	//bool                     test;

	int                       imageno1, imageno2, imageno3, imageno4, imageno5, imageno6;            //��ͼʱ���
	CString  excelpath;                       //д���ļ�·����

	long                    m_lTotalCounts;                          //�������������ڼ���uph
	DWORD              m_dwUsedTime;                          //����ʱ��
	DWORD              m_dwNow;                                  //����ʱ�䣬���ڼ���������ʱ��
	DWORD              m_dwLast;                                   //�ϴ�ʱ�䣬���ڼ���������ʱ��


	short                  LeftOK1, RightOK1, LeftOK2, RightOK2;    //������ۼ����

public:
	afx_msg void OnBnClickedCancel();
	// �˳����
	void ExitSoft();
	afx_msg void OnExitMenu();
	afx_msg void OnParasetMenu();
	afx_msg void OnIoset();
	afx_msg void OnMotionMenu();
	// ��ȡ����
	int ReadPara();
	// ������д���ļ�
	int WritePara();
	afx_msg void OnBnClickedRstButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedRealtimeButton();

	afx_msg void OnBnClickedGrabButton();
	afx_msg void OnBnClickedStartButton();
	// ��ʼ����
	int Start();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClearProduct();
	afx_msg void OnBnClickedStopButton();
	// ���������д��Excel
	void WriteCheckDataToExcel();
	// ��������״̬��Ϣ����ɫ�ƣ��ؼ����ݣ�������
	void RefreshStatus();
//	afx_msg void OnBnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListBox m_message_listbox;
	CListCtrl m_data_list;
	// �����б����ͣ����
	void ToPause();
	CListCtrl m_datalist2;
	afx_msg void OnBnClickedShieldleftButton();
	afx_msg void OnBnClickedShieldrightButton();
	void ReadIO();
	afx_msg void OnBnClickedWritedataleftButton();
	afx_msg void OnBnClickedWritedatarightButton();
	afx_msg void OnVisondebug();
};
