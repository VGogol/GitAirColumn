
// 1269Air ColumnDlg.h : 头文件
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

// CMy1269AirColumnDlg 对话框
class CMy1269AirColumnDlg : public CDialogEx
{
// 构造
public:
	CMy1269AirColumnDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY1269AIRCOLUMN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	CRect                   rect1, rect2, rect3, rect4,rect5,rect6;          //获取对话框四个窗口的大小

	//0,1,2,3,17,18六个抓取图像   4复位线程  5实时线程
	//6轴运动线程   7获取位置触发相机线程
	//8,9,10,11,20,21六个图像处理线程     12,13两个分析结果线程
	//14,15两个位置刷新吹气线程   16刷新板卡状态数据写入文件等
	//19尺寸数据写入数据库线程（改成用Excel，不用此线程）  
	HANDLE               m_hThread[25];
	int                        m_exitThread[25];
	//bool                     test;

	int                       imageno1, imageno2, imageno3, imageno4, imageno5, imageno6;            //存图时编号
	CString  excelpath;                       //写入文件路径名

	long                    m_lTotalCounts;                          //生产总数，用于计算uph
	DWORD              m_dwUsedTime;                          //生产时间
	DWORD              m_dwNow;                                  //现在时间，用于计算总生产时间
	DWORD              m_dwLast;                                   //上次时间，用于计算总生产时间


	short                  LeftOK1, RightOK1, LeftOK2, RightOK2;    //两端外观检测结果

public:
	afx_msg void OnBnClickedCancel();
	// 退出软件
	void ExitSoft();
	afx_msg void OnExitMenu();
	afx_msg void OnParasetMenu();
	afx_msg void OnIoset();
	afx_msg void OnMotionMenu();
	// 读取参数
	int ReadPara();
	// 将参数写入文件
	int WritePara();
	afx_msg void OnBnClickedRstButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedRealtimeButton();

	afx_msg void OnBnClickedGrabButton();
	afx_msg void OnBnClickedStartButton();
	// 开始运行
	int Start();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClearProduct();
	afx_msg void OnBnClickedStopButton();
	// 将检测数据写入Excel
	void WriteCheckDataToExcel();
	// 更新其他状态信息，三色灯，控件数据，参数等
	void RefreshStatus();
//	afx_msg void OnBnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListBox m_message_listbox;
	CListCtrl m_data_list;
	// 运行中变成暂停处理
	void ToPause();
	CListCtrl m_datalist2;
	afx_msg void OnBnClickedShieldleftButton();
	afx_msg void OnBnClickedShieldrightButton();
	void ReadIO();
	afx_msg void OnBnClickedWritedataleftButton();
	afx_msg void OnBnClickedWritedatarightButton();
	afx_msg void OnVisondebug();
};
