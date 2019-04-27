// VisionDebugDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "VisionDebugDlg.h"
#include "afxdialogex.h"


using namespace HalconCpp;
// CVisionDebugDlg 对话框

IMPLEMENT_DYNAMIC(CVisionDebugDlg, CDialogEx)

CVisionDebugDlg::CVisionDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVisionDebugDlg::IDD, pParent)
	, m_X1Offset_edit(0)
	, m_Y1Offset_edit(0)
	, m_X2Offset_edit(0)
	, m_Y2Offset_edit(0)
{

}

CVisionDebugDlg::~CVisionDebugDlg()
{
}

void CVisionDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CameraNub_COMBO, m_CameraNub_combo);
	DDX_Text(pDX, IDC_X1OFFSET_EDIT, m_X1Offset_edit);
	DDX_Text(pDX, IDC_Y1OFFSET_EDIT, m_Y1Offset_edit);
	DDX_Text(pDX, IDC_X2OFFSET_EDIT, m_X2Offset_edit);
	DDX_Text(pDX, IDC_Y2OFFSET_EDIT, m_Y2Offset_edit);
}


BEGIN_MESSAGE_MAP(CVisionDebugDlg, CDialogEx)
	ON_BN_CLICKED(IDC_GrabImage_BUTTON, &CVisionDebugDlg::OnBnClickedGrabimageButton)
	ON_BN_CLICKED(IDC_ClearWindow_BUTTON, &CVisionDebugDlg::OnBnClickedClearwindowButton)
	ON_BN_CLICKED(IDC_FindModel_BUTTON, &CVisionDebugDlg::OnBnClickedFindmodelButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_DispROI_BUTTON, &CVisionDebugDlg::OnBnClickedDisproiButton)
	ON_BN_CLICKED(IDC_DrawRegion_BUTTON, &CVisionDebugDlg::OnBnClickedDrawregionButton)
	ON_BN_CLICKED(IDC_GenModel_BUTTON, &CVisionDebugDlg::OnBnClickedGenmodelButton)
	ON_BN_CLICKED(IDC_TestNewModel_BUTTON, &CVisionDebugDlg::OnBnClickedTestnewmodelButton)
	ON_BN_CLICKED(IDC_DispNewROI_BUTTON, &CVisionDebugDlg::OnBnClickedDispnewroiButton)
	ON_BN_CLICKED(IDC_SaveModel_BUTTON, &CVisionDebugDlg::OnBnClickedSavemodelButton)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_FitWindow_BUTTON, &CVisionDebugDlg::OnBnClickedFitwindowButton)
END_MESSAGE_MAP()


// CVisionDebugDlg 消息处理程序


BOOL CVisionDebugDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CamNub[0] = L"左尺寸相机";
	CamNub[1] = L"右尺寸相机";
	for (int i = 0; i < 2; i++)
	{
		m_CameraNub_combo.AddString(CamNub[i]);
	}
	m_CameraNub_combo.SetCurSel(0);

	//设置窗口
	HWND   HW1X = GetDlgItem(IDC_Image_PIC)->m_hWnd;
	GetDlgItem(IDC_Image_PIC)->GetClientRect(WindowRect);
	OpenWindow(WindowRect.top, WindowRect.left, WindowRect.Width(), WindowRect.Height(), (Hlong)HW1X, "visible", "", &ImageWindowHandle);

	SetColor(ImageWindowHandle, "red");
	//SetLineWidth(ImageWindowHandle, 3);

	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//按钮-采集图像
void CVisionDebugDlg::OnBnClickedGrabimageButton()
{
	// TODO:  在此添加控件通知处理程序代码
	GetCameraAndPara();
	try
	{
		ReadImage(&grabimage, "C:/Users/Administrator/Desktop/1269/1.bmp");
		/*GrabImage(&grabimage, CurCamera);*/
		//显示
		DispObj(grabimage, ImageWindowHandle);
	}
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		MessageBox(L"抓取图像失败，请重新抓取！", L"警告", MB_ICONWARNING);
	}
}

