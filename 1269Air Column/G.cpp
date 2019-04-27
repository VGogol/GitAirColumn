#include "stdafx.h"
#include "G.h"


CMCard           G::MCard;


int                 G::MotionUsed;                      //运动参数使用个数
int                 G::Camera1Used;                  //相机1参数已使用个数
int                 G::Camera2Used;                  //相机2参数已使用个数
int                 G::Camera3Used;                  //相机3参数已使用个数
int                 G::Camera4Used;                  //相机4参数已使用个数
int                 G::Camera5Used;                   //相机5参数已使用个数
int                 G::Camera6Used;                   //相机6参数已使用个数
int                 G::Vision1Used;                     //视觉1参数已使用个数
int                 G::Vision2Used;                     //视觉2参数已使用个数
int                 G::Vision3Used;                     //视觉3参数已使用个数
int                 G::Vision4Used;                     //视觉4参数已使用个数
int                 G::Vision5Used;                     //视觉5参数已使用个数
int                 G::Vision6Used;                     //视觉6参数已使用个数

PARAM                    G::PR[All_Para_Counts];
DB					       G::db;
CCriticalSection       G::m_criticalsection_imagelist1;			          //图像列表1临界
CCriticalSection       G::m_criticalsection_imagelist2;			          //图像列表2临界
CCriticalSection       G::m_criticalsection_imagelist3;			          //图像列表3临界
CCriticalSection       G::m_criticalsection_imagelist4;                   //图像列表4临界
CCriticalSection       G::m_criticalsection_imagelist5;                   //图像列表5临界
CCriticalSection       G::m_criticalsection_imagelist6;                   //图像列表6临界
//CCriticalSection       G::m_criticalsection_productlist1;               //产品列表1临界
//CCriticalSection       G::m_criticalsection_productlist2;               //产品列表2临界

CCriticalSection       G::m_criticalsection_LeftPositive;            //左产品正向临界
CCriticalSection       G::m_criticalsection_RightPositive;         //右产品正向临界
CCriticalSection       G::m_criticalsection_LeftLateral1;           //左产品左侧临界
CCriticalSection       G::m_criticalsection_LeftLateral2;           //左产品右侧临界
CCriticalSection       G::m_criticalsection_RightLateral1;         //右产品左侧临界
CCriticalSection       G::m_criticalsection_RightLateral2;         //右产品右侧临界
CCriticalSection       G::m_criticalsection_LeftResultlist;	         //左产品结果临界
CCriticalSection       G::m_criticalsection_RightResultlist;        //右产品结果临界
//CCriticalSection       G::m_criticalsection_sizelist1tofile;		        //数据到文件列表1临界
//CCriticalSection       G::m_criticalsection_sizelist2tofile;		        //数据到文件列表2临界


////////////////////////////               全局变量
HTuple                     G::hv_AcqHandle1;                       //相机1
HTuple                     G::hv_AcqHandle2;                      //相机2
HTuple                     G::hv_AcqHandle3;                       //相机3
HTuple                     G::hv_AcqHandle4;                      //相机4
HTuple                     G::hv_AcqHandle5;                      //相机5
HTuple                     G::hv_AcqHandle6;                      //相机6
HTuple                     G::HW1;                                      //窗口1
HTuple                     G::HW2;                                      //窗口2
HTuple                     G::HW3;                                     //窗口3
HTuple                     G::HW4;                                    //窗口4
HTuple                     G::HW5;                                    //窗口5
HTuple                     G::HW6;                                    //窗口6

CString                    G::ParaFileName=L".//1269 Air Column para.ini";

BOOL                      G::bRunning;                              //运行中标志   
BOOL                      G::bResetOK;                              //复位完成
BOOL                      G::bCanGrab;                              //允许拍照
bool                        G::bAlarm;                                 //报警标志
bool                       G::bRunflag;                                  //自动流程中标志，用于限制单轴手动
bool                       G::bLeftRstOK;                     //左边轴复位完成
bool                       G::bRightRstOK;                      //右边轴复位完成
bool                       G::bLeftNoShield;                       //左端没有屏蔽，可以执行
bool                       G::bRightNoShield;                     //右端没有屏蔽，可以执行
BOOL                      G::bOpenExcelOk;                     //Excel打开成功

int                          G::UserAuthority;                             //当前登录用户权限

HTuple                    G::modle1;                                 //模板1，左尺寸模板

LONGLONG            G::GridCounts1;                         //已走的格数1，用于左相机触发计数
LONGLONG            G::GridCounts2;                        //已走的格数2，用于右相机触发计数

long                       G::grab1counts;                          //图像1抓图数量
long                       G::grab2counts;                          //图像2抓图数量
long                       G::grab3counts;                          //图像3抓图数量
long                       G::grab4counts;                          //图像4抓图数量
long                       G::grab5counts;                          //图像5抓图数量
long                       G::grab6counts;                          //图像6抓图数量
long                       G::grab1delete;                           //图像1删除数量
long                       G::grab2delete;                           //图像2删除数量
long                       G::triggercounts1;                        //触发次数1
long                       G::triggercounts2;                         //触发次数2
long                       G::blow1;                                     //吹产品次数1
long                       G::blow2;                                    //吹产品次数2
long                       G::LeftNullCounts;                 //左侧空产品数，用于计算单边屏蔽停止
long                       G::RightNullCounts;             //右侧空产品数，用于计算单边屏蔽停止
//long                       G::ToFileCounts1;                        //结果写入数据库计数1
//long                       G::ToFileCounts2;                        //结果写入数据库计数2

double                   G::LeftLength;                            //当前左产品总长
double                   G::LeftDiameter;                        //当前左产品外径
double                   G::RightLength;                          //当前右产品总长
double                   G::RightDiameter;                       //当前右产品外径

////////////////////////////////////////////////////////////////////////////////////////
CList<ImageLinked, ImageLinked&>G::Imagelist1;                               //图像列表1
CList<ImageLinked, ImageLinked&>G::Imagelist2;                               //图像列表2
CList<ImageLinked, ImageLinked&>G::Imagelist3;                               //图像列表3
CList<ImageLinked, ImageLinked&>G::Imagelist4;                               //图像列表4
CList<ImageLinked, ImageLinked&>G::Imagelist5;                               //图像列表5
CList<ImageLinked, ImageLinked&>G::Imagelist6;                               //图像列表6
//CList<Product, Product&>G::ProductList1;                              //产品列表1-左侧产品
//CList<Product, Product&>G::ProductList2;                              //产品列表2-右侧产品

CList<ProductPositive, ProductPositive&>G::LeftPositiveList;      //左产品正向列表
CList<ProductPositive, ProductPositive&>G::RightPositiveList;    //右产品正向特征列表
CList<ProductLateral, ProductLateral&>G::LeftLateralList1;      //左产品左侧列表
CList<ProductLateral, ProductLateral&>G::LeftLateralList2;     //左产品右侧列表
CList<ProductLateral, ProductLateral&>G::RightLateralList1;   //右产品左侧列表
CList<ProductLateral, ProductLateral&>G::RightLateralList2;  //右产品右侧列表
CList<DisposeResult, DisposeResult&>G::LeftResultlist;        //左产品检测结果列表
CList<DisposeResult, DisposeResult&>G::RightResultlist;        //右产品检测结果列表
//CList<CheckSize, CheckSize&>G::SizelistToFile1;                 //检测尺寸结果存文件链表1
//CList<CheckSize, CheckSize&>G::SizelistToFile2;                 //检测尺寸结果存文件链表2


G::G()
{
}


G::~G()
{
}



