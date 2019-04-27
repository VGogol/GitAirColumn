#include "stdafx.h"
#include "G.h"


CMCard           G::MCard;


int                 G::MotionUsed;                      //�˶�����ʹ�ø���
int                 G::Camera1Used;                  //���1������ʹ�ø���
int                 G::Camera2Used;                  //���2������ʹ�ø���
int                 G::Camera3Used;                  //���3������ʹ�ø���
int                 G::Camera4Used;                  //���4������ʹ�ø���
int                 G::Camera5Used;                   //���5������ʹ�ø���
int                 G::Camera6Used;                   //���6������ʹ�ø���
int                 G::Vision1Used;                     //�Ӿ�1������ʹ�ø���
int                 G::Vision2Used;                     //�Ӿ�2������ʹ�ø���
int                 G::Vision3Used;                     //�Ӿ�3������ʹ�ø���
int                 G::Vision4Used;                     //�Ӿ�4������ʹ�ø���
int                 G::Vision5Used;                     //�Ӿ�5������ʹ�ø���
int                 G::Vision6Used;                     //�Ӿ�6������ʹ�ø���

PARAM                    G::PR[All_Para_Counts];
DB					       G::db;
CCriticalSection       G::m_criticalsection_imagelist1;			          //ͼ���б�1�ٽ�
CCriticalSection       G::m_criticalsection_imagelist2;			          //ͼ���б�2�ٽ�
CCriticalSection       G::m_criticalsection_imagelist3;			          //ͼ���б�3�ٽ�
CCriticalSection       G::m_criticalsection_imagelist4;                   //ͼ���б�4�ٽ�
CCriticalSection       G::m_criticalsection_imagelist5;                   //ͼ���б�5�ٽ�
CCriticalSection       G::m_criticalsection_imagelist6;                   //ͼ���б�6�ٽ�
//CCriticalSection       G::m_criticalsection_productlist1;               //��Ʒ�б�1�ٽ�
//CCriticalSection       G::m_criticalsection_productlist2;               //��Ʒ�б�2�ٽ�

CCriticalSection       G::m_criticalsection_LeftPositive;            //���Ʒ�����ٽ�
CCriticalSection       G::m_criticalsection_RightPositive;         //�Ҳ�Ʒ�����ٽ�
CCriticalSection       G::m_criticalsection_LeftLateral1;           //���Ʒ����ٽ�
CCriticalSection       G::m_criticalsection_LeftLateral2;           //���Ʒ�Ҳ��ٽ�
CCriticalSection       G::m_criticalsection_RightLateral1;         //�Ҳ�Ʒ����ٽ�
CCriticalSection       G::m_criticalsection_RightLateral2;         //�Ҳ�Ʒ�Ҳ��ٽ�
CCriticalSection       G::m_criticalsection_LeftResultlist;	         //���Ʒ����ٽ�
CCriticalSection       G::m_criticalsection_RightResultlist;        //�Ҳ�Ʒ����ٽ�
//CCriticalSection       G::m_criticalsection_sizelist1tofile;		        //���ݵ��ļ��б�1�ٽ�
//CCriticalSection       G::m_criticalsection_sizelist2tofile;		        //���ݵ��ļ��б�2�ٽ�


////////////////////////////               ȫ�ֱ���
HTuple                     G::hv_AcqHandle1;                       //���1
HTuple                     G::hv_AcqHandle2;                      //���2
HTuple                     G::hv_AcqHandle3;                       //���3
HTuple                     G::hv_AcqHandle4;                      //���4
HTuple                     G::hv_AcqHandle5;                      //���5
HTuple                     G::hv_AcqHandle6;                      //���6
HTuple                     G::HW1;                                      //����1
HTuple                     G::HW2;                                      //����2
HTuple                     G::HW3;                                     //����3
HTuple                     G::HW4;                                    //����4
HTuple                     G::HW5;                                    //����5
HTuple                     G::HW6;                                    //����6

CString                    G::ParaFileName=L".//1269 Air Column para.ini";

BOOL                      G::bRunning;                              //�����б�־   
BOOL                      G::bResetOK;                              //��λ���
BOOL                      G::bCanGrab;                              //��������
bool                        G::bAlarm;                                 //������־
bool                       G::bRunflag;                                  //�Զ������б�־���������Ƶ����ֶ�
bool                       G::bLeftRstOK;                     //����Ḵλ���
bool                       G::bRightRstOK;                      //�ұ��Ḵλ���
bool                       G::bLeftNoShield;                       //���û�����Σ�����ִ��
bool                       G::bRightNoShield;                     //�Ҷ�û�����Σ�����ִ��
BOOL                      G::bOpenExcelOk;                     //Excel�򿪳ɹ�

int                          G::UserAuthority;                             //��ǰ��¼�û�Ȩ��

HTuple                    G::modle1;                                 //ģ��1����ߴ�ģ��

LONGLONG            G::GridCounts1;                         //���ߵĸ���1�������������������
LONGLONG            G::GridCounts2;                        //���ߵĸ���2�������������������

long                       G::grab1counts;                          //ͼ��1ץͼ����
long                       G::grab2counts;                          //ͼ��2ץͼ����
long                       G::grab3counts;                          //ͼ��3ץͼ����
long                       G::grab4counts;                          //ͼ��4ץͼ����
long                       G::grab5counts;                          //ͼ��5ץͼ����
long                       G::grab6counts;                          //ͼ��6ץͼ����
long                       G::grab1delete;                           //ͼ��1ɾ������
long                       G::grab2delete;                           //ͼ��2ɾ������
long                       G::triggercounts1;                        //��������1
long                       G::triggercounts2;                         //��������2
long                       G::blow1;                                     //����Ʒ����1
long                       G::blow2;                                    //����Ʒ����2
long                       G::LeftNullCounts;                 //���ղ�Ʒ�������ڼ��㵥������ֹͣ
long                       G::RightNullCounts;             //�Ҳ�ղ�Ʒ�������ڼ��㵥������ֹͣ
//long                       G::ToFileCounts1;                        //���д�����ݿ����1
//long                       G::ToFileCounts2;                        //���д�����ݿ����2

double                   G::LeftLength;                            //��ǰ���Ʒ�ܳ�
double                   G::LeftDiameter;                        //��ǰ���Ʒ�⾶
double                   G::RightLength;                          //��ǰ�Ҳ�Ʒ�ܳ�
double                   G::RightDiameter;                       //��ǰ�Ҳ�Ʒ�⾶

