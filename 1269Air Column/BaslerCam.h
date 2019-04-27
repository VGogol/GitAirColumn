#include "stdafx.h"
//#include "G.h"

//Basler Camera Configure create on 2018.8.14 

//V1.1.1.0
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#include <pylon/PylonGUI.h>
#endif


using namespace std;
using namespace Pylon;
using namespace HalconCpp;
using namespace GenApi;


class BaslerCam
{

public:
	BaslerCam(void);

	~BaslerCam(void);

	typedef struct CamPara_
	{

		CString          Camname                                  ;    //相机名称
		CString          GainAuto                                 ;	   //自动增益
		CString          ExposureAuto                             ;	   //自动曝光
		CString          TriggerMode                              ;	   //触发模式
		CString          TriggerSelector                          ;	   //抓图模式
		CString          TriggerSource                            ;	   //触发源
		CString          TriggerActivation                        ;	   //触发信号
		CString          PixelFormat                              ;	   //图像格式
		CString          GammaSelector                            ;	   //Gamma设置模式
		int              ExposureTimeabs                          ;	   //曝光值
		float            LineDebouncerTimerAbs                    ;	   //滤波时间
		float            GammaValue                               ;	   //Gamma值
		int              GainValue                                ;	   //增益值
		int              BlackLevel                               ;	   //白平衡值
		int              Width                                    ;	   //图像宽度
		int              Height                                   ;	   //图像高度
		int              XOffset                                  ;	   //X偏移值
		int              YOffset                                  ;	   //Y偏移值
		int              TriggerDelayabs                          ;	   //触发延时
		int              GevHeartbeatTimeout                      ;	   //心跳时间
		bool             CenterX                                  ;	   //中心X偏移
		bool             CenterY                                  ;	   //中心Y偏移
		bool             GammaEnable                              ;	   //允许设Gamma值


		void Init()
		{

			Camname                                   =L""        ;          
			GainAuto                       		      =L""        ; 
			ExposureAuto                 	          =L""        ; 
			TriggerMode                  		      =L""        ; 
			TriggerSelector               		      =L""        ; 
			TriggerSource                 		      =L""        ; 
			TriggerActivation            	          =L""        ; 
			PixelFormat                    		      =L""        ; 
			GammaSelector              		          =L""        ; 
			ExposureTimeabs                           =0          ; 
			LineDebouncerTimerAbs 		              =0          ;
			GammaValue                 		          =0          ;
			GainValue                      		      =0          ;
			BlackLevel                      		  =0          ;
			Width                             		  =0          ;
			Height                            		  =0          ;
			XOffset                           		  =0          ;
			YOffset                           		  =0          ;
			TriggerDelayabs              	          =0          ;
			GevHeartbeatTimeout    		              =0          ;
			CenterX                                   =false      ;
			CenterY                          		  =false      ;
			GammaEnable                		          =false      ;

		}

	}CamPara; 

	//public function
public:

	//Initialize Camera and return a const bool
	bool   IniCam(int CamNub,CString *Str);

	//Open Camera and return a const bool
	bool   OpenCam(CString CamName,CInstantCamera *m_InstantCamera,CString *Str);

	//Configure Camera and return a const bool
	bool   ConfigCam(CInstantCamera *m_InstantCamera,CString *Str);

	//Catch One Image One time When Called and return a const bool
	bool   CatchOneImage(CInstantCamera *m_InstantCamera,HObject *image,CString *Str);

	//Catch One ColorImage One time When Called and return a const bool
	bool   CatchOneImageRGB(CInstantCamera *m_InstantCamera,HObject *image,CString *Str);

	//Catch  Image All The Way Until the bool CatchStop Was False  and return a const bool
	bool   CatchImageContinue(CInstantCamera *m_InstantCamera,bool CatchStop,HObject *image,CString *Str);

	//Close Camera and return a const bool
	bool   CloseCam(CInstantCamera *m_InstantCamera,CString *Str);

	//Get the Camera Framerate
	bool   FrameRate(CInstantCamera *m_InstantCamera,float FrameRateAbs,CString *Str);

	//public variable

public:

	CamPara campara;
};