// Procedures 
// Chapter: Graphics / Text
// Short Description: This procedure writes a text message. 
void G::disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
	HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box)
{
	try
	{
	
	// Local iconic variables

	// Local control variables
	HTuple  hv_Red, hv_Green, hv_Blue, hv_Row1Part;
	HTuple  hv_Column1Part, hv_Row2Part, hv_Column2Part, hv_RowWin;
	HTuple  hv_ColumnWin, hv_WidthWin, hv_HeightWin, hv_MaxAscent;
	HTuple  hv_MaxDescent, hv_MaxWidth, hv_MaxHeight, hv_R1;
	HTuple  hv_C1, hv_FactorRow, hv_FactorColumn, hv_UseShadow;
	HTuple  hv_ShadowColor, hv_Exception, hv_Width, hv_Index;
	HTuple  hv_Ascent, hv_Descent, hv_W, hv_H, hv_FrameHeight;
	HTuple  hv_FrameWidth, hv_R2, hv_C2, hv_DrawMode, hv_CurrentColor;

	//This procedure displays text in a graphics window.
	//
	//Input parameters:
	//WindowHandle: The WindowHandle of the graphics window, where
	//   the message should be displayed
	//String: A tuple of strings containing the text message to be displayed
	//CoordSystem: If set to 'window', the text position is given
	//   with respect to the window coordinate system.
	//   If set to 'image', image coordinates are used.
	//   (This may be useful in zoomed images.)
	//Row: The row coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Column: The column coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Color: defines the color of the text as string.
	//   If set to [], '' or 'auto' the currently set color is used.
	//   If a tuple of strings is passed, the colors are used cyclically
	//   for each new textline.
	//Box: If Box[0] is set to 'true', the text is written within an orange box.
	//     If set to' false', no box is displayed.
	//     If set to a color string (e.g. 'white', '#FF00CC', etc.),
	//       the text is written in a box of that color.
	//     An optional second value for Box (Box[1]) controls if a shadow is displayed:
	//       'true' -> display a shadow in a default color
	//       'false' -> display no shadow (same as if no second value is given)
	//       otherwise -> use given string as color string for the shadow color
	//
	//Prepare window
	GetRgb(hv_WindowHandle, &hv_Red, &hv_Green, &hv_Blue);
	GetPart(hv_WindowHandle, &hv_Row1Part, &hv_Column1Part, &hv_Row2Part, &hv_Column2Part);
	GetWindowExtents(hv_WindowHandle, &hv_RowWin, &hv_ColumnWin, &hv_WidthWin, &hv_HeightWin);
	SetPart(hv_WindowHandle, 0, 0, hv_HeightWin - 1, hv_WidthWin - 1);
	//
	//default settings
	if (0 != (hv_Row == -1))
	{
		hv_Row = 12;
	}
	if (0 != (hv_Column == -1))
	{
		hv_Column = 12;
	}
	if (0 != (hv_Color == HTuple()))
	{
		hv_Color = "";
	}
	//
	hv_String = (("" + hv_String) + "").TupleSplit("\n");
	//
	//Estimate extentions of text depending on font size.
	GetFontExtents(hv_WindowHandle, &hv_MaxAscent, &hv_MaxDescent, &hv_MaxWidth, &hv_MaxHeight);
	if (0 != (hv_CoordSystem == HTuple("window")))
	{
		hv_R1 = hv_Row;
		hv_C1 = hv_Column;
	}
	else
	{
		//Transform image to window coordinates
		hv_FactorRow = (1.*hv_HeightWin) / ((hv_Row2Part - hv_Row1Part) + 1);
		hv_FactorColumn = (1.*hv_WidthWin) / ((hv_Column2Part - hv_Column1Part) + 1);
		hv_R1 = ((hv_Row - hv_Row1Part) + 0.5)*hv_FactorRow;
		hv_C1 = ((hv_Column - hv_Column1Part) + 0.5)*hv_FactorColumn;
	}
	//
	//Display text box depending on text size
	hv_UseShadow = 1;
	hv_ShadowColor = "gray";
	if (0 != (HTuple(hv_Box[0]) == HTuple("true")))
	{
		hv_Box[0] = "#fce9d4";
		hv_ShadowColor = "#f28d26";
	}
	if (0 != ((hv_Box.TupleLength())>1))
	{
		if (0 != (HTuple(hv_Box[1]) == HTuple("true")))
		{
			//Use default ShadowColor set above
		}
		else if (0 != (HTuple(hv_Box[1]) == HTuple("false")))
		{
			hv_UseShadow = 0;
		}
		else
		{
			hv_ShadowColor = ((const HTuple&)hv_Box)[1];
			//Valid color?
			try
			{
				SetColor(hv_WindowHandle, HTuple(hv_Box[1]));
			}
			// catch (Exception) 
			catch (HalconCpp::HException &HDevExpDefaultException)
			{
				HDevExpDefaultException.ToHTuple(&hv_Exception);
				hv_Exception = "Wrong value of control parameter Box[1] (must be a 'true', 'false', or a valid color string)";
				throw HalconCpp::HException(hv_Exception);
			}
		}
	}
	if (0 != (HTuple(hv_Box[0]) != HTuple("false")))
	{
		//Valid color?
		try
		{
			SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
		}
		// catch (Exception) 
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			hv_Exception = "Wrong value of control parameter Box[0] (must be a 'true', 'false', or a valid color string)";
			throw HalconCpp::HException(hv_Exception);
		}
		//Calculate box extents
		hv_String = (" " + hv_String) + " ";
		hv_Width = HTuple();
		{
			HTuple end_val93 = (hv_String.TupleLength()) - 1;
			HTuple step_val93 = 1;
			for (hv_Index = 0; hv_Index.Continue(end_val93, step_val93); hv_Index += step_val93)
			{
				GetStringExtents(hv_WindowHandle, HTuple(hv_String[hv_Index]), &hv_Ascent,
					&hv_Descent, &hv_W, &hv_H);
				hv_Width = hv_Width.TupleConcat(hv_W);
			}
		}
		hv_FrameHeight = hv_MaxHeight*(hv_String.TupleLength());
		hv_FrameWidth = (HTuple(0).TupleConcat(hv_Width)).TupleMax();
		hv_R2 = hv_R1 + hv_FrameHeight;
		hv_C2 = hv_C1 + hv_FrameWidth;
		//Display rectangles
		GetDraw(hv_WindowHandle, &hv_DrawMode);
		SetDraw(hv_WindowHandle, "fill");
		//Set shadow color
		SetColor(hv_WindowHandle, hv_ShadowColor);
		if (0 != hv_UseShadow)
		{
			DispRectangle1(hv_WindowHandle, hv_R1 + 1, hv_C1 + 1, hv_R2 + 1, hv_C2 + 1);
		}
		//Set box color
		SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
		DispRectangle1(hv_WindowHandle, hv_R1, hv_C1, hv_R2, hv_C2);
		SetDraw(hv_WindowHandle, hv_DrawMode);
	}
	//Write text.
  {
	  HTuple end_val115 = (hv_String.TupleLength()) - 1;
	  HTuple step_val115 = 1;
	  for (hv_Index = 0; hv_Index.Continue(end_val115, step_val115); hv_Index += step_val115)
	  {
		  hv_CurrentColor = ((const HTuple&)hv_Color)[hv_Index % (hv_Color.TupleLength())];
		  if (0 != (HTuple(hv_CurrentColor != HTuple("")).TupleAnd(hv_CurrentColor != HTuple("auto"))))
		  {
			  SetColor(hv_WindowHandle, hv_CurrentColor);
		  }
		  else
		  {
			  SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
		  }
		  hv_Row = hv_R1 + (hv_MaxHeight*hv_Index);
		  SetTposition(hv_WindowHandle, hv_Row, hv_C1);
		  WriteString(hv_WindowHandle, HTuple(hv_String[hv_Index]));
	  }
  }
  //Reset changed window settings
  SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
  SetPart(hv_WindowHandle, hv_Row1Part, hv_Column1Part, hv_Row2Part, hv_Column2Part);
  }
  catch(...)
  {
	  return;
  }
  return;
}


// Procedures 
// Chapter: Matching / Shape-Based
// Short Description: Display the results of Shape-Based Matching. 
void G::dev_display_shape_matching_results(HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row,
	HTuple hv_Column, HTuple hv_Angle, HTuple hv_ScaleR, HTuple hv_ScaleC, HTuple hv_Model, HTuple  WindowHandle)
{
	try
	{
		// Local iconic variables
		HObject  ho_ModelContours, ho_ContoursAffinTrans;

		// Local control variables
		HTuple  hv_NumMatches, hv_Index, hv_Match, hv_HomMat2DIdentity;
		HTuple  hv_HomMat2DScale, hv_HomMat2DRotate, hv_HomMat2DTranslate;

		//This procedure displays the results of Shape-Based Matching.
		//
		hv_NumMatches = hv_Row.TupleLength();
		if (0 != (hv_NumMatches > 0))
		{
			if (0 != ((hv_ScaleR.TupleLength()) == 1))
			{
				TupleGenConst(hv_NumMatches, hv_ScaleR, &hv_ScaleR);
			}
			if (0 != ((hv_ScaleC.TupleLength()) == 1))
			{
				TupleGenConst(hv_NumMatches, hv_ScaleC, &hv_ScaleC);
			}
			if (0 != ((hv_Model.TupleLength()) == 0))
			{
				TupleGenConst(hv_NumMatches, 0, &hv_Model);
			}
			else if (0 != ((hv_Model.TupleLength()) == 1))
			{
				TupleGenConst(hv_NumMatches, hv_Model, &hv_Model);
			}
	{
		HTuple end_val15 = (hv_ModelID.TupleLength()) - 1;
		HTuple step_val15 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val15, step_val15); hv_Index += step_val15)
		{
			GetShapeModelContours(&ho_ModelContours, HTuple(hv_ModelID[hv_Index]), 1);
			if (HDevWindowStack::IsOpen())
				SetColor(HDevWindowStack::GetActive(), HTuple(hv_Color[hv_Index % (hv_Color.TupleLength())]));
			{
				HTuple end_val18 = hv_NumMatches - 1;
				HTuple step_val18 = 1;
				for (hv_Match = 0; hv_Match.Continue(end_val18, step_val18); hv_Match += step_val18)
				{
					if (0 != (hv_Index == HTuple(hv_Model[hv_Match])))
					{
						HomMat2dIdentity(&hv_HomMat2DIdentity);
						HomMat2dScale(hv_HomMat2DIdentity, HTuple(hv_ScaleR[hv_Match]), HTuple(hv_ScaleC[hv_Match]),
							0, 0, &hv_HomMat2DScale);
						HomMat2dRotate(hv_HomMat2DScale, HTuple(hv_Angle[hv_Match]), 0, 0, &hv_HomMat2DRotate);
						HomMat2dTranslate(hv_HomMat2DRotate, HTuple(hv_Row[hv_Match]), HTuple(hv_Column[hv_Match]),
							&hv_HomMat2DTranslate);
						AffineTransContourXld(ho_ModelContours, &ho_ContoursAffinTrans, hv_HomMat2DTranslate);
						//if (HDevWindowStack::IsOpen())
						DispObj(ho_ContoursAffinTrans, WindowHandle);
					}
				}
			}
		}
	}
		}
		return;
	}
	catch (...)
	{
		return;
	}
	
}