////////////////////////////////////////////////////////////////////////////////////////
CList<ImageLinked, ImageLinked&>G::Imagelist1;                               //ͼ���б�1
CList<ImageLinked, ImageLinked&>G::Imagelist2;                               //ͼ���б�2
CList<ImageLinked, ImageLinked&>G::Imagelist3;                               //ͼ���б�3
CList<ImageLinked, ImageLinked&>G::Imagelist4;                               //ͼ���б�4
CList<ImageLinked, ImageLinked&>G::Imagelist5;                               //ͼ���б�5
CList<ImageLinked, ImageLinked&>G::Imagelist6;                               //ͼ���б�6
//CList<Product, Product&>G::ProductList1;                              //��Ʒ�б�1-����Ʒ
//CList<Product, Product&>G::ProductList2;                              //��Ʒ�б�2-�Ҳ��Ʒ

CList<ProductPositive, ProductPositive&>G::LeftPositiveList;      //���Ʒ�����б�
CList<ProductPositive, ProductPositive&>G::RightPositiveList;    //�Ҳ�Ʒ���������б�
CList<ProductLateral, ProductLateral&>G::LeftLateralList1;      //���Ʒ����б�
CList<ProductLateral, ProductLateral&>G::LeftLateralList2;     //���Ʒ�Ҳ��б�
CList<ProductLateral, ProductLateral&>G::RightLateralList1;   //�Ҳ�Ʒ����б�
CList<ProductLateral, ProductLateral&>G::RightLateralList2;  //�Ҳ�Ʒ�Ҳ��б�
CList<DisposeResult, DisposeResult&>G::LeftResultlist;        //���Ʒ������б�
CList<DisposeResult, DisposeResult&>G::RightResultlist;        //�Ҳ�Ʒ������б�
//CList<CheckSize, CheckSize&>G::SizelistToFile1;                 //���ߴ������ļ�����1
//CList<CheckSize, CheckSize&>G::SizelistToFile2;                 //���ߴ������ļ�����2


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