//按钮-清屏
void CVisionDebugDlg::OnBnClickedClearwindowButton()
{
	// TODO:  在此添加控件通知处理程序代码
	//grabimage = null;
	ClearWindow(ImageWindowHandle);
}


// 获取相机和对应模板、参数
int CVisionDebugDlg::GetCameraAndPara()
{
	int  camerano = m_CameraNub_combo.GetCurSel();            //获取当前选择相机号
	switch (camerano)
	{
	case 0:
		//当前相机
		CurCamera = G::hv_AcqHandle1;
		CurModel = G::modle1;
		CurModelPath = LeftUPModelPath;
		SetPart(ImageWindowHandle, 0, 0, 1200, 1920);
		ImgCol1 = 0;
		ImgRow1 = 0;
		ImgRow2 = 1200;
		ImgCol2 = 1920;
		break;
	case 1:
		CurCamera = G::hv_AcqHandle4;
		SetPart(ImageWindowHandle, 0, 0, 1200, 1920);
		CurModelPath = RightUPModelPath;
		ImgCol1 = 0;
		ImgRow1 = 0;
		ImgRow2 = 1200;
		ImgCol2 = 1920;
		break;
	}

	return 0;
}

//按钮-匹配模板（匹配原先模板）
void CVisionDebugDlg::OnBnClickedFindmodelButton()
{
	// TODO:  在此添加控件通知处理程序代码
	HTuple     hv_Row, hv_Column, hv_Angle;       //找模板函数对应参数
	HTuple     hv_score;
	GetCameraAndPara();
	try
	{
		FindShapeModel(grabimage, CurModel, -0.39, 0.78, 0.85, 1, 0.5, "least_squares",
			0, 0.9, &hv_Row, &hv_Column, &hv_Angle, &hv_score);
		score = hv_score.D();
		if (0 != ((hv_Row.TupleLength()) == 1))
		{
			G::dev_display_shape_matching_results(CurModel, "red", hv_Row, hv_Column, hv_Angle, 1, 1, 0,ImageWindowHandle);
			G::disp_message(ImageWindowHandle, "匹配分值: " + hv_score, "image", 260, 60, "green", "false");
		}
		else
		{
			G::disp_message(ImageWindowHandle, "没有匹配到模板", "image", 100, 100, "red", "false");
		}
	}
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		G::disp_message(ImageWindowHandle, "没有匹配到模板", "image", 100, 100, "red", "false");
	}
}


void CVisionDebugDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CString   str;
	str.Format(L"%.6f", score);
	SetDlgItemText(IDC_ModelScore_STATIC, str);
	str.Format(L"%.6f", testscore/*[0].D()*/);
	SetDlgItemText(IDC_ModelTestScore_STATIC, str);

	CDialogEx::OnTimer(nIDEvent);
}

//按钮-显示ROI
void CVisionDebugDlg::OnBnClickedDisproiButton()
{
	// TODO:  在此添加控件通知处理程序代码
	SetDraw(ImageWindowHandle, "margin");
	//绘制矩形
	//显示矩形
}

//按钮-绘制区域
void CVisionDebugDlg::OnBnClickedDrawregionButton()
{
	// TODO:  在此添加控件通知处理程序代码
	HTuple       hv_row1, hv_col1, hv_row2, hv_col2;

	SetDraw(ImageWindowHandle, "margin");
	DrawRectangle1(ImageWindowHandle, &hv_row1, &hv_col1, &hv_row2, &hv_col2);
	GenRectangle1(&hv_Rectangle[CurRegionNo], hv_row1, hv_col1, hv_row2, hv_col2);
	DispObj(hv_Rectangle[CurRegionNo], ImageWindowHandle);

	CurRegionNo++;
}


// 初始化变量
int CVisionDebugDlg::InitVariable()
{
	CurParaNo = 0;
	CurRegionNo = 0;
	return 0;
}

