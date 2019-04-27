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

		CString          Camname                                  ;    //�������
		CString          GainAuto                                 ;	   //�Զ�����
		CString          ExposureAuto                             ;	   //�Զ��ع�
		CString          TriggerMode                              ;	   //����ģʽ
		CString          TriggerSelector                          ;	   //ץͼģʽ
		CString          TriggerSource                            ;	   //����Դ
		CString          TriggerActivation                        ;	   //�����ź�
		CString          PixelFormat                              ;	   //ͼ���ʽ
		CString          GammaSelector                            ;	   //Gamma����ģʽ
		int              ExposureTimeabs                          ;	   //�ع�ֵ
		float            LineDebouncerTimerAbs                    ;	   //�˲�ʱ��
		float            GammaValue                               ;	   //Gammaֵ
		int              GainValue                                ;	   //����ֵ
		int              BlackLevel                               ;	   //��ƽ��ֵ
		int              Width                                    ;	   //ͼ����
		int              Height                                   ;	   //ͼ��߶�
		int              XOffset                                  ;	   //Xƫ��ֵ
		int              YOffset                                  ;	   //Yƫ��ֵ
		int              TriggerDelayabs                          ;	   //������ʱ
		int              GevHeartbeatTimeout                      ;	   //����ʱ��
		bool             CenterX                                  ;	   //����Xƫ��
		bool             CenterY                                  ;	   //����Yƫ��
		bool             GammaEnable                              ;	   //������Gammaֵ


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