// ���ԭ�㺯��
//��ţ���λ�ٶ�
int G::GoHome(int AxisNum,long  RstSpeed)
{
	if (MCard.m_bMotionCardOpenning == FALSE)
	{
		MCard.DebugInfo2(L"GoHome::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	dmc_set_home_pin_logic(0, AxisNum, 0, 0);
	//��ԭ��ģʽ�����㷽��0������1�����򣩣�
	//�������ٶ�ģʽ��0:���ٻ���,��dmc_set_profile����ʼ�ٶ�����,1:���ٻ���,������ٶ�����)
	dmc_set_homemode(0, AxisNum, 0, 1, 0, 1);          //���û�ԭ��ģʽ
	//���������ʽ    1�͵�ƽ��Ч
	dmc_set_pulse_outmode(0, AxisNum, 1);     
	dmc_set_profile(0, AxisNum, 1000, RstSpeed, 0.1, 0.1, 500);
	dmc_home_move(0, AxisNum);
	return 0;
}


// �򿪲���ʼ�����
int G::OpenCamera()
{
	try
	{
		HalconCpp::CloseAllFramegrabbers();

		///////////////////////////////////             ���1                      ////////////////////////////////////
		
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
		AfxMessageBox(L"���1��ʧ�ܣ��������ߣ�");
		return -1;
	}

	////////////////////////////////////         ���2              //////////////////////////////////////////////
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
	//	AfxMessageBox(L"���2��ʧ�ܣ��������ߣ�");
	//	return -1;
	//}

	////////////////////////////////////         ���3              //////////////////////////////////////////////
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
		AfxMessageBox(L"���3��ʧ�ܣ��������ߣ�");
		return -1;
	}

	////////////////////////////////////         ���4              //////////////////////////////////////////////
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
	//	AfxMessageBox(L"���4��ʧ�ܣ��������ߣ�");
	//	return -1;
	//}

	return 0;
}


// ��ʼ������
BOOL G::InitParam()
{
	///////////////////////////////////////          �����λ              ////////////////////////////////////
	MCard.I[0].lpwstrPortName = L"[0]��ͣ��ť";                        MCard.I[0].sValue = 0;
	MCard.I[1].lpwstrPortName = L"[1]������ť";                        MCard.I[1].sValue = 0;
	MCard.I[2].lpwstrPortName = L"[2]����";		                        MCard.I[2].sValue = 0;
	MCard.I[3].lpwstrPortName = L"[3]���Ϲ���1";		                MCard.I[3].sValue = 0;
	MCard.I[4].lpwstrPortName = L"[4]���Ϲ���1";		                MCard.I[4].sValue = 0;
	MCard.I[5].lpwstrPortName = L"[5]���Ϲ���2";		                MCard.I[5].sValue = 0;
	MCard.I[6].lpwstrPortName = L"[6]���Ϲ���2";		                MCard.I[6].sValue = 0;
	MCard.I[7].lpwstrPortName = L"[7]����";		                        MCard.I[7].sValue = 0;
	MCard.I[8].lpwstrPortName = L"[8]����"; 		                        MCard.I[8].sValue = 0;
	MCard.I[9].lpwstrPortName = L"[9]����";		                        MCard.I[9].sValue = 0;
	MCard.I[10].lpwstrPortName = L"[10]����";		                    MCard.I[10].sValue = 0;
	MCard.I[11].lpwstrPortName = L"[11]����";		                    MCard.I[11].sValue = 0;
	MCard.I[12].lpwstrPortName = L"[12]����";		                    MCard.I[12].sValue = 0;
	MCard.I[13].lpwstrPortName = L"[13]����";		                    MCard.I[13].sValue = 0;
	MCard.I[14].lpwstrPortName = L"[14]����";		                    MCard.I[14].sValue = 0;
	MCard.I[15].lpwstrPortName = L"[15]����";		                    MCard.I[15].sValue = 0;

	/////////////////////////////////////            �����λ              /////////////////////////////////////
	MCard.O[0].lpwstrPortName = L"[0]��ɫ�ƺ��";                    MCard.O[0].sValue = 0;
	MCard.O[1].lpwstrPortName = L"[1]��ɫ���̵�";		             MCard.O[1].sValue = 0;
	MCard.O[2].lpwstrPortName = L"[2]��ɫ�ƻƵ� ";		             MCard.O[2].sValue = 0;
	MCard.O[3].lpwstrPortName = L"[3]������";		                    MCard.O[3].sValue = 0;
	MCard.O[4].lpwstrPortName = L"[4]NG��ŷ�1";	        	    MCard.O[4].sValue = 0;
	MCard.O[5].lpwstrPortName = L"[5]NG��ŷ�2";	        	    MCard.O[5].sValue = 0;
	MCard.O[6].lpwstrPortName = L"[6]����";	        	                MCard.O[6].sValue = 0;
	MCard.O[7].lpwstrPortName = L"[7]����";	        	                MCard.O[7].sValue = 0;
	MCard.O[8].lpwstrPortName = L"[8]����";	        	                MCard.O[8].sValue = 0;
	MCard.O[9].lpwstrPortName = L"[9]����";	        	                MCard.O[9].sValue = 0;
	MCard.O[10].lpwstrPortName = L"[10]����";		                    MCard.O[10].sValue = 0;
	MCard.O[11].lpwstrPortName = L"[11]����";		                    MCard.O[11].sValue = 0;
	MCard.O[12].lpwstrPortName = L"[12]����";		                    MCard.O[12].sValue = 0;
	MCard.O[13].lpwstrPortName = L"[13]����";		                    MCard.O[13].sValue = 0;
	MCard.O[14].lpwstrPortName = L"[14]���������";		        MCard.O[14].sValue = 0;
	MCard.O[15].lpwstrPortName = L"[15]���������"	;		        MCard.O[15].sValue = 0;

	///////////////////////////////////         ȫ�ֲ���                    ////////////////////////////////////////
	///////////////////////�˶�����
	G::MotionUsed = 0;        //ʹ�ø�������
	PR[0].type = L"�˶�����";       PR[0].paraname = L"���Ʒ�ŷ���λ�ٶ�";       PR[0].paraMinValue = 1000;
	PR[0].paraMaxValue = 60000;          PR[0].paravalue = 25000;      PR[0].remark = L"��λ��pulse/s";
	G::MotionUsed++;

	PR[1].type = L"�˶�����";       PR[1].paraname = L"���Ʒ�ŷ��Զ������ٶ�";       PR[1].paraMinValue = 1000;
	PR[1].paraMaxValue = 60000;          PR[1].paravalue = 25000;      PR[1].remark = L"��λ��pulse/s";
	G::MotionUsed++;
 
	PR[2].type = L"�˶�����";       PR[2].paraname = L"���Ʒ��ʼ����λ��";       PR[2].paraMinValue = 1000;
	PR[2].paraMaxValue = 30000;          PR[2].paravalue = 10000;      PR[2].remark = L"��λ��pulse";
	G::MotionUsed++;

	PR[3].type = L"�˶�����";       PR[3].paraname = L"���Ʒ���ռ������";       PR[3].paraMinValue = 1000;
	PR[3].paraMaxValue = 1000000;          PR[3].paravalue = 280000/72;      PR[3].remark = L"��λ��pulse";
	G::MotionUsed++;

	PR[4].type = L"�˶�����";       PR[4].paraname = L"���ƷOK����λ��";       PR[4].paraMinValue = 0;
	PR[4].paraMaxValue = 300000;          PR[4].paravalue = 10000;      PR[4].remark = L"��λ��pulse";
	G::MotionUsed++;

	PR[5].type = L"�˶�����";       PR[5].paraname = L"���ƷNG����λ��";       PR[5].paraMinValue = 0;
	PR[5].paraMaxValue = 100000;          PR[5].paravalue = 280000 / 72;      PR[5].remark = L"��λ��pulse";
	G::MotionUsed++;
	/*PR[4].type = L"�˶�����";       PR[4].paraname = L"�������ʼ����λ��";       PR[4].paraMinValue = 1000;
	PR[4].paraMaxValue = 30000;          PR[4].paravalue = 10000;      PR[4].remark = L"��λ��pulse";
	G::MotionUsed++;

	PR[5].type = L"�˶�����";       PR[5].paraname = L"��������ռ������";       PR[5].paraMinValue = 1000;
	PR[5].paraMaxValue = 10000;          PR[5].paravalue = 280000 / 72;      PR[5].remark = L"��λ��pulse";
	G::MotionUsed++;*/

	PR[6].type = L"�˶�����";       PR[6].paraname = L"�Ҳ�Ʒ�ŷ���λ�ٶ�";       PR[6].paraMinValue = 100;
	PR[6].paraMaxValue = 300000;          PR[6].paravalue = 10000;      PR[6].remark = L"��λ��pulse";
	G::MotionUsed++;

	PR[7].type = L"�˶�����";       PR[7].paraname = L"�Ҳ�Ʒ�ŷ��Զ������ٶ�";       PR[7].paraMinValue = 100;
	PR[7].paraMaxValue = 100000;          PR[7].paravalue = 20000;      PR[7].remark = L"��λ��pulse";
	G::MotionUsed++;

	PR[8].type = L"�˶�����";       PR[8].paraname = L"�Ҳ�Ʒ��ʼ����λ��";       PR[8].paraMinValue = 0;
	PR[8].paraMaxValue = 30000;          PR[8].paravalue = 10000;      PR[8].remark = L"��λ��pulse";
	G::MotionUsed++;

	PR[9].type = L"�˶�����";       PR[9].paraname = L"�Ҳ�Ʒ���ռ������";       PR[9].paraMinValue = 0;
	PR[9].paraMaxValue = 10000;          PR[9].paravalue = 2000;      PR[9].remark = L"��λ��pulse";
	G::MotionUsed++;

	PR[10].type = L"�˶�����";       PR[10].paraname = L"�Ҳ�ƷOK����λ��";       PR[10].paraMinValue = 0;
	PR[10].paraMaxValue = 300000;          PR[10].paravalue = 10000;      PR[10].remark = L"��λ��pulse";
	G::MotionUsed++;

	PR[11].type = L"�˶�����";       PR[11].paraname = L"�����NG����λ��";       PR[11].paraMinValue = 0;
	PR[11].paraMaxValue = 100000;          PR[11].paravalue = 2000;      PR[11].remark = L"��λ��pulse";
	G::MotionUsed++;

	PR[MotionUsed].type = L"�˶�����";       PR[MotionUsed].paraname = L"������λ��ʱʱ��";       PR[MotionUsed].paraMinValue = 0;
	PR[MotionUsed].paraMaxValue = 100000;          PR[MotionUsed].paravalue = 6000;      PR[MotionUsed].remark = L"��λ��ms";
	G::MotionUsed++;

	//13
	PR[MotionUsed].type = L"�˶�����";       PR[MotionUsed].paraname = L"�������������������";       PR[MotionUsed].paraMinValue = 3;
	PR[MotionUsed].paraMaxValue = 1000;          PR[MotionUsed].paravalue = 5;      PR[MotionUsed].remark = L"��λ��PCS";
	G::MotionUsed++;

	//14
	PR[MotionUsed].type = L"�˶�����";       PR[MotionUsed].paraname = L"�Ҳ�����������������";       PR[MotionUsed].paraMinValue = 3;
	PR[MotionUsed].paraMaxValue = 1000;          PR[MotionUsed].paravalue = 5;      PR[MotionUsed].remark = L"��λ��PCS";
	G::MotionUsed++;

	//����
	for (int i = G::MotionUsed; i < 30; i++)
	{
		PR[i].type = L"�˶�����";       PR[i].paraname = L"����";       PR[i].paraMinValue = 0;
		PR[i].paraMaxValue = 50000;       
	}


	////////////////////////////////////////////////////////////�������
	//���1����
	G::Camera1Used = 0;               //���1ʹ�ò�������
	PR[30].type = L"���1����";        PR[30].paraname = L"���1����";       PR[30].paraMinValue = 0;
	PR[30].paraMaxValue = 3000;     PR[30].paravalue = 0;               PR[30].remark = L"��λ��";
	G::Camera1Used++;

	PR[31].type = L"���1����";        PR[31].paraname = L"���1�ع�ʱ��";       PR[31].paraMinValue = 0;
	PR[31].paraMaxValue = 3000;     PR[31].paravalue = 3000;               PR[31].remark = L"��λ��";
	G::Camera1Used++;

	PR[32].type = L"���1����";        PR[32].paraname = L"���1��Ұ��";       PR[32].paraMinValue = 0;
	PR[32].paraMaxValue = 3000;     PR[32].paravalue = 1920;               PR[32].remark = L"��λ������";
	G::Camera1Used++;

	PR[33].type = L"���1����";        PR[33].paraname = L"���1��Ұ��";       PR[33].paraMinValue = 0;
	PR[33].paraMaxValue = 3000;     PR[33].paravalue = 1200;               PR[33].remark = L"��λ������";
	G::Camera1Used++;

	//���1����
	for (int i = 30+G::Camera1Used; i < 40; i++)
	{
		PR[i].type = L"���1����";      PR[i].paraname = L"����";
	}


	//���2����
	G::Camera2Used = 0;
	PR[40].type = L"���2����";        PR[40].paraname = L"���2����";       PR[40].paraMinValue = 0;
	PR[40].paraMaxValue = 3000;     PR[40].paravalue = 300;               PR[40].remark = L"��λ��";
	G::Camera2Used++;

	PR[41].type = L"���2����";        PR[41].paraname = L"���2�ع�ʱ��";       PR[41].paraMinValue = 0;
	PR[41].paraMaxValue = 3000;     PR[41].paravalue = 300;               PR[41].remark = L"��λ��";
	G::Camera2Used++;

	PR[42].type = L"���2����";        PR[42].paraname = L"���2��Ұ��";       PR[42].paraMinValue = 0;
	PR[42].paraMaxValue = 3000;     PR[42].paravalue = 1920;               PR[42].remark = L"��λ������";
	G::Camera2Used++;

	PR[43].type = L"���2����";        PR[43].paraname = L"���2��Ұ��";       PR[43].paraMinValue = 0;
	PR[43].paraMaxValue = 3000;     PR[43].paravalue = 1920;               PR[43].remark = L"��λ������";
	G::Camera2Used++;
	
	//���2����
	for (int i = 40+G::Camera2Used; i < 50; i++)
	{
		PR[i].type = L"���2����";      PR[i].paraname = L"����";
	}


	//���3����
	G::Camera3Used = 0;
	PR[50].type = L"���3����";        PR[50].paraname = L"���3����";       PR[50].paraMinValue = 0;
	PR[50].paraMaxValue = 3000;     PR[50].paravalue = 0;               PR[50].remark = L"��λ��";
	G::Camera3Used++;

	PR[51].type = L"���3����";        PR[51].paraname = L"���3�ع�ʱ��";       PR[51].paraMinValue = 0;
	PR[51].paraMaxValue = 3000;     PR[51].paravalue = 3000;               PR[51].remark = L"��λ��";
	G::Camera3Used++;

	PR[52].type = L"���3����";        PR[52].paraname = L"���3��Ұ��";       PR[52].paraMinValue = 0;
	PR[52].paraMaxValue = 3000;     PR[52].paravalue = 1920;               PR[52].remark = L"��λ������";
	G::Camera3Used++;

	PR[53].type = L"���3����";        PR[53].paraname = L"���3��Ұ��";       PR[53].paraMinValue = 0;
	PR[53].paraMaxValue = 3000;     PR[53].paravalue = 1200;               PR[53].remark = L"��λ������";
	G::Camera3Used++;
	
	//���3����
	for (int i = 50+G::Camera3Used; i < 60; i++)
	{
		PR[i].type = L"���3����";      PR[i].paraname = L"����";
	}


	//���4����
	G::Camera4Used = 0;
	PR[60].type = L"���4����";        PR[60].paraname = L"���4����";       PR[60].paraMinValue = 0;
	PR[60].paraMaxValue = 3000;     PR[60].paravalue = 300;               PR[60].remark = L"��λ��";
	G::Camera4Used++;

	PR[61].type = L"���4����";        PR[61].paraname = L"���4�ع�ʱ��";       PR[61].paraMinValue = 0;
	PR[61].paraMaxValue = 3000;     PR[61].paravalue = 300;               PR[61].remark = L"��λ��";
	G::Camera4Used++;

	PR[62].type = L"���4����";        PR[62].paraname = L"���4��Ұ��";       PR[62].paraMinValue = 0;
	PR[62].paraMaxValue = 3000;     PR[62].paravalue = 1920;               PR[62].remark = L"��λ������";
	G::Camera4Used++;

	PR[63].type = L"���4����";        PR[63].paraname = L"���4��Ұ��";       PR[63].paraMinValue = 0;
	PR[63].paraMaxValue = 3000;     PR[63].paravalue = 1920;               PR[63].remark = L"��λ������";
	G::Camera4Used++;
	
	//���4����
	for (int i = 60+G::Camera4Used; i < 70; i++)
	{
		PR[i].type = L"���4����";      PR[i].paraname = L"����";
	}


	//���5����
	G::Camera5Used = 0;                       //ʹ�ø�������
	PR[70].type = L"���5����";        PR[70].paraname = L"���5����";       PR[70].paraMinValue = 0;
	PR[70].paraMaxValue = 3000;     PR[70].paravalue = 300;               PR[70].remark = L"��λ��";
	G::Camera5Used++;

	PR[71].type = L"���5����";        PR[71].paraname = L"���5�ع�ʱ��";       PR[71].paraMinValue = 0;
	PR[71].paraMaxValue = 3000;     PR[71].paravalue = 300;               PR[71].remark = L"��λ��";
	G::Camera5Used++;

	PR[72].type = L"���5����";        PR[72].paraname = L"���5��Ұ��";       PR[72].paraMinValue = 0;
	PR[72].paraMaxValue = 3000;     PR[72].paravalue = 1920;               PR[72].remark = L"��λ������";
	G::Camera5Used++;

	PR[73].type = L"���5����";        PR[73].paraname = L"���5��Ұ��";       PR[73].paraMinValue = 0;
	PR[73].paraMaxValue = 3000;     PR[73].paravalue = 1920;               PR[73].remark = L"��λ������";
	G::Camera5Used++;

	//���5����
	for (int i = 70 + G::Camera5Used; i < 80; i++)
	{
		PR[i].type = L"���5����";      PR[i].paraname = L"����";
	}

	//���6����
	G::Camera6Used = 0;
	PR[80].type = L"���6����";        PR[80].paraname = L"���6����";       PR[80].paraMinValue = 0;
	PR[80].paraMaxValue = 3000;     PR[80].paravalue = 300;               PR[80].remark = L"��λ��";
	G::Camera6Used++;

	PR[81].type = L"���6����";        PR[81].paraname = L"���6�ع�ʱ��";       PR[81].paraMinValue = 0;
	PR[81].paraMaxValue = 3000;     PR[81].paravalue = 300;               PR[81].remark = L"��λ��";
	G::Camera6Used++;

	PR[82].type = L"���6����";        PR[82].paraname = L"���6��Ұ��";       PR[82].paraMinValue = 0;
	PR[82].paraMaxValue = 3000;     PR[82].paravalue = 1920;               PR[82].remark = L"��λ������";
	G::Camera6Used++;

	PR[83].type = L"���6����";        PR[83].paraname = L"���6��Ұ��";       PR[83].paraMinValue = 0;
	PR[83].paraMaxValue = 3000;     PR[83].paravalue = 1920;               PR[83].remark = L"��λ������";
	G::Camera6Used++;

	//���6����
	for (int i = 80 + G::Camera6Used; i < 90; i++)
	{
		PR[i].type = L"���6����";      PR[i].paraname = L"����";
	}


	///////////////////////////////////////////////////////////////�Ӿ�����
	//�Ӿ�1����߳ߴ��⣩
	G::Vision1Used = 0;
	PR[90].type = L"�Ӿ�1����";      PR[90].paraname = L"�Ӿ�1�ָ���ֵ��ֵ";      PR[90].paraMinValue = 0;
	PR[90].paraMaxValue = 255;    PR[90].paravalue = 0;                PR[90].remark = L"��λ��";
	G::Vision1Used++;

	PR[91].type = L"�Ӿ�1����";      PR[91].paraname = L"�Ӿ�1�ָ���ֵ��ֵ";      PR[91].paraMinValue = 0;
	PR[91].paraMaxValue = 255;    PR[91].paravalue = 200;                PR[91].remark = L"��λ��";
	G::Vision1Used++;	

	PR[92].type = L"�Ӿ�1����";      PR[92].paraname = L"���Ʒ�ܳ�OK��С�ߴ�";      PR[92].paraMinValue = 10;
	PR[92].paraMaxValue = 20;    PR[92].paravalue = 14;                PR[92].remark = L"��λ��mm";
	G::Vision1Used++;

	PR[93].type = L"�Ӿ�1����";      PR[93].paraname = L"���Ʒ�ܳ�OK���ߴ�";      PR[93].paraMinValue = 10;
	PR[93].paraMaxValue = 20;    PR[93].paravalue = 16;                PR[93].remark = L"��λ��mm";
	G::Vision1Used++;

	PR[94].type = L"�Ӿ�1����";      PR[94].paraname = L"���Ʒ�⾶OK��С�ߴ�";      PR[94].paraMinValue = 2.2;
	PR[94].paraMaxValue = 5;    PR[94].paravalue = 1;                PR[94].remark = L"��λ��mm";
	G::Vision1Used++;

	PR[95].type = L"�Ӿ�1����";      PR[95].paraname = L"���Ʒ�⾶OK���ߴ�";      PR[95].paraMinValue = 2.6;
	PR[95].paraMaxValue = 5;    PR[95].paravalue = 1;                PR[95].remark = L"��λ��mm";
	G::Vision1Used++;

	//�Ӿ�1����
	for (int i = 90+G::Vision1Used; i < 100; i++)
	{
		PR[i].type = L"�Ӿ�1����";      PR[i].paraname = L"����";
	}

	//�Ӿ�2������������ۣ�
	G::Vision2Used = 0;
	PR[100].type = L"�Ӿ�2����";      PR[100].paraname = L"�Ӿ�2�ָ���ֵ��ֵ";      PR[100].paraMinValue = 0;
	PR[100].paraMaxValue = 255;    PR[100].paravalue = 0;                PR[100].remark = L"��λ��";
	G::Vision2Used++;

	PR[101].type = L"�Ӿ�2����";      PR[101].paraname = L"�Ӿ�2�ָ���ֵ��ֵ";      PR[101].paraMinValue = 0;
	PR[101].paraMaxValue = 255;    PR[101].paravalue = 200;                PR[101].remark = L"��λ��";
	G::Vision2Used++;

	//�Ӿ�2����
	for (int i = 100+G::Vision2Used; i < 110; i++)
	{
		PR[i].type = L"�Ӿ�2����";      PR[i].paraname = L"����";
	}

	//�Ӿ�3�������-�Ҷ���ۣ�
	G::Vision3Used = 0;
	PR[110].type = L"�Ӿ�3����";      PR[110].paraname = L"�Ӿ�3�ָ���ֵ��ֵ";      PR[110].paraMinValue = 0;
	PR[110].paraMaxValue = 255;    PR[110].paravalue = 0;                PR[110].remark = L"��λ��";
	G::Vision3Used++;

	PR[111].type = L"�Ӿ�3����";      PR[111].paraname = L"�Ӿ�3�ָ���ֵ��ֵ";      PR[111].paraMinValue = 0;
	PR[111].paraMaxValue = 255;    PR[111].paravalue = 200;                PR[111].remark = L"��λ��";
	G::Vision3Used++;

	//�Ӿ�3����
	for (int i = 110+G::Vision3Used; i < 120; i++)
	{
		PR[i].type = L"�Ӿ�3����";      PR[i].paraname = L"����";
	}

	//�Ӿ�4�����������-�ߴ��⣩
	G::Vision4Used = 0;
	PR[120].type = L"�Ӿ�4����";     PR[120].paraname = L"�Ӿ�4�ָ���ֵ��ֵ";      PR[120].paraMinValue = 0;
	PR[120].paraMaxValue = 255;    PR[120].paravalue = 0;                PR[120].remark = L"��λ��";
	G::Vision4Used++;

	PR[121].type = L"�Ӿ�4����";      PR[121].paraname = L"�Ӿ�4�ָ���ֵ��ֵ";      PR[121].paraMinValue = 0;
	PR[121].paraMaxValue = 255;    PR[121].paravalue = 200;                PR[121].remark = L"��λ��";
	G::Vision4Used++;

	PR[122].type = L"�Ӿ�4����";      PR[122].paraname = L"�Ҳ�Ʒ�ܳ�OK��С�ߴ�";      PR[122].paraMinValue = 10;
	PR[122].paraMaxValue = 255;    PR[122].paravalue = 200;                PR[122].remark = L"��λ��";
	G::Vision4Used++;

	PR[123].type = L"�Ӿ�4����";      PR[123].paraname = L"�Ҳ�Ʒ�ܳ�OK���ߴ�";      PR[123].paraMinValue = 10;
	PR[123].paraMaxValue = 255;    PR[123].paravalue = 200;                PR[123].remark = L"��λ��";
	G::Vision4Used++;

	PR[124].type = L"�Ӿ�4����";      PR[124].paraname = L"�Ҳ�Ʒ�⾶OK��С�ߴ�";      PR[124].paraMinValue = 10;
	PR[124].paraMaxValue = 255;    PR[124].paravalue = 200;                PR[124].remark = L"��λ��";
	G::Vision4Used++;

	PR[125].type = L"�Ӿ�4����";      PR[125].paraname = L"�Ҳ�Ʒ�⾶OK���ߴ�";      PR[125].paraMinValue = 10;
	PR[125].paraMaxValue = 255;    PR[125].paravalue = 200;                PR[125].remark = L"��λ��";
	G::Vision4Used++;

	//�Ӿ�4����
	for (int i = G::Vision4Used+120; i < 130; i++)
	{
		PR[i].type = L"�Ӿ�4����";      PR[i].paraname = L"����";
	}


	//�Ӿ�5�������-�����ۣ�
	G::Vision5Used = 0;
	PR[130].type = L"�Ӿ�5����";      PR[130].paraname = L"�Ӿ�5�ָ���ֵ��ֵ";      PR[130].paraMinValue = 0;
	PR[130].paraMaxValue = 255;    PR[130].paravalue = 0;                PR[130].remark = L"��λ��";
	G::Vision5Used++;

	PR[131].type = L"�Ӿ�5����";      PR[131].paraname = L"�Ӿ�5�ָ���ֵ��ֵ";      PR[131].paraMinValue = 0;
	PR[131].paraMaxValue = 255;    PR[131].paravalue = 200;                PR[131].remark = L"��λ��";
	G::Vision5Used++;

	//�Ӿ�3����
	for (int i = 130 + G::Vision5Used; i < 140; i++)
	{
		PR[i].type = L"�Ӿ�5����";      PR[i].paraname = L"����";
	}


	//�Ӿ�6�����������-�Ҷ���ۣ�
	G::Vision6Used = 0;
	PR[140].type = L"�Ӿ�6����";     PR[140].paraname = L"�Ӿ�6�ָ���ֵ��ֵ";      PR[140].paraMinValue = 0;
	PR[140].paraMaxValue = 255;    PR[140].paravalue = 0;                PR[140].remark = L"��λ��";
	G::Vision6Used++;

	PR[141].type = L"�Ӿ�6����";      PR[141].paraname = L"�Ӿ�6�ָ���ֵ��ֵ";      PR[141].paraMinValue = 0;
	PR[141].paraMaxValue = 255;    PR[141].paravalue = 200;                PR[141].remark = L"��λ��";
	G::Vision6Used++;

	//�Ӿ�4����
	for (int i = G::Vision6Used + 140; i < 150; i++)
	{
		PR[i].type = L"�Ӿ�6����";      PR[i].paraname = L"����";
	}

	///////////////�����������
	PR[150].type = L"��������";		PR[150].paraname = L"�ܲ���";       PR[150].paravalue = 100;
	PR[151].type = L"��������";       PR[151].paraname = L"OK��";         PR[151].paravalue = 0;
	PR[152].type = L"��������";       PR[152].paraname = L"NG��";         PR[152].paravalue = 0;
	PR[153].type = L"��������";       PR[153].paraname = L"����";           PR[153].paravalue = 100;
	PR[154].type = L"��������";       PR[154].paraname = L"CT";             PR[154].paravalue = 0;
	PR[155].type = L"��������";       PR[155].paraname = L"UPH";          PR[155].paravalue = 0;

	return 0;
}


// ����ͨ�ò���������д�룩
int G::WriteCommonPara()
{
	try
	{
		/*G::db.m_pRecordset->Open("SELECT  *  FROM " + (_bstr_t)ParamTableName, G::db.m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockPessimistic, adCmdText);*/
		for (int i = 0; i < 150; i++)
		{
			/*if (G::PR[i].paraname != L"����")
			{*/
			try
			{
				G::db.m_pRecordset->AddNew();
				G::db.m_pRecordset->PutCollect(L"���", _variant_t(G::double_string(i + 1)));
				G::db.m_pRecordset->PutCollect(L"����", _variant_t(G::PR[i].type));
				G::db.m_pRecordset->PutCollect(L"������", _variant_t(G::PR[i].paraname));
				G::db.m_pRecordset->PutCollect(L"����ֵ", _variant_t(G::double_string(G::PR[i].paravalue)));
				G::db.m_pRecordset->PutCollect(L"���ֵ", _variant_t(G::double_string(G::PR[i].paraMaxValue)));
				G::db.m_pRecordset->PutCollect(L"��Сֵ", _variant_t(G::double_string(G::PR[i].paraMinValue)));
				G::db.m_pRecordset->PutCollect(L"��ע", _variant_t(G::PR[i].remark));
				G::db.m_pRecordset->Update();
			}
			catch (_com_error a)
			{
				CString  err;
				err = a.ErrorMessage();
				AfxMessageBox(err + L"ʧ��");
			}
			//}        //if

		}         //for
	}
	catch (_com_error c)
	{
		CString  err;
		err = c.ErrorMessage();
		AfxMessageBox(err + L"ʧ��");
		return -1;
	}
	//CString		str;
	//CString    str2 = L"������";
	//CString    str3 = L"����ֵ";
	//CString    str4 = L"�������ֵ";
	//CString    str5 = L"������Сֵ";
	//CString    str6 = L"��ע";


	//try
	//{

	//	/////////////////////////////////////    д���˶�����    ////////////////////////////////////
	//	for (int i = 0; i < 30; i++)
	//	{
	//		int j = i;
	//		//������
	//		str2.Format(L"������%d", i);
	//		::WritePrivateProfileString(L"�˶�����", str2, G::PR[j].paraname, G::ParaFileName);
	//		//����ֵ
	//		str3.Format(L"����ֵ%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paravalue);
	//		::WritePrivateProfileString(L"�˶�����", str3, str, G::ParaFileName);
	//		//���ֵ
	//		str4.Format(L"���ֵ%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMaxValue);
	//		::WritePrivateProfileString(L"�˶�����", str4, str, G::ParaFileName);
	//		//��Сֵ
	//		str5.Format(L"��Сֵ%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMinValue);
	//		::WritePrivateProfileString(L"�˶�����", str5, str, G::ParaFileName);
	//		//��ע
	//		str6.Format(L"��ע%d", i);
	//		::WritePrivateProfileString(L"�˶�����", str6, G::PR[j].remark, G::ParaFileName);
	//	}

	//	////////////////////////////    д���������       ////////////////////////////////////////
	//	for (int i = 0; i < 60; i++)
	//	{
	//		int j = i + 30;
	//		//������
	//		str2.Format(L"������%d", i);
	//		::WritePrivateProfileString(L"�������", str2, G::PR[j].paraname, G::ParaFileName);
	//		//����ֵ
	//		str3.Format(L"����ֵ%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paravalue);
	//		::WritePrivateProfileString(L"�������", str3, str, G::ParaFileName);
	//		//���ֵ
	//		str4.Format(L"���ֵ%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMaxValue);
	//		::WritePrivateProfileString(L"�������", str4, str, G::ParaFileName);
	//		//��Сֵ
	//		str5.Format(L"��Сֵ%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMinValue);
	//		::WritePrivateProfileString(L"�������", str5, str, G::ParaFileName);
	//		//��ע
	//		str6.Format(L"��ע%d", i);
	//		::WritePrivateProfileString(L"�������", str6, G::PR[j].remark, G::ParaFileName);
	//	}

	//	//////////////////////////////      д���Ӿ�����       ////////////////////////////////////
	//	for (int i = 0; i < 60; i++)
	//	{
	//		int j = i + 90;
	//		//������
	//		str2.Format(L"������%d", i);
	//		::WritePrivateProfileString(L"�Ӿ�����", str2, G::PR[j].paraname, G::ParaFileName);
	//		//����ֵ
	//		str3.Format(L"����ֵ%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paravalue);
	//		::WritePrivateProfileString(L"�Ӿ�����", str3, str, G::ParaFileName);
	//		//���ֵ
	//		str4.Format(L"���ֵ%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMaxValue);
	//		::WritePrivateProfileString(L"�Ӿ�����", str4, str, G::ParaFileName);
	//		//��Сֵ
	//		str5.Format(L"��Сֵ%d", i);
	//		str.Format(L"%.3lf", G::PR[j].paraMinValue);
	//		::WritePrivateProfileString(L"�Ӿ�����", str5, str, G::ParaFileName);
	//		//��ע
	//		str6.Format(L"��ע%d", i);
	//		::WritePrivateProfileString(L"�Ӿ�����", str6, G::PR[j].remark, G::ParaFileName);
	//	}
	//	
	//	//AfxMessageBox(L"��������ɹ���");
	//}
	//catch (...)
	//{
	//	AfxMessageBox(L"ͨ�ò�������ʧ��!");
	//	return -1;
	//}
	return 0;
}


// ����IO�˿���
int G::WriteIOPara()
{
	CString    str;
	try
	{
		////////////////////////////////////      д�����빦�ܿ���       /////////////////////////////////
		for (int i = 0; i < 16; i++)
		{
			str.Format(L"%d�������", i);
			::WritePrivateProfileString(L"InputPort", str, G::MCard.I[i].lpwstrPortName, G::ParaFileName);
		}
		/////////////////////////////////        д��������ܿ���       /////////////////////////////////
		for (int i = 0; i < 16; i++)
		{
			str.Format(L"%d�������", i);
			::WritePrivateProfileString(L"OutPort", str, G::MCard.O[i].lpwstrPortName, G::ParaFileName);
		}
	}
	catch (...)
	{
		AfxMessageBox(L"IO����������ʧ��!");
		return -1;
	}
	return 0;
}

//�������
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
		AfxMessageBox(L"��������ȷ�������ţ�");
		break;
	}
	return 0;
}