//按钮-生成模板
void CVisionDebugDlg::OnBnClickedGenmodelButton()
{
	// TODO:  在此添加控件通知处理程序代码
	int   unionno=0;           //联合区域下标
	HObject    ho_modelimage;         //新建模板-模板图像 
	try
	{
		if (CurRegionNo == 1)
		{
			HalconCpp::ReduceDomain(grabimage, hv_Rectangle[0], &ho_modelimage);
		}       //if
		else if (CurRegionNo > 1)
		{
			for (int i = 0; i < CurRegionNo - 1; i++)
			{
				Union2(hv_Rectangle[i], hv_Rectangle[i + 1], &hv_Union[i]);
				unionno++;
			}
			HalconCpp::ReduceDomain(grabimage, hv_Union[unionno - 1], &ho_modelimage);
		}        //else if
		CreateShapeModel(ho_modelimage, "auto", -0.39, 0.79, "auto", "auto", "use_polarity", "auto", "auto", &NewModel);
		MessageBox(L"生成成功！", L"提示", MB_ICONINFORMATION);
	}
	catch (...)
	{
		MessageBox(L"生成模板失败！", L"错误", MB_ICONERROR);
	}
	
}

//按钮-测试新模板
void CVisionDebugDlg::OnBnClickedTestnewmodelButton()
{
	// TODO:  在此添加控件通知处理程序代码
	HTuple     hv_Row, hv_Column, hv_Angle;       //找模板函数对应参数
	HTuple     hv_testscore;
	try
	{
		FindShapeModel(grabimage, NewModel, -0.39, 0.78, 0.85, 1, 0.5, "least_squares",
			0, 0.8, &hv_Row, &hv_Column, &hv_Angle, &hv_testscore);
		testscore = hv_testscore.D();
		if (0 != ((hv_Row.TupleLength()) == 1))
		{
			G::dev_display_shape_matching_results(NewModel, "red", hv_Row, hv_Column, hv_Angle, 1, 1, 0,ImageWindowHandle);
			G::disp_message(ImageWindowHandle, "匹配分值: " + hv_testscore, "image", 260, 60, "green", "false");
		}
		else
		{
			G::disp_message(ImageWindowHandle, "没有匹配到模板", "image", 100, 100, "red", "false");
		}
	}
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		G::disp_message(ImageWindowHandle, "没有匹配到模板", "image", 100, 100, "red", "false");
	}
}

//按钮-显示新建模板对应ROI
void CVisionDebugDlg::OnBnClickedDispnewroiButton()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
}

//按钮-保存模板
void CVisionDebugDlg::OnBnClickedSavemodelButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (IDNO == MessageBox(L"确定保存新建模板？", L"提示", MB_YESNO | MB_ICONQUESTION))
		return;
	//char* path = (LPSTR)(LPCTSTR)(CurModelPath);
	const char *path = CurModelPath.data();
	try
	{
		WriteShapeModel(NewModel, path);
		MessageBox(L"保存模板成功，重启软件生效！", L"提示", MB_OK);
	}
	catch (...)
	{
		MessageBox(L"保存模板失败！", L"错误", MB_ICONERROR);
	}
}


