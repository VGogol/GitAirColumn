#pragma once
#include"G.h"
#include "afxwin.h"

// CVisionDebugDlg �Ի���

static     const  int    MAX_REGION_COUNTS = 3;              //�������������Ĭ��Ϊ3��


class CVisionDebugDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVisionDebugDlg)

public:
	CVisionDebugDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVisionDebugDlg();

public:
	CString   CamNub[2];                   //�������
	HTuple   ImageWindowHandle;              //��ʾͼ�񴰿�
	CRect     WindowRect;       
	short      CurParaNo;                   //��ǰѡ�������Ӧ�Ĳ�����
	int          CurRegionNo;                 //��ǰ������-�����½�ģ���������ʱ����
	string    CurModelPath;              //��ǰģ��·��-���ڱ����½�ģ��ʱѡ��·��
	double     score, testscore;              //ƥ��ģ���ֵ

	//ͼ���С�仯��
	double  ImgCol1;
	double  ImgRow1;
	double  ImgRow2;
	double  ImgCol2;
	
	HTuple       CurCamera;                    //��ǰѡ������
	HObject     grabimage;                 //ץȡ��ͼ��
	HTuple      CurModel;                   //��ǰѡ�������Ӧ��ģ��
	HTuple      NewModel;                //�½���ģ��
	HObject     hv_Rectangle[MAX_REGION_COUNTS];      //��������½�ģ��ʱ���Ƶ�����Ĭ�����Ϊ3��
	HObject     hv_Union[MAX_REGION_COUNTS];          //��������½�ģ��ʱ�ϲ�����
	//HTuple      hv_score, hv_testscore;                            //����ģ��ƥ���ֵ


// �Ի�������
	enum { IDD = IDD_VisionDebug_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	// ��ȡ����Ͷ�Ӧ����
	int GetCameraAndPara();
	afx_msg void OnBnClickedFindmodelButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedDisproiButton();
	afx_msg void OnBnClickedDrawregionButton();
	// ��ʼ������
	int InitVariable();
	afx_msg void OnBnClickedGenmodelButton();
	afx_msg void OnBnClickedTestnewmodelButton();
	afx_msg void OnBnClickedDispnewroiButton();
	afx_msg void OnBnClickedSavemodelButton();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedFitwindowButton();
};