// д��������Ϣ
int G::WriteProduction()
{
	try
	{
		////////////////////////////////    д����������          ///////////////////////////////////
		for (int i = 0; i < 6; i++)
		{
			int j = i + PI_TotalCounts;
			CString  str;
			str.Format(L"%.2lf", G::PR[j].paravalue);
			::WritePrivateProfileString(L"��������", G::PR[j].paraname, str, G::ParaFileName);
		}
	}
	catch (...)
	{
		AfxMessageBox(L"��������д��ʧ��!");
		return -1;
	}
	return 0;
}


// ɾ����������
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

//������ɫ
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

//���ÿؼ���ɫ
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

// doubleתstring��
CString G::double_string(double xx)
{
	CString  mm;
	mm.Format(L"%.3lf", xx);
	return mm;
}


// stringתdouble��
double G::string_double(CString xx)
{
	double   mm;
	mm = _wtof(xx);
	return mm;
}


// ͼ����1�����Ʒ�ߴ磩
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
		//����任
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

			//�����⾶
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
			//����ת��double����������
			//sizelink.Diameter = hv_Distance_Measure_01_0[0].D()*0.01465;

			//�����ܳ�
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
			//ת�����ݸ�ʽ����������
			//sizelink.TotalLength = hv_Distance_Measure_02_0[0].D()*0.01465;
		}
		///û��ƥ�䵽ģ��
		else
		{
			G::disp_message(G::HW1, "δƥ�䵽ģ��", "image",100, 100, "red", "false");
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
	//G::LeftDisposecounts1++;                   //��������һ
	//G::m_criticalsection_sizelist1.Lock();
	//G::Sizelist1.AddTail(sizelink);
	//G::m_criticalsection_sizelist1.Unlock();
	//��������������ʾ
	//G::LeftLength = sizelink.TotalLength;
	//G::LeftDiameter = sizelink.Diameter;

	/*SetDraw(Global::HW2, "margin");
	SetColor(Global::HW2, "green");*/

	return 0;
}