BOOL CVisionDebugDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//CRect rtImage;
	//GetDlgItem(IDC_Image_PIC)->GetWindowRect(rtImage);
	//Hlong   ImagePtX, ImagePtY;
	//Hlong   Row0, Col0, Row1, Col1;
	//double  Scale = 0.1;
	//if (zDelta > 0)
	//{
	//	ImagePtX = m_dDispImagePartCol0 + (pt.x - rtImage.left) / (rtImage.Width() - 1.0)*(m_dDispImagePartCol1 - m_dDispImagePartCol0);
	//	ImagePtY = m_dDispImagePartRow0 + (pt.y - rtImage.top) / (rtImage.Height() - 1.0)*(m_dDispImagePartRow1 - m_dDispImagePartRow0);
	//	Row0 = ImagePtY - 1 / (1 - Scale)*(ImagePtY - m_dDispImagePartRow0);
	//	Row1 = ImagePtY - 1 / (1 - Scale)*(ImagePtY - m_dDispImagePartRow1);
	//	Col0 = ImagePtX - 1 / (1 - Scale)*(ImagePtX - m_dDispImagePartCol0);
	//	Col1 = ImagePtX - 1 / (1 - Scale)*(ImagePtX - m_dDispImagePartCol1);

	//	m_dDispImagePartRow0 = Row0;
	//	m_dDispImagePartCol0 = Col0;
	//	m_dDispImagePartRow1 = Row1;
	//	m_dDispImagePartCol1 = Col1;
	//}
	//else
	//{
	//	ImagePtX = m_dDispImagePartCol0 + (pt.x - rtImage.left) / (rtImage.Width() - 1.0)*(m_dDispImagePartCol1 - m_dDispImagePartCol0);
	//	ImagePtY = m_dDispImagePartRow0 + (pt.y - rtImage.top) / (rtImage.Height() - 1.0)*(m_dDispImagePartRow1 - m_dDispImagePartRow0);
	//	Row0 = ImagePtY - 1 / (1 + Scale)*(ImagePtY - m_dDispImagePartRow0);
	//	Row1 = ImagePtY - 1 / (1 + Scale)*(ImagePtY - m_dDispImagePartRow1);
	//	Col0 = ImagePtX - 1 / (1 + Scale)*(ImagePtX - m_dDispImagePartCol0);
	//	Col1 = ImagePtX - 1 / (1 + Scale)*(ImagePtX - m_dDispImagePartCol1);

	//	m_dDispImagePartRow0 = Row0;
	//	m_dDispImagePartCol0 = Col0;
	//	m_dDispImagePartRow1 = Row1;
	//	m_dDispImagePartCol1 = Col1;
	//}

	////显示图像
	//SetPart(ImageWindowHandle, m_dDispImagePartRow0, m_dDispImagePartCol0, m_dDispImagePartRow1 - 1, m_dDispImagePartCol1 - 1);
	//DispObj(grabimage, ImageWindowHandle);
	//SetSystem("flush_graphic", "true");

	HTuple row, col, button;
	HTuple str;
	Herror error_num;
	double   scale;
	SetSystem("do_low_error", "stderr");
	SetCheck("~give_error");


	
	//error_num = GetMposition(ImageWindowHandle, &row, &col, &button);
	GetMposition(ImageWindowHandle, &row, &col, &button);
	SetCheck("~give_error");
	if (/*error_num == H_MSG_TRUE*/1)
	{
		SetTposition(ImageWindowHandle, row, col);
		str.Append(row).Append(",");
		str.Append(col);

		if (zDelta < 0)
		{
			scale = 0.9;                            //放大
		}
		else if (zDelta > 0)
		{
			scale = 1 / 0.9;                     //缩小
		}
		else
			return FALSE;
		ImgCol1 = col - abs(double(col - ImgCol1))* scale;
		ImgCol2 = col + abs(double(col - ImgCol2))* scale;
		ImgRow1 = row - abs(double(row - ImgRow1))* scale;
		ImgRow2 = row + abs(double(row - ImgRow2))* scale;
		str.Append(",").Append(ImgRow1).Append(",").Append(ImgRow2).Append(",").Append(ImgCol1).Append(",").Append(ImgCol2);
		SetPart(ImageWindowHandle, ImgRow1, ImgCol1, ImgRow2, ImgCol2);
		ClearWindow(ImageWindowHandle);
		DispImage(grabimage, ImageWindowHandle);
		//WriteString(ImageWindowHandle, str);
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

//按钮-适应窗口
void CVisionDebugDlg::OnBnClickedFitwindowButton()
{
	// TODO:  在此添加控件通知处理程序代码
	GetCameraAndPara();
	try
	{
		//显示
		DispImage(grabimage, ImageWindowHandle);
		//DispObj(grabimage, ImageWindowHandle);
	}
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		//MessageBox(L"抓取图像失败，请重新抓取！", L"警告", MB_ICONWARNING);
	}

}