// 轴回原点函数
//轴号，复位速度
int G::GoHome(int AxisNum,long  RstSpeed)
{
	if (MCard.m_bMotionCardOpenning == FALSE)
	{
		MCard.DebugInfo2(L"GoHome::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	dmc_set_home_pin_logic(0, AxisNum, 0, 0);
	//回原点模式，回零方向（0：负向；1：正向），
	//，回零速度模式（0:低速回零,以dmc_set_profile中起始速度运行,1:高速回零,以最大速度运行)
	dmc_set_homemode(0, AxisNum, 0, 1, 0, 1);          //设置回原点模式
	//脉冲输出方式    1低电平有效
	dmc_set_pulse_outmode(0, AxisNum, 1);     
	dmc_set_profile(0, AxisNum, 1000, RstSpeed, 0.1, 0.1, 500);
	dmc_home_move(0, AxisNum);
	return 0;
}


// 打开并初始化相机
int G::OpenCamera()
{
	try
	{
		HalconCpp::CloseAllFramegrabbers();

		///////////////////////////////////             相机1                      ////////////////////////////////////
		
		OpenFramegrabber("GigEVision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "C0", 0, -1, &G::hv_AcqHandle1);
		SetFramegrabberParam(G::hv_AcqHandle1, "GainRaw", (long)PR[30].paravalue);   //0
		//SetFramegrabberParam(Global::hv_AcqHandle1, "GammaEnable", 1);
		SetFramegrabberParam(G::hv_AcqHandle1, "grab_timeout", -1);
		SetFramegrabberParam(G::hv_AcqHandle1, "ExposureTimeAbs", PR[31].paravalue);                //3000
		//SetFramegrabberParam(Global::hv_AcqHandle1, "Gamma", 2);
		//SetFramegrabberParam(Global::hv_AcqHandle1, "DigitalShift", 2);
		SetFramegrabberParam(G::hv_AcqHandle1, "TriggerMode", "On");
		SetFramegrabberParam(G::hv_AcqHandle1, "TriggerActivation", "RisingEdge"); 
		//SetFramegrabberParam(Global::hv_AcqHandle1, "TriggerActivation", "FallingEdge");
		SetFramegrabberParam(G::hv_AcqHandle1, "OffsetX", 0);
		SetFramegrabberParam(G::hv_AcqHandle1, "OffsetY", 0);
		SetFramegrabberParam(G::hv_AcqHandle1, "Width", (long)PR[32].paravalue);              //1920
		SetFramegrabberParam(G::hv_AcqHandle1, "Height", (long)PR[33].paravalue);             //1200
		SetFramegrabberParam(G::hv_AcqHandle1, "TriggerSelector", "AcquisitionStart");
		SetFramegrabberParam(G::hv_AcqHandle1, "PixelFormat", "Mono8");
	}
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		AfxMessageBox(L"相机1打开失败！请检查连线！");
		return -1;
	}

	////////////////////////////////////         相机2              //////////////////////////////////////////////
	//try
	//{
	//	OpenFramegrabber("GigEVision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "C1", 0, -1, &G::hv_AcqHandle2);
	//	SetFramegrabberParam(G::hv_AcqHandle2, "GainRaw", PR[40].paravalue);
	//	SetFramegrabberParam(G::hv_AcqHandle2, "grab_timeout", -1);
	//	SetFramegrabberParam(G::hv_AcqHandle2, "ExposureTimeAbs", PR[41].paravalue);
	//	//SetFramegrabberParam(Global::hv_AcqHandle2, "TriggerActivation", "FallingEdge");
	//	SetFramegrabberParam(G::hv_AcqHandle2, "TriggerActivation", "RisingEdge");
	//	SetFramegrabberParam(G::hv_AcqHandle2, "OffsetX", 0);
	//	SetFramegrabberParam(G::hv_AcqHandle2, "OffsetY", 0);
	//	SetFramegrabberParam(G::hv_AcqHandle2, "Width", PR[42].paravalue);
	//	SetFramegrabberParam(G::hv_AcqHandle2, "Height", PR[43].paravalue);
	//	SetFramegrabberParam(G::hv_AcqHandle2, "TriggerSelector", "AcquisitionStart");
	//	SetFramegrabberParam(G::hv_AcqHandle2, "TriggerMode", "On");
	//	SetFramegrabberParam(G::hv_AcqHandle2, "PixelFormat", "Mono8");

	//}
	//catch (HalconCpp::HException &HDevExpDefaultException)
	//{
	//	AfxMessageBox(L"相机2打开失败！请检查连线！");
	//	return -1;
	//}

	////////////////////////////////////         相机3              //////////////////////////////////////////////
	try
	{
		//HalconCpp::CloseAllFramegrabbers();

		OpenFramegrabber("GigEVision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "C2", 0, -1, &G::hv_AcqHandle3);
		SetFramegrabberParam(G::hv_AcqHandle3, "GainRaw",  (long)PR[50].paravalue);              //0
		SetFramegrabberParam(G::hv_AcqHandle3, "grab_timeout", -1);
		SetFramegrabberParam(G::hv_AcqHandle3, "ExposureTimeAbs", PR[51].paravalue);            //1000
		//SetFramegrabberParam(Global::hv_AcqHandle2, "TriggerActivation", "FallingEdge");
		SetFramegrabberParam(G::hv_AcqHandle3, "TriggerMode", "On");
		SetFramegrabberParam(G::hv_AcqHandle3, "TriggerActivation", "RisingEdge");
		SetFramegrabberParam(G::hv_AcqHandle3, "OffsetX", 0);
		SetFramegrabberParam(G::hv_AcqHandle3, "OffsetY", 0);
		SetFramegrabberParam(G::hv_AcqHandle3, "Width",  (long)PR[52].paravalue);                 //1920
		SetFramegrabberParam(G::hv_AcqHandle3, "Height",  (long)PR[53].paravalue);                 //1200
		SetFramegrabberParam(G::hv_AcqHandle3, "TriggerSelector", "AcquisitionStart");
		SetFramegrabberParam(G::hv_AcqHandle3, "PixelFormat", "Mono8");

	}
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		AfxMessageBox(L"相机3打开失败！请检查连线！");
		return -1;
	}

	////////////////////////////////////         相机4              //////////////////////////////////////////////
	//try
	//{

	//	OpenFramegrabber("GigEVision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "C3", 0, -1, &G::hv_AcqHandle4);
	//	SetFramegrabberParam(G::hv_AcqHandle4, "GainRaw", PR[60].paravalue);
	//	SetFramegrabberParam(G::hv_AcqHandle4, "grab_timeout", -1);
	//	SetFramegrabberParam(G::hv_AcqHandle4, "ExposureTimeAbs", PR[61].paravalue);
	//	SetFramegrabberParam(G::hv_AcqHandle4, "TriggerActivation", "RisingEdge");
	//	SetFramegrabberParam(G::hv_AcqHandle4, "OffsetX", 0);
	//	SetFramegrabberParam(G::hv_AcqHandle4, "OffsetY", 0);
	//	SetFramegrabberParam(G::hv_AcqHandle4, "Width", PR[62].paravalue);
	//	SetFramegrabberParam(G::hv_AcqHandle4, "Height", PR[63].paravalue);
	//	SetFramegrabberParam(G::hv_AcqHandle4, "TriggerSelector", "AcquisitionStart");
	//	SetFramegrabberParam(G::hv_AcqHandle4, "TriggerMode", "On");
	//	SetFramegrabberParam(G::hv_AcqHandle4, "PixelFormat", "Mono8");

	//}
	//catch (HalconCpp::HException &HDevExpDefaultException)
	//{
	//	AfxMessageBox(L"相机4打开失败！请检查连线！");
	//	return -1;
	//}

	return 0;
}


// 初始化参数
BOOL G::InitParam()
{
	///////////////////////////////////////          输入点位              ////////////////////////////////////
	MCard.I[0].lpwstrPortName = L"[0]急停按钮";                        MCard.I[0].sValue = 0;
	MCard.I[1].lpwstrPortName = L"[1]启动按钮";                        MCard.I[1].sValue = 0;
	MCard.I[2].lpwstrPortName = L"[2]备用";		                        MCard.I[2].sValue = 0;
	MCard.I[3].lpwstrPortName = L"[3]满料光纤1";		                MCard.I[3].sValue = 0;
	MCard.I[4].lpwstrPortName = L"[4]半料光纤1";		                MCard.I[4].sValue = 0;
	MCard.I[5].lpwstrPortName = L"[5]满料光纤2";		                MCard.I[5].sValue = 0;
	MCard.I[6].lpwstrPortName = L"[6]半料光纤2";		                MCard.I[6].sValue = 0;
	MCard.I[7].lpwstrPortName = L"[7]备用";		                        MCard.I[7].sValue = 0;
	MCard.I[8].lpwstrPortName = L"[8]备用"; 		                        MCard.I[8].sValue = 0;
	MCard.I[9].lpwstrPortName = L"[9]备用";		                        MCard.I[9].sValue = 0;
	MCard.I[10].lpwstrPortName = L"[10]备用";		                    MCard.I[10].sValue = 0;
	MCard.I[11].lpwstrPortName = L"[11]备用";		                    MCard.I[11].sValue = 0;
	MCard.I[12].lpwstrPortName = L"[12]备用";		                    MCard.I[12].sValue = 0;
	MCard.I[13].lpwstrPortName = L"[13]备用";		                    MCard.I[13].sValue = 0;
	MCard.I[14].lpwstrPortName = L"[14]备用";		                    MCard.I[14].sValue = 0;
	MCard.I[15].lpwstrPortName = L"[15]备用";		                    MCard.I[15].sValue = 0;

	/////////////////////////////////////            输出点位              /////////////////////////////////////
	MCard.O[0].lpwstrPortName = L"[0]三色灯红灯";                    MCard.O[0].sValue = 0;
	MCard.O[1].lpwstrPortName = L"[1]三色灯绿灯";		             MCard.O[1].sValue = 0;
	MCard.O[2].lpwstrPortName = L"[2]三色灯黄灯 ";		             MCard.O[2].sValue = 0;
	MCard.O[3].lpwstrPortName = L"[3]蜂鸣器";		                    MCard.O[3].sValue = 0;
	MCard.O[4].lpwstrPortName = L"[4]NG电磁阀1";	        	    MCard.O[4].sValue = 0;
	MCard.O[5].lpwstrPortName = L"[5]NG电磁阀2";	        	    MCard.O[5].sValue = 0;
	MCard.O[6].lpwstrPortName = L"[6]备用";	        	                MCard.O[6].sValue = 0;
	MCard.O[7].lpwstrPortName = L"[7]备用";	        	                MCard.O[7].sValue = 0;
	MCard.O[8].lpwstrPortName = L"[8]备用";	        	                MCard.O[8].sValue = 0;
	MCard.O[9].lpwstrPortName = L"[9]备用";	        	                MCard.O[9].sValue = 0;
	MCard.O[10].lpwstrPortName = L"[10]备用";		                    MCard.O[10].sValue = 0;
	MCard.O[11].lpwstrPortName = L"[11]备用";		                    MCard.O[11].sValue = 0;
	MCard.O[12].lpwstrPortName = L"[12]备用";		                    MCard.O[12].sValue = 0;
	MCard.O[13].lpwstrPortName = L"[13]备用";		                    MCard.O[13].sValue = 0;
	MCard.O[14].lpwstrPortName = L"[14]左相机触发";		        MCard.O[14].sValue = 0;
	MCard.O[15].lpwstrPortName = L"[15]右相机触发"	;		        MCard.O[15].sValue = 0;

	///////////////////////////////////         全局参数                    ////////////////////////////////////////
	///////////////////////运动参数
	G::MotionUsed = 0;        //使用个数清零
	PR[0].type = L"运动参数";       PR[0].paraname = L"左产品伺服复位速度";       PR[0].paraMinValue = 1000;
	PR[0].paraMaxValue = 60000;          PR[0].paravalue = 25000;      PR[0].remark = L"单位：pulse/s";
	G::MotionUsed++;

	PR[1].type = L"运动参数";       PR[1].paraname = L"左产品伺服自动运行速度";       PR[1].paraMinValue = 1000;
	PR[1].paraMaxValue = 60000;          PR[1].paravalue = 25000;      PR[1].remark = L"单位：pulse/s";
	G::MotionUsed++;
 
	PR[2].type = L"运动参数";       PR[2].paraname = L"左产品初始拍照位置";       PR[2].paraMinValue = 1000;
	PR[2].paraMaxValue = 30000;          PR[2].paravalue = 10000;      PR[2].remark = L"单位：pulse";
	G::MotionUsed++;

	PR[3].type = L"运动参数";       PR[3].paraname = L"左产品拍照间隔距离";       PR[3].paraMinValue = 1000;
	PR[3].paraMaxValue = 1000000;          PR[3].paravalue = 280000/72;      PR[3].remark = L"单位：pulse";
	G::MotionUsed++;

	PR[4].type = L"运动参数";       PR[4].paraname = L"左产品OK吹气位置";       PR[4].paraMinValue = 0;
	PR[4].paraMaxValue = 300000;          PR[4].paravalue = 10000;      PR[4].remark = L"单位：pulse";
	G::MotionUsed++;

	PR[5].type = L"运动参数";       PR[5].paraname = L"左产品NG吹气位置";       PR[5].paraMinValue = 0;
	PR[5].paraMaxValue = 100000;          PR[5].paravalue = 280000 / 72;      PR[5].remark = L"单位：pulse";
	G::MotionUsed++;
	/*PR[4].type = L"运动参数";       PR[4].paraname = L"右相机初始拍照位置";       PR[4].paraMinValue = 1000;
	PR[4].paraMaxValue = 30000;          PR[4].paravalue = 10000;      PR[4].remark = L"单位：pulse";
	G::MotionUsed++;

	PR[5].type = L"运动参数";       PR[5].paraname = L"右相机拍照间隔距离";       PR[5].paraMinValue = 1000;
	PR[5].paraMaxValue = 10000;          PR[5].paravalue = 280000 / 72;      PR[5].remark = L"单位：pulse";
	G::MotionUsed++;*/

	PR[6].type = L"运动参数";       PR[6].paraname = L"右产品伺服复位速度";       PR[6].paraMinValue = 100;
	PR[6].paraMaxValue = 300000;          PR[6].paravalue = 10000;      PR[6].remark = L"单位：pulse";
	G::MotionUsed++;

	PR[7].type = L"运动参数";       PR[7].paraname = L"右产品伺服自动运行速度";       PR[7].paraMinValue = 100;
	PR[7].paraMaxValue = 100000;          PR[7].paravalue = 20000;      PR[7].remark = L"单位：pulse";
	G::MotionUsed++;

	PR[8].type = L"运动参数";       PR[8].paraname = L"右产品初始拍照位置";       PR[8].paraMinValue = 0;
	PR[8].paraMaxValue = 30000;          PR[8].paravalue = 10000;      PR[8].remark = L"单位：pulse";
	G::MotionUsed++;

	PR[9].type = L"运动参数";       PR[9].paraname = L"右产品拍照间隔距离";       PR[9].paraMinValue = 0;
	PR[9].paraMaxValue = 10000;          PR[9].paravalue = 2000;      PR[9].remark = L"单位：pulse";
	G::MotionUsed++;

	PR[10].type = L"运动参数";       PR[10].paraname = L"右产品OK吹气位置";       PR[10].paraMinValue = 0;
	PR[10].paraMaxValue = 300000;          PR[10].paravalue = 10000;      PR[10].remark = L"单位：pulse";
	G::MotionUsed++;

	PR[11].type = L"运动参数";       PR[11].paraname = L"右相机NG吹气位置";       PR[11].paraMinValue = 0;
	PR[11].paraMaxValue = 100000;          PR[11].paravalue = 2000;      PR[11].remark = L"单位：pulse";
	G::MotionUsed++;

	PR[MotionUsed].type = L"运动参数";       PR[MotionUsed].paraname = L"整机复位超时时间";       PR[MotionUsed].paraMinValue = 0;
	PR[MotionUsed].paraMaxValue = 100000;          PR[MotionUsed].paravalue = 6000;      PR[MotionUsed].remark = L"单位：ms";
	G::MotionUsed++;

	//13
	PR[MotionUsed].type = L"运动参数";       PR[MotionUsed].paraname = L"左侧连续空料屏蔽数量";       PR[MotionUsed].paraMinValue = 3;
	PR[MotionUsed].paraMaxValue = 1000;          PR[MotionUsed].paravalue = 5;      PR[MotionUsed].remark = L"单位：PCS";
	G::MotionUsed++;

	//14
	PR[MotionUsed].type = L"运动参数";       PR[MotionUsed].paraname = L"右侧连续空料屏蔽数量";       PR[MotionUsed].paraMinValue = 3;
	PR[MotionUsed].paraMaxValue = 1000;          PR[MotionUsed].paravalue = 5;      PR[MotionUsed].remark = L"单位：PCS";
	G::MotionUsed++;

	//备用
	for (int i = G::MotionUsed; i < 30; i++)
	{
		PR[i].type = L"运动参数";       PR[i].paraname = L"备用";       PR[i].paraMinValue = 0;
		PR[i].paraMaxValue = 50000;       
	}


	////////////////////////////////////////////////////////////相机参数
	//相机1参数
	G::Camera1Used = 0;               //相机1使用参数清零
	PR[30].type = L"相机1参数";        PR[30].paraname = L"相机1增益";       PR[30].paraMinValue = 0;
	PR[30].paraMaxValue = 3000;     PR[30].paravalue = 0;               PR[30].remark = L"单位：";
	G::Camera1Used++;

	PR[31].type = L"相机1参数";        PR[31].paraname = L"相机1曝光时间";       PR[31].paraMinValue = 0;
	PR[31].paraMaxValue = 3000;     PR[31].paravalue = 3000;               PR[31].remark = L"单位：";
	G::Camera1Used++;

	PR[32].type = L"相机1参数";        PR[32].paraname = L"相机1视野宽";       PR[32].paraMinValue = 0;
	PR[32].paraMaxValue = 3000;     PR[32].paravalue = 1920;               PR[32].remark = L"单位：像素";
	G::Camera1Used++;

	PR[33].type = L"相机1参数";        PR[33].paraname = L"相机1视野高";       PR[33].paraMinValue = 0;
	PR[33].paraMaxValue = 3000;     PR[33].paravalue = 1200;               PR[33].remark = L"单位：像素";
	G::Camera1Used++;

	//相机1备用
	for (int i = 30+G::Camera1Used; i < 40; i++)
	{
		PR[i].type = L"相机1参数";      PR[i].paraname = L"备用";
	}


	//相机2参数
	G::Camera2Used = 0;
	PR[40].type = L"相机2参数";        PR[40].paraname = L"相机2增益";       PR[40].paraMinValue = 0;
	PR[40].paraMaxValue = 3000;     PR[40].paravalue = 300;               PR[40].remark = L"单位：";
	G::Camera2Used++;

	PR[41].type = L"相机2参数";        PR[41].paraname = L"相机2曝光时间";       PR[41].paraMinValue = 0;
	PR[41].paraMaxValue = 3000;     PR[41].paravalue = 300;               PR[41].remark = L"单位：";
	G::Camera2Used++;

	PR[42].type = L"相机2参数";        PR[42].paraname = L"相机2视野长";       PR[42].paraMinValue = 0;
	PR[42].paraMaxValue = 3000;     PR[42].paravalue = 1920;               PR[42].remark = L"单位：像素";
	G::Camera2Used++;

	PR[43].type = L"相机2参数";        PR[43].paraname = L"相机2视野宽";       PR[43].paraMinValue = 0;
	PR[43].paraMaxValue = 3000;     PR[43].paravalue = 1920;               PR[43].remark = L"单位：像素";
	G::Camera2Used++;
	
	//相机2备用
	for (int i = 40+G::Camera2Used; i < 50; i++)
	{
		PR[i].type = L"相机2参数";      PR[i].paraname = L"备用";
	}


	//相机3参数
	G::Camera3Used = 0;
	PR[50].type = L"相机3参数";        PR[50].paraname = L"相机3增益";       PR[50].paraMinValue = 0;
	PR[50].paraMaxValue = 3000;     PR[50].paravalue = 0;               PR[50].remark = L"单位：";
	G::Camera3Used++;

	PR[51].type = L"相机3参数";        PR[51].paraname = L"相机3曝光时间";       PR[51].paraMinValue = 0;
	PR[51].paraMaxValue = 3000;     PR[51].paravalue = 3000;               PR[51].remark = L"单位：";
	G::Camera3Used++;

	PR[52].type = L"相机3参数";        PR[52].paraname = L"相机3视野长";       PR[52].paraMinValue = 0;
	PR[52].paraMaxValue = 3000;     PR[52].paravalue = 1920;               PR[52].remark = L"单位：像素";
	G::Camera3Used++;

	PR[53].type = L"相机3参数";        PR[53].paraname = L"相机3视野宽";       PR[53].paraMinValue = 0;
	PR[53].paraMaxValue = 3000;     PR[53].paravalue = 1200;               PR[53].remark = L"单位：像素";
	G::Camera3Used++;
	
	//相机3备用
	for (int i = 50+G::Camera3Used; i < 60; i++)
	{
		PR[i].type = L"相机3参数";      PR[i].paraname = L"备用";
	}


	//相机4参数
	G::Camera4Used = 0;
	PR[60].type = L"相机4参数";        PR[60].paraname = L"相机4增益";       PR[60].paraMinValue = 0;
	PR[60].paraMaxValue = 3000;     PR[60].paravalue = 300;               PR[60].remark = L"单位：";
	G::Camera4Used++;

	PR[61].type = L"相机4参数";        PR[61].paraname = L"相机4曝光时间";       PR[61].paraMinValue = 0;
	PR[61].paraMaxValue = 3000;     PR[61].paravalue = 300;               PR[61].remark = L"单位：";
	G::Camera4Used++;

	PR[62].type = L"相机4参数";        PR[62].paraname = L"相机4视野长";       PR[62].paraMinValue = 0;
	PR[62].paraMaxValue = 3000;     PR[62].paravalue = 1920;               PR[62].remark = L"单位：像素";
	G::Camera4Used++;

	PR[63].type = L"相机4参数";        PR[63].paraname = L"相机4视野宽";       PR[63].paraMinValue = 0;
	PR[63].paraMaxValue = 3000;     PR[63].paravalue = 1920;               PR[63].remark = L"单位：像素";
	G::Camera4Used++;
	
	//相机4备用
	for (int i = 60+G::Camera4Used; i < 70; i++)
	{
		PR[i].type = L"相机4参数";      PR[i].paraname = L"备用";
	}


	//相机5参数
	G::Camera5Used = 0;                       //使用个数清零
	PR[70].type = L"相机5参数";        PR[70].paraname = L"相机5增益";       PR[70].paraMinValue = 0;
	PR[70].paraMaxValue = 3000;     PR[70].paravalue = 300;               PR[70].remark = L"单位：";
	G::Camera5Used++;

	PR[71].type = L"相机5参数";        PR[71].paraname = L"相机5曝光时间";       PR[71].paraMinValue = 0;
	PR[71].paraMaxValue = 3000;     PR[71].paravalue = 300;               PR[71].remark = L"单位：";
	G::Camera5Used++;

	PR[72].type = L"相机5参数";        PR[72].paraname = L"相机5视野长";       PR[72].paraMinValue = 0;
	PR[72].paraMaxValue = 3000;     PR[72].paravalue = 1920;               PR[72].remark = L"单位：像素";
	G::Camera5Used++;

	PR[73].type = L"相机5参数";        PR[73].paraname = L"相机5视野宽";       PR[73].paraMinValue = 0;
	PR[73].paraMaxValue = 3000;     PR[73].paravalue = 1920;               PR[73].remark = L"单位：像素";
	G::Camera5Used++;

	//相机5备用
	for (int i = 70 + G::Camera5Used; i < 80; i++)
	{
		PR[i].type = L"相机5参数";      PR[i].paraname = L"备用";
	}

	//相机6参数
	G::Camera6Used = 0;
	PR[80].type = L"相机6参数";        PR[80].paraname = L"相机6增益";       PR[80].paraMinValue = 0;
	PR[80].paraMaxValue = 3000;     PR[80].paravalue = 300;               PR[80].remark = L"单位：";
	G::Camera6Used++;

	PR[81].type = L"相机6参数";        PR[81].paraname = L"相机6曝光时间";       PR[81].paraMinValue = 0;
	PR[81].paraMaxValue = 3000;     PR[81].paravalue = 300;               PR[81].remark = L"单位：";
	G::Camera6Used++;

	PR[82].type = L"相机6参数";        PR[82].paraname = L"相机6视野长";       PR[82].paraMinValue = 0;
	PR[82].paraMaxValue = 3000;     PR[82].paravalue = 1920;               PR[82].remark = L"单位：像素";
	G::Camera6Used++;

	PR[83].type = L"相机6参数";        PR[83].paraname = L"相机6视野宽";       PR[83].paraMinValue = 0;
	PR[83].paraMaxValue = 3000;     PR[83].paravalue = 1920;               PR[83].remark = L"单位：像素";
	G::Camera6Used++;

	//相机6备用
	for (int i = 80 + G::Camera6Used; i < 90; i++)
	{
		PR[i].type = L"相机6参数";      PR[i].paraname = L"备用";
	}


	///////////////////////////////////////////////////////////////视觉参数
	//视觉1（左边尺寸检测）
	G::Vision1Used = 0;
	PR[90].type = L"视觉1参数";      PR[90].paraname = L"视觉1分割阈值低值";      PR[90].paraMinValue = 0;
	PR[90].paraMaxValue = 255;    PR[90].paravalue = 0;                PR[90].remark = L"单位：";
	G::Vision1Used++;

	PR[91].type = L"视觉1参数";      PR[91].paraname = L"视觉1分割阈值高值";      PR[91].paraMinValue = 0;
	PR[91].paraMaxValue = 255;    PR[91].paravalue = 200;                PR[91].remark = L"单位：";
	G::Vision1Used++;	

	PR[92].type = L"视觉1参数";      PR[92].paraname = L"左产品总长OK最小尺寸";      PR[92].paraMinValue = 10;
	PR[92].paraMaxValue = 20;    PR[92].paravalue = 14;                PR[92].remark = L"单位：mm";
	G::Vision1Used++;

	PR[93].type = L"视觉1参数";      PR[93].paraname = L"左产品总长OK最大尺寸";      PR[93].paraMinValue = 10;
	PR[93].paraMaxValue = 20;    PR[93].paravalue = 16;                PR[93].remark = L"单位：mm";
	G::Vision1Used++;

	PR[94].type = L"视觉1参数";      PR[94].paraname = L"左产品外径OK最小尺寸";      PR[94].paraMinValue = 2.2;
	PR[94].paraMaxValue = 5;    PR[94].paravalue = 1;                PR[94].remark = L"单位：mm";
	G::Vision1Used++;

	PR[95].type = L"视觉1参数";      PR[95].paraname = L"左产品外径OK最大尺寸";      PR[95].paraMinValue = 2.6;
	PR[95].paraMaxValue = 5;    PR[95].paravalue = 1;                PR[95].remark = L"单位：mm";
	G::Vision1Used++;

	//视觉1备用
	for (int i = 90+G::Vision1Used; i < 100; i++)
	{
		PR[i].type = L"视觉1参数";      PR[i].paraname = L"备用";
	}

	//视觉2（左相机左端外观）
	G::Vision2Used = 0;
	PR[100].type = L"视觉2参数";      PR[100].paraname = L"视觉2分割阈值低值";      PR[100].paraMinValue = 0;
	PR[100].paraMaxValue = 255;    PR[100].paravalue = 0;                PR[100].remark = L"单位：";
	G::Vision2Used++;

	PR[101].type = L"视觉2参数";      PR[101].paraname = L"视觉2分割阈值高值";      PR[101].paraMinValue = 0;
	PR[101].paraMaxValue = 255;    PR[101].paravalue = 200;                PR[101].remark = L"单位：";
	G::Vision2Used++;

	//视觉2备用
	for (int i = 100+G::Vision2Used; i < 110; i++)
	{
		PR[i].type = L"视觉2参数";      PR[i].paraname = L"备用";
	}

	//视觉3（左相机-右端外观）
	G::Vision3Used = 0;
	PR[110].type = L"视觉3参数";      PR[110].paraname = L"视觉3分割阈值低值";      PR[110].paraMinValue = 0;
	PR[110].paraMaxValue = 255;    PR[110].paravalue = 0;                PR[110].remark = L"单位：";
	G::Vision3Used++;

	PR[111].type = L"视觉3参数";      PR[111].paraname = L"视觉3分割阈值高值";      PR[111].paraMinValue = 0;
	PR[111].paraMaxValue = 255;    PR[111].paravalue = 200;                PR[111].remark = L"单位：";
	G::Vision3Used++;

	//视觉3备用
	for (int i = 110+G::Vision3Used; i < 120; i++)
	{
		PR[i].type = L"视觉3参数";      PR[i].paraname = L"备用";
	}

	//视觉4参数（右相机-尺寸检测）
	G::Vision4Used = 0;
	PR[120].type = L"视觉4参数";     PR[120].paraname = L"视觉4分割阈值低值";      PR[120].paraMinValue = 0;
	PR[120].paraMaxValue = 255;    PR[120].paravalue = 0;                PR[120].remark = L"单位：";
	G::Vision4Used++;

	PR[121].type = L"视觉4参数";      PR[121].paraname = L"视觉4分割阈值高值";      PR[121].paraMinValue = 0;
	PR[121].paraMaxValue = 255;    PR[121].paravalue = 200;                PR[121].remark = L"单位：";
	G::Vision4Used++;

	PR[122].type = L"视觉4参数";      PR[122].paraname = L"右产品总长OK最小尺寸";      PR[122].paraMinValue = 10;
	PR[122].paraMaxValue = 255;    PR[122].paravalue = 200;                PR[122].remark = L"单位：";
	G::Vision4Used++;

	PR[123].type = L"视觉4参数";      PR[123].paraname = L"右产品总长OK最大尺寸";      PR[123].paraMinValue = 10;
	PR[123].paraMaxValue = 255;    PR[123].paravalue = 200;                PR[123].remark = L"单位：";
	G::Vision4Used++;

	PR[124].type = L"视觉4参数";      PR[124].paraname = L"右产品外径OK最小尺寸";      PR[124].paraMinValue = 10;
	PR[124].paraMaxValue = 255;    PR[124].paravalue = 200;                PR[124].remark = L"单位：";
	G::Vision4Used++;

	PR[125].type = L"视觉4参数";      PR[125].paraname = L"右产品外径OK最大尺寸";      PR[125].paraMinValue = 10;
	PR[125].paraMaxValue = 255;    PR[125].paravalue = 200;                PR[125].remark = L"单位：";
	G::Vision4Used++;

	//视觉4备用
	for (int i = G::Vision4Used+120; i < 130; i++)
	{
		PR[i].type = L"视觉4参数";      PR[i].paraname = L"备用";
	}


	//视觉5（右相机-左端外观）
	G::Vision5Used = 0;
	PR[130].type = L"视觉5参数";      PR[130].paraname = L"视觉5分割阈值低值";      PR[130].paraMinValue = 0;
	PR[130].paraMaxValue = 255;    PR[130].paravalue = 0;                PR[130].remark = L"单位：";
	G::Vision5Used++;

	PR[131].type = L"视觉5参数";      PR[131].paraname = L"视觉5分割阈值高值";      PR[131].paraMinValue = 0;
	PR[131].paraMaxValue = 255;    PR[131].paravalue = 200;                PR[131].remark = L"单位：";
	G::Vision5Used++;

	//视觉3备用
	for (int i = 130 + G::Vision5Used; i < 140; i++)
	{
		PR[i].type = L"视觉5参数";      PR[i].paraname = L"备用";
	}


	//视觉6参数（右相机-右端外观）
	G::Vision6Used = 0;
	PR[140].type = L"视觉6参数";     PR[140].paraname = L"视觉6分割阈值低值";      PR[140].paraMinValue = 0;
	PR[140].paraMaxValue = 255;    PR[140].paravalue = 0;                PR[140].remark = L"单位：";
	G::Vision6Used++;

	PR[141].type = L"视觉6参数";      PR[141].paraname = L"视觉6分割阈值高值";      PR[141].paraMinValue = 0;
	PR[141].paraMaxValue = 255;    PR[141].paravalue = 200;                PR[141].remark = L"单位：";
	G::Vision6Used++;

	//视觉4备用
	for (int i = G::Vision6Used + 140; i < 150; i++)
	{
		PR[i].type = L"视觉6参数";      PR[i].paraname = L"备用";
	}

	///////////////生产相关数据
	PR[150].type = L"生产数据";		PR[150].paraname = L"总产量";       PR[150].paravalue = 100;
	PR[151].type = L"生产数据";       PR[151].paraname = L"OK数";         PR[151].paravalue = 0;
	PR[152].type = L"生产数据";       PR[152].paraname = L"NG数";         PR[152].paravalue = 0;
	PR[153].type = L"生产数据";       PR[153].paraname = L"良率";           PR[153].paravalue = 100;
	PR[154].type = L"生产数据";       PR[154].paraname = L"CT";             PR[154].paravalue = 0;
	PR[155].type = L"生产数据";       PR[155].paraname = L"UPH";          PR[155].paravalue = 0;

	return 0;
}


// 保存通用参数（初次写入）
int G::WriteCommonPara()
{
	try
	{
		/*G::db.m_pRecordset->Open("SELECT  *  FROM " + (_bstr_t)ParamTableName, G::db.m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockPessimistic, adCmdText);*/
		for (int i = 0; i < 150; i++)
		{
			/*if (G::PR[i].paraname != L"备用")
			{*/
			try
			{
				G::db.m_pRecordset->AddNew();
				G::db.m_pRecordset->PutCollect(L"序号", _variant_t(G::double_string(i + 1)));
				G::db.m_pRecordset->PutCollect(L"类型", _variant_t(G::PR[i].type));
				G::db.m_pRecordset->PutCollect(L"参数名", _variant_t(G::PR[i].paraname));
				G::db.m_pRecordset->PutCollect(L"参数值", _variant_t(G::double_string(G::PR[i].paravalue)));
				G::db.m_pRecordset->PutCollect(L"最大值", _variant_t(G::double_string(G::PR[i].paraMaxValue)));
				G::db.m_pRecordset->PutCollect(L"最小值", _variant_t(G::double_string(G::PR[i].paraMinValue)));
				G::db.m_pRecordset->PutCollect(L"备注", _variant_t(G::PR[i].remark));
				G::db.m_pRecordset->Update();
			}
			catch (_com_error a)
			{
				CString  err;
				err = a.ErrorMessage();
				AfxMessageBox(err + L"失败");
			}
			//}        //if

		}         //for
	}
	catch (_com_error c)
	{
		CString  err;
		err = c.ErrorMessage();
		AfxMessageBox(err + L"失败");
		return -1;
	}
	//CString		str;
	//CString    str2 = L"参数名";
	//CString    str3 = L"参数值";
	//CString    str4 = L"参数最大值";
	//CString    str5 = L"参数最小值";
	//CString    str6 = L"备注";


	//try
	//{

	//	/////////////////////////////////////    写入运动参数    ////////////////////////////////////
	//	for (int i = 0; i < 30; i++)
	//	{
	//		int j = i;
	//		//参数名
	//		str2.Format(L"参数名%d", i);
	//		::WritePrivateProfileString(L"运动参数", str2, G::PR[j].paraname, G::ParaFileName);
	//		//参数值
	//		str3.Format(L"参数值%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paravalue);
	//		::WritePrivateProfileString(L"运动参数", str3, str, G::ParaFileName);
	//		//最大值
	//		str4.Format(L"最大值%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMaxValue);
	//		::WritePrivateProfileString(L"运动参数", str4, str, G::ParaFileName);
	//		//最小值
	//		str5.Format(L"最小值%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMinValue);
	//		::WritePrivateProfileString(L"运动参数", str5, str, G::ParaFileName);
	//		//备注
	//		str6.Format(L"备注%d", i);
	//		::WritePrivateProfileString(L"运动参数", str6, G::PR[j].remark, G::ParaFileName);
	//	}

	//	////////////////////////////    写入相机参数       ////////////////////////////////////////
	//	for (int i = 0; i < 60; i++)
	//	{
	//		int j = i + 30;
	//		//参数名
	//		str2.Format(L"参数名%d", i);
	//		::WritePrivateProfileString(L"相机参数", str2, G::PR[j].paraname, G::ParaFileName);
	//		//参数值
	//		str3.Format(L"参数值%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paravalue);
	//		::WritePrivateProfileString(L"相机参数", str3, str, G::ParaFileName);
	//		//最大值
	//		str4.Format(L"最大值%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMaxValue);
	//		::WritePrivateProfileString(L"相机参数", str4, str, G::ParaFileName);
	//		//最小值
	//		str5.Format(L"最小值%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMinValue);
	//		::WritePrivateProfileString(L"相机参数", str5, str, G::ParaFileName);
	//		//备注
	//		str6.Format(L"备注%d", i);
	//		::WritePrivateProfileString(L"相机参数", str6, G::PR[j].remark, G::ParaFileName);
	//	}

	//	//////////////////////////////      写入视觉参数       ////////////////////////////////////
	//	for (int i = 0; i < 60; i++)
	//	{
	//		int j = i + 90;
	//		//参数名
	//		str2.Format(L"参数名%d", i);
	//		::WritePrivateProfileString(L"视觉参数", str2, G::PR[j].paraname, G::ParaFileName);
	//		//参数值
	//		str3.Format(L"参数值%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paravalue);
	//		::WritePrivateProfileString(L"视觉参数", str3, str, G::ParaFileName);
	//		//最大值
	//		str4.Format(L"最大值%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMaxValue);
	//		::WritePrivateProfileString(L"视觉参数", str4, str, G::ParaFileName);
	//		//最小值
	//		str5.Format(L"最小值%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMinValue);
	//		::WritePrivateProfileString(L"视觉参数", str5, str, G::ParaFileName);
	//		//备注
	//		str6.Format(L"备注%d", i);
	//		::WritePrivateProfileString(L"视觉参数", str6, G::PR[j].remark, G::ParaFileName);
	//	}
	//	
	//	//AfxMessageBox(L"参数保存成功！");
	//}
	//catch (...)
	//{
	//	AfxMessageBox(L"通用参数保存失败!");
	//	return -1;
	//}
	return 0;
}


// 保存IO端口名
int G::WriteIOPara()
{
	CString    str;
	try
	{
		////////////////////////////////////      写入输入功能口名       /////////////////////////////////
		for (int i = 0; i < 16; i++)
		{
			str.Format(L"%d号输入口", i);
			::WritePrivateProfileString(L"InputPort", str, G::MCard.I[i].lpwstrPortName, G::ParaFileName);
		}
		/////////////////////////////////        写入输出功能口名       /////////////////////////////////
		for (int i = 0; i < 16; i++)
		{
			str.Format(L"%d号输出口", i);
			::WritePrivateProfileString(L"OutPort", str, G::MCard.O[i].lpwstrPortName, G::ParaFileName);
		}
	}
	catch (...)
	{
		AfxMessageBox(L"IO功能名保存失败!");
		return -1;
	}
	return 0;
}

//相机触发
int G::CameraTrigger(int CameraNo)
{
	switch (CameraNo)
	{
	case 1:
		//G::MCard.SetDoBit(TRIGGER1, 0);
		//Sleep(2);
		//G::MCard.SetDoBit(TRIGGER1, 1);
		G::MCard.SetDoBitReverse(TRIGGER1,1,20);
		break;
	case 2:
		//G::MCard.SetDoBit(TRIGGER2, 0);
		//Sleep(2);
		//G::MCard.SetDoBit(TRIGGER2, 1);
		G::MCard.SetDoBitReverse(TRIGGER2,1,20);
		break;
	default:
		AfxMessageBox(L"请输入正确的相机编号！");
		break;
	}
	return 0;
}


// 写入生产信息
int G::WriteProduction()
{
	try
	{
		////////////////////////////////    写入生产数据          ///////////////////////////////////
		for (int i = 0; i < 6; i++)
		{
			int j = i + PI_TotalCounts;
			CString  str;
			str.Format(L"%.2lf", G::PR[j].paravalue);
			::WritePrivateProfileString(L"生产数据", G::PR[j].paraname, str, G::ParaFileName);
		}
	}
	catch (...)
	{
		AfxMessageBox(L"生产数据写入失败!");
		return -1;
	}
	return 0;
}


// 删除链表内容
int G::ClearLinked()
{
	if (G::Imagelist1.GetCount() > 0)
		G::Imagelist1.RemoveAll();
	
	if (G::Imagelist2.GetCount() > 0)
		G::Imagelist2.RemoveAll();
	
	if (G::Imagelist3.GetCount() > 0)
		G::Imagelist3.RemoveAll();
	
	if (G::Imagelist4.GetCount() > 0)
		G::Imagelist4.RemoveAll();

	if (G::Imagelist5.GetCount() > 0)
		G::Imagelist5.RemoveAll();

	if (G::Imagelist6.GetCount() > 0)
		G::Imagelist6.RemoveAll();
	
	if (G::LeftPositiveList.GetCount() > 0)
		G::LeftPositiveList.RemoveAll();
	
	if (G::RightPositiveList.GetCount() > 0)
		G::RightPositiveList.RemoveAll();

	if (G::LeftLateralList1.GetCount() > 0)
		G::LeftLateralList1.RemoveAll();
	if (G::LeftLateralList2.GetCount() > 0)
		G::LeftLateralList2.RemoveAll();
	if (G::RightLateralList1.GetCount() > 0)
		G::RightLateralList1.RemoveAll();

	if (G::RightLateralList2.GetCount() > 0)
		G::RightLateralList2.RemoveAll();

	if (G::LeftResultlist.GetCount() > 0)
		G::LeftResultlist.RemoveAll();
	
	if (G::RightResultlist.GetCount() > 0)
		G::RightResultlist.RemoveAll();
	
	return 0;
}

//更新颜色
int    G::UpdateColor(CDialog* pdlg, UINT uid, UINT uvalue)
{
	if (0 == uvalue)
	{
		G::SetCtlColor(pdlg, uid, RGB(255, 60, 60));
	}
	else if (1 == uvalue)
	{
		G::SetCtlColor(pdlg, uid, RGB(60, 255, 60));
	}
	else if (-1 == uvalue)
	{
		G::SetCtlColor(pdlg, uid, RGB(200, 200, 50));
	}
	return 0;
}

//设置控件颜色
int    G::SetCtlColor(CDialog* pdlg, UINT uid, COLORREF color)
{
	CWnd* pmwnd = pdlg->GetDlgItem(uid);
	CDC *pdc = pmwnd->GetDC();
	RECT rect;
	pmwnd->GetClientRect(&rect);
	pdc->FillSolidRect((LPCRECT)&rect, color);
	pmwnd->ReleaseDC(pdc);
	return 0;
}

// double转string型
CString G::double_string(double xx)
{
	CString  mm;
	mm.Format(L"%.3lf", xx);
	return mm;
}


// string转double型
double G::string_double(CString xx)
{
	double   mm;
	mm = _wtof(xx);
	return mm;
}


// 图像处理1（左产品尺寸）
int G::ImageDispose1(HObject  ho_image, HTuple hv_modle)
{
	//CheckSize  sizelink;

	HObject  ho_ROI_0, ho_ImageReduced0, ho_Regions0, ho_RegionOpening0;
	HObject  ho_RegionFillUp0, ho_Image, ho_Rectangle;

	HTuple   hv_Phi0, hv_Area0, hv_Row0, hv_Column0, hv_HomMat2D0;
	HTuple   hv_Row, hv_Column, hv_Angle, hv_Score;
	HTuple   hv_AmplitudeThreshold, hv_RoiWidthLen2;
	HTuple  hv_LineRowStart_Measure_01_0, hv_LineColumnStart_Measure_01_0;
	HTuple  hv_LineRowEnd_Measure_01_0, hv_LineColumnEnd_Measure_01_0;
	HTuple  hv_TmpCtrl_Row, hv_TmpCtrl_Column, hv_TmpCtrl_Dr;
	HTuple  hv_TmpCtrl_Dc, hv_TmpCtrl_Phi, hv_TmpCtrl_Len1;
	HTuple  hv_TmpCtrl_Len2, hv_MsrHandle_Measure_01_0, hv_Row_Measure_01_0;
	HTuple  hv_Column_Measure_01_0, hv_Amplitude_Measure_01_0;
	HTuple  hv_Distance_Measure_01_0, hv_LineRowStart_Measure_02_0;
	HTuple  hv_LineColumnStart_Measure_02_0, hv_LineRowEnd_Measure_02_0;
	HTuple  hv_LineColumnEnd_Measure_02_0, hv_MsrHandle_Measure_02_0;
	HTuple  hv_Row_Measure_02_0, hv_Column_Measure_02_0, hv_Amplitude_Measure_02_0;
	HTuple  hv_Distance_Measure_02_0, hv_diameter, hv_length;

	try
	{
		//仿射变换
		GenRectangle1(&ho_ROI_0, 219.5, 501.9, 542, 1769.1);
		ReduceDomain(ho_image, ho_ROI_0, &ho_ImageReduced0);
		Threshold(ho_ImageReduced0, &ho_Regions0, 0, 141);
		OpeningRectangle1(ho_Regions0, &ho_RegionOpening0, 3, 3);
		FillUp(ho_RegionOpening0, &ho_RegionFillUp0);
		OrientationRegion(ho_RegionFillUp0, &hv_Phi0);
		AreaCenter(ho_RegionFillUp0, &hv_Area0, &hv_Row0, &hv_Column0);
		VectorAngleToRigid(hv_Row0, hv_Column0, hv_Phi0, hv_Row0, hv_Column0, 0, &hv_HomMat2D0);
		AffineTransImage(ho_ImageReduced0, &ho_Image, hv_HomMat2D0, "constant", "false");

		//Image Acquisition 01: Do something
		FindShapeModel(ho_Image, hv_modle, -0.39, 0.78, 0.85, 1, 0.5, "least_squares",
			0, 0.9, &hv_Row, &hv_Column, &hv_Angle, &hv_Score);
		if (0 != ((hv_Row.TupleLength()) == 1))
		{
			//hv_c += 1;
			/*dev_display_shape_matching_results(hv_modle, "red", hv_Row, hv_Column, hv_Angle,
				1, 1, 0);*/
			//gen_cross_contour_xld (Cross, Row, Column, 6, Angle)

			GenRectangle1(&ho_Rectangle, hv_Row - 100, hv_Column - 50, hv_Row + 120, hv_Column + 1200);

			//测量外径
			//Measure 01: Code generated by Measure 01
			//Measure 01: Prepare measurement
			hv_AmplitudeThreshold = 58;
			hv_RoiWidthLen2 = 5;
			SetSystem("int_zooming", "false");
			//Measure 01: Coordinates for line Measure 01 [0]
			hv_LineRowStart_Measure_01_0 = hv_Row - 100;
			hv_LineColumnStart_Measure_01_0 = hv_Column + 20;
			hv_LineRowEnd_Measure_01_0 = hv_Row + 120;
			hv_LineColumnEnd_Measure_01_0 = hv_Column + 20;
			//Measure 01: Convert coordinates to rectangle2 type
			hv_TmpCtrl_Row = 0.5*(hv_LineRowStart_Measure_01_0 + hv_LineRowEnd_Measure_01_0);
			hv_TmpCtrl_Column = 0.5*(hv_LineColumnStart_Measure_01_0 + hv_LineColumnEnd_Measure_01_0);
			hv_TmpCtrl_Dr = hv_LineRowStart_Measure_01_0 - hv_LineRowEnd_Measure_01_0;
			hv_TmpCtrl_Dc = hv_LineColumnEnd_Measure_01_0 - hv_LineColumnStart_Measure_01_0;
			hv_TmpCtrl_Phi = hv_TmpCtrl_Dr.TupleAtan2(hv_TmpCtrl_Dc);
			hv_TmpCtrl_Len1 = 0.5*(((hv_TmpCtrl_Dr*hv_TmpCtrl_Dr) + (hv_TmpCtrl_Dc*hv_TmpCtrl_Dc)).TupleSqrt());
			hv_TmpCtrl_Len2 = hv_RoiWidthLen2;

			//gen_rectangle1 (Rectangle1, LineRowEnd_Measure_01_0, LineColumnStart_Measure_01_0, LineRowEnd_Measure_01_0, LineColumnEnd_Measure_01_0)
			//Measure 01: Create measure for line Measure 01 [0]
			//Measure 01: Attention: This assumes all images have the same size!
			GenMeasureRectangle2(hv_TmpCtrl_Row, hv_TmpCtrl_Column, hv_TmpCtrl_Phi, hv_TmpCtrl_Len1,
				hv_TmpCtrl_Len2, 1920, 1200, "nearest_neighbor", &hv_MsrHandle_Measure_01_0);
			//Measure 01: ***************************************************************
			//Measure 01: * The code which follows is to be executed once / measurement *
			//Measure 01: ***************************************************************
			//Measure 01: The image is assumed to be made available in the
			//Measure 01: variable last displayed in the graphics window
			CopyObj(ho_Image, &ho_Image, 1, 1);
			//Measure 01: Execute measurements
			MeasurePos(ho_Image, hv_MsrHandle_Measure_01_0, 12, 58, "all", "all", &hv_Row_Measure_01_0,
				&hv_Column_Measure_01_0, &hv_Amplitude_Measure_01_0, &hv_Distance_Measure_01_0);
			//Measure 01: Do something with the results
			//Measure 01: Clear measure when done
			CloseMeasure(hv_MsrHandle_Measure_01_0);
			//hv_diameter = hv_diameter.TupleConcat(hv_Distance_Measure_01_0);
			//数据转成double，传入链表
			//sizelink.Diameter = hv_Distance_Measure_01_0[0].D()*0.01465;

			//测量总长
			//Measure 02: Code generated by Measure 02
			//Measure 02: Prepare measurement
			hv_AmplitudeThreshold = 70;
			hv_RoiWidthLen2 = 1.5;
			SetSystem("int_zooming", "true");
			//Measure 02: Coordinates for line Measure 02 [0]
			//367
			hv_LineRowStart_Measure_02_0 = hv_Row + 10;
			hv_LineColumnStart_Measure_02_0 = hv_Column - 50;
			hv_LineRowEnd_Measure_02_0 = hv_Row + 10;
			hv_LineColumnEnd_Measure_02_0 = hv_Column + 1200;
			//Measure 02: Convert coordinates to rectangle2 type
			hv_TmpCtrl_Row = 0.5*(hv_LineRowStart_Measure_02_0 + hv_LineRowEnd_Measure_02_0);
			hv_TmpCtrl_Column = 0.5*(hv_LineColumnStart_Measure_02_0 + hv_LineColumnEnd_Measure_02_0);
			hv_TmpCtrl_Dr = hv_LineRowStart_Measure_02_0 - hv_LineRowEnd_Measure_02_0;
			hv_TmpCtrl_Dc = hv_LineColumnEnd_Measure_02_0 - hv_LineColumnStart_Measure_02_0;
			hv_TmpCtrl_Phi = hv_TmpCtrl_Dr.TupleAtan2(hv_TmpCtrl_Dc);
			hv_TmpCtrl_Len1 = 0.5*(((hv_TmpCtrl_Dr*hv_TmpCtrl_Dr) + (hv_TmpCtrl_Dc*hv_TmpCtrl_Dc)).TupleSqrt());
			hv_TmpCtrl_Len2 = hv_RoiWidthLen2;
			//Measure 02: Create measure for line Measure 02 [0]
			//Measure 02: Attention: This assumes all images have the same size!
			GenMeasureRectangle2(hv_TmpCtrl_Row, hv_TmpCtrl_Column, hv_TmpCtrl_Phi, hv_TmpCtrl_Len1,
				hv_TmpCtrl_Len2, 1920, 1200, "nearest_neighbor", &hv_MsrHandle_Measure_02_0);
			//Measure 02: ***************************************************************
			//Measure 02: * The code which follows is to be executed once / measurement *
			//Measure 02: ***************************************************************
			//Measure 02: The image is assumed to be made available in the
			//Measure 02: variable last displayed in the graphics window
			CopyObj(ho_Image, &ho_Image, 1, 1);
			//Measure 02: Execute measurements
			MeasurePos(ho_Image, hv_MsrHandle_Measure_02_0, 1, 70, "all", "all", &hv_Row_Measure_02_0,
				&hv_Column_Measure_02_0, &hv_Amplitude_Measure_02_0, &hv_Distance_Measure_02_0);
			//Measure 02: Do something with the results
			//Measure 02: Clear measure when done
			CloseMeasure(hv_MsrHandle_Measure_02_0);
			//hv_length = hv_length.TupleConcat(hv_Distance_Measure_02_0);
			//转换数据格式，传入链表
			//sizelink.TotalLength = hv_Distance_Measure_02_0[0].D()*0.01465;
		}
		///没有匹配到模板
		else
		{
			G::disp_message(G::HW1, "未匹配到模板", "image",100, 100, "red", "false");
			//sizelink.Diameter = 0;
			//sizelink.TotalLength = 0;
		}
	}
	catch (...)
	{
		//sizelink.Diameter = 0;
		//sizelink.TotalLength = 0;
		//G::Sizelist1.AddTail(sizelink);
		//return -1;
	}
	//G::LeftDisposecounts1++;                   //处理数加一
	//G::m_criticalsection_sizelist1.Lock();
	//G::Sizelist1.AddTail(sizelink);
	//G::m_criticalsection_sizelist1.Unlock();
	//传入数据用于显示
	//G::LeftLength = sizelink.TotalLength;
	//G::LeftDiameter = sizelink.Diameter;

	/*SetDraw(Global::HW2, "margin");
	SetColor(Global::HW2, "green");*/

	return 0;
}

// 图像处理2（右产品尺寸）
int G::ImageDispose2(HObject  ho_image, HTuple hv_modle)
{
	//CheckSize  sizelink;

	//HObject  ho_ROI_0, ho_ImageReduced0, ho_Regions0, ho_RegionOpening0;
	//HObject  ho_RegionFillUp0, ho_Image, ho_Rectangle;

	//HTuple   hv_Phi0, hv_Area0, hv_Row0, hv_Column0, hv_HomMat2D0;
	//HTuple   hv_Row, hv_Column, hv_Angle, hv_Score;
	//HTuple   hv_AmplitudeThreshold, hv_RoiWidthLen2;
	//HTuple  hv_LineRowStart_Measure_01_0, hv_LineColumnStart_Measure_01_0;
	//HTuple  hv_LineRowEnd_Measure_01_0, hv_LineColumnEnd_Measure_01_0;
	//HTuple  hv_TmpCtrl_Row, hv_TmpCtrl_Column, hv_TmpCtrl_Dr;
	//HTuple  hv_TmpCtrl_Dc, hv_TmpCtrl_Phi, hv_TmpCtrl_Len1;
	//HTuple  hv_TmpCtrl_Len2, hv_MsrHandle_Measure_01_0, hv_Row_Measure_01_0;
	//HTuple  hv_Column_Measure_01_0, hv_Amplitude_Measure_01_0;
	//HTuple  hv_Distance_Measure_01_0, hv_LineRowStart_Measure_02_0;
	//HTuple  hv_LineColumnStart_Measure_02_0, hv_LineRowEnd_Measure_02_0;
	//HTuple  hv_LineColumnEnd_Measure_02_0, hv_MsrHandle_Measure_02_0;
	//HTuple  hv_Row_Measure_02_0, hv_Column_Measure_02_0, hv_Amplitude_Measure_02_0;
	//HTuple  hv_Distance_Measure_02_0, hv_diameter, hv_length;

	//try
	//{
	//	//仿射变换
	//	GenRectangle1(&ho_ROI_0, 219.5, 501.9, 542, 1769.1);
	//	ReduceDomain(ho_image, ho_ROI_0, &ho_ImageReduced0);
	//	Threshold(ho_ImageReduced0, &ho_Regions0, 0, 141);
	//	OpeningRectangle1(ho_Regions0, &ho_RegionOpening0, 3, 3);
	//	FillUp(ho_RegionOpening0, &ho_RegionFillUp0);
	//	OrientationRegion(ho_RegionFillUp0, &hv_Phi0);
	//	AreaCenter(ho_RegionFillUp0, &hv_Area0, &hv_Row0, &hv_Column0);
	//	VectorAngleToRigid(hv_Row0, hv_Column0, hv_Phi0, hv_Row0, hv_Column0, 0, &hv_HomMat2D0);
	//	AffineTransImage(ho_ImageReduced0, &ho_Image, hv_HomMat2D0, "constant", "false");

	//	//Image Acquisition 01: Do something
	//	FindShapeModel(ho_Image, hv_modle, -0.39, 0.78, 0.85, 1, 0.5, "least_squares",
	//		0, 0.9, &hv_Row, &hv_Column, &hv_Angle, &hv_Score);
	//	if (0 != ((hv_Row.TupleLength()) == 1))
	//	{
	//		//hv_c += 1;
	//		dev_display_shape_matching_results(hv_modle, "red", hv_Row, hv_Column, hv_Angle,
	//			1, 1, 0);
	//		//gen_cross_contour_xld (Cross, Row, Column, 6, Angle)

	//		GenRectangle1(&ho_Rectangle, hv_Row - 100, hv_Column - 50, hv_Row + 120, hv_Column + 1200);

	//		//测量外径
	//		//Measure 01: Code generated by Measure 01
	//		//Measure 01: Prepare measurement
	//		hv_AmplitudeThreshold = 58;
	//		hv_RoiWidthLen2 = 5;
	//		SetSystem("int_zooming", "false");
	//		//Measure 01: Coordinates for line Measure 01 [0]
	//		hv_LineRowStart_Measure_01_0 = hv_Row - 100;
	//		hv_LineColumnStart_Measure_01_0 = hv_Column + 20;
	//		hv_LineRowEnd_Measure_01_0 = hv_Row + 120;
	//		hv_LineColumnEnd_Measure_01_0 = hv_Column + 20;
	//		//Measure 01: Convert coordinates to rectangle2 type
	//		hv_TmpCtrl_Row = 0.5*(hv_LineRowStart_Measure_01_0 + hv_LineRowEnd_Measure_01_0);
	//		hv_TmpCtrl_Column = 0.5*(hv_LineColumnStart_Measure_01_0 + hv_LineColumnEnd_Measure_01_0);
	//		hv_TmpCtrl_Dr = hv_LineRowStart_Measure_01_0 - hv_LineRowEnd_Measure_01_0;
	//		hv_TmpCtrl_Dc = hv_LineColumnEnd_Measure_01_0 - hv_LineColumnStart_Measure_01_0;
	//		hv_TmpCtrl_Phi = hv_TmpCtrl_Dr.TupleAtan2(hv_TmpCtrl_Dc);
	//		hv_TmpCtrl_Len1 = 0.5*(((hv_TmpCtrl_Dr*hv_TmpCtrl_Dr) + (hv_TmpCtrl_Dc*hv_TmpCtrl_Dc)).TupleSqrt());
	//		hv_TmpCtrl_Len2 = hv_RoiWidthLen2;

	//		//gen_rectangle1 (Rectangle1, LineRowEnd_Measure_01_0, LineColumnStart_Measure_01_0, LineRowEnd_Measure_01_0, LineColumnEnd_Measure_01_0)
	//		//Measure 01: Create measure for line Measure 01 [0]
	//		//Measure 01: Attention: This assumes all images have the same size!
	//		GenMeasureRectangle2(hv_TmpCtrl_Row, hv_TmpCtrl_Column, hv_TmpCtrl_Phi, hv_TmpCtrl_Len1,
	//			hv_TmpCtrl_Len2, 1920, 1200, "nearest_neighbor", &hv_MsrHandle_Measure_01_0);
	//		//Measure 01: ***************************************************************
	//		//Measure 01: * The code which follows is to be executed once / measurement *
	//		//Measure 01: ***************************************************************
	//		//Measure 01: The image is assumed to be made available in the
	//		//Measure 01: variable last displayed in the graphics window
	//		CopyObj(ho_Image, &ho_Image, 1, 1);
	//		//Measure 01: Execute measurements
	//		MeasurePos(ho_Image, hv_MsrHandle_Measure_01_0, 12, 58, "all", "all", &hv_Row_Measure_01_0,
	//			&hv_Column_Measure_01_0, &hv_Amplitude_Measure_01_0, &hv_Distance_Measure_01_0);
	//		//Measure 01: Do something with the results
	//		//Measure 01: Clear measure when done
	//		CloseMeasure(hv_MsrHandle_Measure_01_0);
	//		//hv_diameter = hv_diameter.TupleConcat(hv_Distance_Measure_01_0);
	//		//数据转成double，传入链表
	//		sizelink.Diameter = hv_Distance_Measure_01_0[0].D()*0.01465;

	//		//测量总长
	//		//Measure 02: Code generated by Measure 02
	//		//Measure 02: Prepare measurement
	//		hv_AmplitudeThreshold = 70;
	//		hv_RoiWidthLen2 = 1.5;
	//		SetSystem("int_zooming", "true");
	//		//Measure 02: Coordinates for line Measure 02 [0]
	//		//367
	//		hv_LineRowStart_Measure_02_0 = hv_Row + 10;
	//		hv_LineColumnStart_Measure_02_0 = hv_Column - 50;
	//		hv_LineRowEnd_Measure_02_0 = hv_Row + 10;
	//		hv_LineColumnEnd_Measure_02_0 = hv_Column + 1200;
	//		//Measure 02: Convert coordinates to rectangle2 type
	//		hv_TmpCtrl_Row = 0.5*(hv_LineRowStart_Measure_02_0 + hv_LineRowEnd_Measure_02_0);
	//		hv_TmpCtrl_Column = 0.5*(hv_LineColumnStart_Measure_02_0 + hv_LineColumnEnd_Measure_02_0);
	//		hv_TmpCtrl_Dr = hv_LineRowStart_Measure_02_0 - hv_LineRowEnd_Measure_02_0;
	//		hv_TmpCtrl_Dc = hv_LineColumnEnd_Measure_02_0 - hv_LineColumnStart_Measure_02_0;
	//		hv_TmpCtrl_Phi = hv_TmpCtrl_Dr.TupleAtan2(hv_TmpCtrl_Dc);
	//		hv_TmpCtrl_Len1 = 0.5*(((hv_TmpCtrl_Dr*hv_TmpCtrl_Dr) + (hv_TmpCtrl_Dc*hv_TmpCtrl_Dc)).TupleSqrt());
	//		hv_TmpCtrl_Len2 = hv_RoiWidthLen2;
	//		//Measure 02: Create measure for line Measure 02 [0]
	//		//Measure 02: Attention: This assumes all images have the same size!
	//		GenMeasureRectangle2(hv_TmpCtrl_Row, hv_TmpCtrl_Column, hv_TmpCtrl_Phi, hv_TmpCtrl_Len1,
	//			hv_TmpCtrl_Len2, 1920, 1200, "nearest_neighbor", &hv_MsrHandle_Measure_02_0);
	//		//Measure 02: ***************************************************************
	//		//Measure 02: * The code which follows is to be executed once / measurement *
	//		//Measure 02: ***************************************************************
	//		//Measure 02: The image is assumed to be made available in the
	//		//Measure 02: variable last displayed in the graphics window
	//		CopyObj(ho_Image, &ho_Image, 1, 1);
	//		//Measure 02: Execute measurements
	//		MeasurePos(ho_Image, hv_MsrHandle_Measure_02_0, 1, 70, "all", "all", &hv_Row_Measure_02_0,
	//			&hv_Column_Measure_02_0, &hv_Amplitude_Measure_02_0, &hv_Distance_Measure_02_0);
	//		//Measure 02: Do something with the results
	//		//Measure 02: Clear measure when done
	//		CloseMeasure(hv_MsrHandle_Measure_02_0);
	//		//hv_length = hv_length.TupleConcat(hv_Distance_Measure_02_0);
	//		//转换数据格式，传入链表
	//		sizelink.TotalLength = hv_Distance_Measure_02_0[0].D()*0.01465;
	//	}
	//	///没有匹配到模板
	//	else
	//	{
	//		G::disp_message(G::HW3, "未匹配到模板", "image",160, 100, "red", "false");
	//		sizelink.Diameter = 0;
	//		sizelink.TotalLength = 0;
	//	}
	//}
	//catch (...)
	//{
	//	sizelink.Diameter = 0;
	//	sizelink.TotalLength = 0;
	//	/*G::Sizelist2.AddTail(sizelink);
	//	return -1;*/
	//}
	////G::RightDisposecounts1++;
	//G::m_criticalsection_sizelist2.Lock();
	//G::Sizelist2.AddTail(sizelink);
	//G::m_criticalsection_sizelist2.Unlock();
	////传入数据
	//G::RightLength = sizelink.TotalLength;
	//G::RightDiameter = sizelink.Diameter;
	//
	///*SetDraw(Global::HW2, "margin");
	//SetColor(Global::HW2, "green");*/

	return 0;
}

// 创建数据库
int G::CreateDatabase(CString dbname)
{
	G::db._conn_ = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + dbname + L".accdb"; 

	_bstr_t  _conn_(G::db._conn_);
	try
	{

		/*adModeUnknown  缺省。当前的许可权未设置
			adModeRead  只读
			adModeWrite  只写
			adModeReadWrite  可以读写
			adModeShareDenyRead  阻止其它 Connection 对象以读权限打开连接
			adModeShareDenyWrite  阻止其它 Connection 对象以写权限打开连接
			adModeShareExclusive  阻止其它 Connection 对象以读写权限打开连接
			adModeShareDenyNone  阻止其它 Connection 对象以任何权限打开连接*/
		G::db.m_pConnection = NULL;
		//创建m_pConnection实例
		G::db.m_pConnection.CreateInstance("ADODB.Connection");
		//或者m_pConnection.CreateInstance(__uuidof(Connection));
		//使用'.'而不是->创建m_Connection实例，然后m_pConnection->open方法创建连接。
		G::db.m_pConnection->ConnectionTimeout = 3;
		//连接ACCESS2000   
		/*Microsoft.Jet.OLEDB.4.0*/
		//创建m_pRecordset实例
		G::db.m_pRecordset.CreateInstance(__uuidof(Recordset));
		G::db.m_pConnection->Open(_conn_, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		try
		{
			G::db.m_pCatalog = NULL;
			G::db.m_pCatalog.CreateInstance(__uuidof(Catalog));
			//创建数据源
			G::db.m_pCatalog->Create(_conn_);
			//再次打开
			G::db.m_pConnection->Open(_conn_, "", "", adModeUnknown);
		}
		catch (_com_error f)
		{
			//AfxMessageBox(f.Description() + _T("\n数据库连接失败"));
			CString  error;
			error = f.ErrorMessage();
			AfxMessageBox(error + L"\n数据库连接失败");
		}
		
	}

	return 0;
}


// 在数据库中创建表格
int G::CreateTable(CString TableName)
{
	CString   str;
	try
	{
		str = L"CREATE TABLE " + TableName + L"(序号 DOUBLE, 类型 Text,参数名 Text,参数值 DOUBLE,最大值 DOUBLE,最小值 DOUBLE,备注 Text)";
		_bstr_t exeword(str);
		try
		{
			//两种方式打开数据库，可用两种方式操作数据库
			G::db.m_pConnection->Execute(exeword, &db.RecordsAffected, adCmdText);
			/*G::db.m_pRecordset->Open("SELECT  *  FROM " + (_bstr_t)ParaTableName, G::db.m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockPessimistic, adCmdText);*/
		}
		catch (_com_error c)
		{
			/*G::db.m_pRecordset->Open("SELECT  *  FROM " + (_bstr_t)ParaTableName, G::db.m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockPessimistic, adCmdText);*/
		}
	}
	catch (_com_error f)
	{
		CString  errormesg;
		errormesg = f.ErrorMessage();
		AfxMessageBox(errormesg + L"数据表创建失败");
		return -1;
	}
	

	return 0;
}



// 写入一个类型的参数
//0:运动参数   1:相机参数  2：视觉参数
int G::WriteSerialPara(short SerialNo)
{
	///可静态定义各种类参数数量
	int count, beginno;
	switch (SerialNo)
	{
	case 0:         //运动参数
		beginno = 0;
		count = MotionParaCounts;
		break;
	case 1:         //相机参数
		beginno = MotionParaCounts; 
		count = CameraParaCounts;
		break;
	case 2:             //视觉参数
		count = VisionParaCounts;
		beginno = MotionParaCounts+CameraParaCounts;
		break;
	}
	G::db.m_pRecordset->MoveFirst();
	G::db.m_pRecordset->Move(beginno);                //移到指定位置(从1开始计数)
	for (int i = beginno; i < count + beginno; i++)
	{
		try
		{
			G::db.m_pRecordset->PutCollect(L"类型", _variant_t(G::PR[i].type));
			G::db.m_pRecordset->PutCollect(L"参数名", _variant_t(G::PR[i].paraname));
			G::db.m_pRecordset->PutCollect(L"参数值", _variant_t(G::double_string(G::PR[i].paravalue)));
			G::db.m_pRecordset->PutCollect(L"最大值", _variant_t(G::double_string(G::PR[i].paraMaxValue)));
			G::db.m_pRecordset->PutCollect(L"最小值", _variant_t(G::double_string(G::PR[i].paraMinValue)));
			G::db.m_pRecordset->PutCollect(L"备注", _variant_t(G::PR[i].remark));

			G::db.m_pRecordset->Update();
			G::db.m_pRecordset->MoveNext();
		}
		catch (_com_error c)
		{
			CString  err;
			err = c.ErrorMessage();
			//AfxMessageBox(err + L"失败");
			return -1;
		}
	}
	return 0;
}


// 再次读取参数时使用
int G::ReadPara2()
{
	try
	{
		G::db.m_pRecordset->MoveFirst();
		for (int i = 0; i < 50; i++)
		{
			try
			{
				G::PR[i].type = G::db.m_pRecordset->GetCollect(L"类型");
				G::PR[i].paraname = G::db.m_pRecordset->GetCollect(L"参数名");
				G::PR[i].paravalue = G::db.m_pRecordset->GetCollect(L"参数值");
				G::PR[i].paraMaxValue = G::db.m_pRecordset->GetCollect(L"最大值");
				G::PR[i].paraMinValue = G::db.m_pRecordset->GetCollect(L"最小值");
				G::PR[i].remark = G::db.m_pRecordset->GetCollect(L"备注");

				G::db.m_pRecordset->MoveNext();
			}
			catch (_com_error c)
			{
				CString  err;
				err = c.ErrorMessage();
				AfxMessageBox(err + L"失败,请重启软件");
				return -1;
			}
		}             //for
	}
	catch (_com_error a)
	{
		CString  err;
		err = a.ErrorMessage();
		AfxMessageBox(err + L"失败,请重启软件1");
		return -1;
	}
	return 0;
}