// ͼ����2���Ҳ�Ʒ�ߴ磩
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
	//	//����任
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

	//		//�����⾶
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
	//		//����ת��double����������
	//		sizelink.Diameter = hv_Distance_Measure_01_0[0].D()*0.01465;

	//		//�����ܳ�
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
	//		//ת�����ݸ�ʽ����������
	//		sizelink.TotalLength = hv_Distance_Measure_02_0[0].D()*0.01465;
	//	}
	//	///û��ƥ�䵽ģ��
	//	else
	//	{
	//		G::disp_message(G::HW3, "δƥ�䵽ģ��", "image",160, 100, "red", "false");
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
	////��������
	//G::RightLength = sizelink.TotalLength;
	//G::RightDiameter = sizelink.Diameter;
	//
	///*SetDraw(Global::HW2, "margin");
	//SetColor(Global::HW2, "green");*/

	return 0;
}

// �������ݿ�
int G::CreateDatabase(CString dbname)
{
	G::db._conn_ = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + dbname + L".accdb"; 

	_bstr_t  _conn_(G::db._conn_);
	try
	{

		/*adModeUnknown  ȱʡ����ǰ�����Ȩδ����
			adModeRead  ֻ��
			adModeWrite  ֻд
			adModeReadWrite  ���Զ�д
			adModeShareDenyRead  ��ֹ���� Connection �����Զ�Ȩ�޴�����
			adModeShareDenyWrite  ��ֹ���� Connection ������дȨ�޴�����
			adModeShareExclusive  ��ֹ���� Connection �����Զ�дȨ�޴�����
			adModeShareDenyNone  ��ֹ���� Connection �������κ�Ȩ�޴�����*/
		G::db.m_pConnection = NULL;
		//����m_pConnectionʵ��
		G::db.m_pConnection.CreateInstance("ADODB.Connection");
		//����m_pConnection.CreateInstance(__uuidof(Connection));
		//ʹ��'.'������->����m_Connectionʵ����Ȼ��m_pConnection->open�����������ӡ�
		G::db.m_pConnection->ConnectionTimeout = 3;
		//����ACCESS2000   
		/*Microsoft.Jet.OLEDB.4.0*/
		//����m_pRecordsetʵ��
		G::db.m_pRecordset.CreateInstance(__uuidof(Recordset));
		G::db.m_pConnection->Open(_conn_, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		try
		{
			G::db.m_pCatalog = NULL;
			G::db.m_pCatalog.CreateInstance(__uuidof(Catalog));
			//��������Դ
			G::db.m_pCatalog->Create(_conn_);
			//�ٴδ�
			G::db.m_pConnection->Open(_conn_, "", "", adModeUnknown);
		}
		catch (_com_error f)
		{
			//AfxMessageBox(f.Description() + _T("\n���ݿ�����ʧ��"));
			CString  error;
			error = f.ErrorMessage();
			AfxMessageBox(error + L"\n���ݿ�����ʧ��");
		}
		
	}

	return 0;
}


// �����ݿ��д������
int G::CreateTable(CString TableName)
{
	CString   str;
	try
	{
		str = L"CREATE TABLE " + TableName + L"(��� DOUBLE, ���� Text,������ Text,����ֵ DOUBLE,���ֵ DOUBLE,��Сֵ DOUBLE,��ע Text)";
		_bstr_t exeword(str);
		try
		{
			//���ַ�ʽ�����ݿ⣬�������ַ�ʽ�������ݿ�
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
		AfxMessageBox(errormesg + L"���ݱ���ʧ��");
		return -1;
	}
	

	return 0;
}



// д��һ�����͵Ĳ���
//0:�˶�����   1:�������  2���Ӿ�����
int G::WriteSerialPara(short SerialNo)
{
	///�ɾ�̬����������������
	int count, beginno;
	switch (SerialNo)
	{
	case 0:         //�˶�����
		beginno = 0;
		count = MotionParaCounts;
		break;
	case 1:         //�������
		beginno = MotionParaCounts; 
		count = CameraParaCounts;
		break;
	case 2:             //�Ӿ�����
		count = VisionParaCounts;
		beginno = MotionParaCounts+CameraParaCounts;
		break;
	}
	G::db.m_pRecordset->MoveFirst();
	G::db.m_pRecordset->Move(beginno);                //�Ƶ�ָ��λ��(��1��ʼ����)
	for (int i = beginno; i < count + beginno; i++)
	{
		try
		{
			G::db.m_pRecordset->PutCollect(L"����", _variant_t(G::PR[i].type));
			G::db.m_pRecordset->PutCollect(L"������", _variant_t(G::PR[i].paraname));
			G::db.m_pRecordset->PutCollect(L"����ֵ", _variant_t(G::double_string(G::PR[i].paravalue)));
			G::db.m_pRecordset->PutCollect(L"���ֵ", _variant_t(G::double_string(G::PR[i].paraMaxValue)));
			G::db.m_pRecordset->PutCollect(L"��Сֵ", _variant_t(G::double_string(G::PR[i].paraMinValue)));
			G::db.m_pRecordset->PutCollect(L"��ע", _variant_t(G::PR[i].remark));

			G::db.m_pRecordset->Update();
			G::db.m_pRecordset->MoveNext();
		}
		catch (_com_error c)
		{
			CString  err;
			err = c.ErrorMessage();
			//AfxMessageBox(err + L"ʧ��");
			return -1;
		}
	}
	return 0;
}


// �ٴζ�ȡ����ʱʹ��
int G::ReadPara2()
{
	try
	{
		G::db.m_pRecordset->MoveFirst();
		for (int i = 0; i < 50; i++)
		{
			try
			{
				G::PR[i].type = G::db.m_pRecordset->GetCollect(L"����");
				G::PR[i].paraname = G::db.m_pRecordset->GetCollect(L"������");
				G::PR[i].paravalue = G::db.m_pRecordset->GetCollect(L"����ֵ");
				G::PR[i].paraMaxValue = G::db.m_pRecordset->GetCollect(L"���ֵ");
				G::PR[i].paraMinValue = G::db.m_pRecordset->GetCollect(L"��Сֵ");
				G::PR[i].remark = G::db.m_pRecordset->GetCollect(L"��ע");

				G::db.m_pRecordset->MoveNext();
			}
			catch (_com_error c)
			{
				CString  err;
				err = c.ErrorMessage();
				AfxMessageBox(err + L"ʧ��,���������");
				return -1;
			}
		}             //for
	}
	catch (_com_error a)
	{
		CString  err;
		err = a.ErrorMessage();
		AfxMessageBox(err + L"ʧ��,���������1");
		return -1;
	}
	return 0;
}
