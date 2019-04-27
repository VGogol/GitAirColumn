#pragma once
#include"G.h"
#include "afxwin.h"

// CVisionDebugDlg 对话框

static     const  int    MAX_REGION_COUNTS = 3;              //最大区域数量，默认为3个


class CVisionDebugDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVisionDebugDlg)

public:
	CVisionDebugDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVisionDebugDlg();

public:
	CString   CamNub[2];                   //相机名称
	HTuple   ImageWindowHandle;              //显示图像窗口
	CRect     WindowRect;       
	short      CurParaNo;                   //当前选择相机对应的参数号
	int          CurRegionNo;                 //当前区域编号-用于新建模板绘制区域时计数
	string    CurModelPath;              //当前模板路径-用于保存新建模板时选择路径
	double     score, testscore;              //匹配模板分值

	//图像大小变化用
	double  ImgCol1;
	double  ImgRow1;
	double  ImgRow2;
	double  ImgCol2;
	
	HTuple       CurCamera;                    //当前选择的相机
	HObject     grabimage;                 //抓取的图像
	HTuple      CurModel;                   //当前选择相机对应的模板
	HTuple      NewModel;                //新建的模板
	HObject     hv_Rectangle[MAX_REGION_COUNTS];      //用来存放新建模板时绘制的区域，默认最大为3个
	HObject     hv_Union[MAX_REGION_COUNTS];          //用来存放新建模板时合并区域
	//HTuple      hv_score, hv_testscore;                            //新老模板匹配分值


// 对话框数据
	enum { IDD = IDD_VisionDebug_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_CameraNub_combo;
	float m_X1Offset_edit;
	float m_Y1Offset_edit;
	float m_X2Offset_edit;
	float m_Y2Offset_edit;
	afx_msg void OnBnClickedGrabimageButton();
	afx_msg void OnBnClickedClearwindowButton();
	// 获取相机和对应参数
	int GetCameraAndPara();
	afx_msg void OnBnClickedFindmodelButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedDisproiButton();
	afx_msg void OnBnClickedDrawregionButton();
	// 初始化变量
	int InitVariable();
	afx_msg void OnBnClickedGenmodelButton();
	afx_msg void OnBnClickedTestnewmodelButton();
	afx_msg void OnBnClickedDispnewroiButton();
	afx_msg void OnBnClickedSavemodelButton();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedFitwindowButton();
};
