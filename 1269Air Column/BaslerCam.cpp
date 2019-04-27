#include "StdAfx.h"
#include "BaslerCam.h"

BaslerCam::BaslerCam(void)
{

}

BaslerCam::~BaslerCam(void)
{

}

//Initialize Camera and return a const bool
bool BaslerCam::IniCam(int CamNub,CString *Str)
{
	PylonInitialize();
	try
	{
		int Nub;
		DeviceInfoList_t devices;
		CTlFactory &tlFactory = CTlFactory::GetInstance();
		Nub=tlFactory.EnumerateDevices(devices);
		CInstantCameraArray Ci_cam(min(devices.size(),CamNub));
		if(!CamNub==Nub)
		{
			*Str=L"Some Camera Not Found !";
			return false;
		}
	}
	catch(GenICam::GenericException &yy)
	{
		CString cc;
		cc=yy.GetDescription();
		*Str=cc;
		return false;
	}
	return true;
}


//Open Camera and return a const bool
bool BaslerCam::OpenCam(CString CamName,CInstantCamera *m_InstantCamera,CString *Str)
{
	try
	{
		CDeviceInfo  cam;
		cam.SetUserDefinedName(Pylon::String_t(CamName));
		(*m_InstantCamera).Attach(CTlFactory::GetInstance().CreateDevice(cam));
		if((*m_InstantCamera).IsOpen())
		{
			(*m_InstantCamera).Close();
		}  
		(*m_InstantCamera).Open();  
	}
	catch(GenICam::GenericException &yy)
	{
		CString cc;
		cc=yy.GetDescription();
		*Str=cc;
		AfxMessageBox(CamName+" Open Fail");
		return false;
	}
	return true;
}


//Configure Camera and return a const bool
bool BaslerCam::ConfigCam(CInstantCamera *m_InstantCamera,CString *Str)
{
	try
	{
		 //获取相机的属性节点
		INodeMap &CNodeMap=(*m_InstantCamera).GetNodeMap();    
		//1		//配置相机的死亡时间
		CIntegerPtr PtrHeartbeat( CNodeMap.GetNode("GevHeartbeatTimeout"));
		if ( PtrHeartbeat.IsValid())
		{
			PtrHeartbeat->SetValue(campara.GevHeartbeatTimeout);
		}
//2		//配置相机的曝光模式
		CEnumerationPtr PtrExposureAuto( CNodeMap.GetNode("ExposureAuto"));                   
		if ( IsWritable( PtrExposureAuto))
		{
			if(campara.ExposureAuto.Trim()==L"Off")
			{
				PtrExposureAuto->FromString("Off");
			}
			else if(campara.ExposureAuto.Trim()==L"On")
			{
				PtrExposureAuto->FromString("On");
			}
		}

//3		//配置相机的曝光值
		const CFloatPtr PtrExposureTimex=CNodeMap.GetNode("ExposureTimeAbs"); 
		if(PtrExposureTimex.IsValid())
		{
		     PtrExposureTimex->SetValue(campara.ExposureTimeabs);
		}
		//配置相机的触发延时
		const CFloatPtr PtrTriggerDelayabs=CNodeMap.GetNode("TriggerDelayAbs"); 
		if(PtrTriggerDelayabs.IsValid())
		{
			 PtrTriggerDelayabs->SetValue(campara.TriggerDelayabs);
		}

//4		//配置相机的抓图方式
		CEnumerationPtr PtrTriggerSelector( CNodeMap.GetNode("TriggerSelector"));                   
		if ( IsWritable( PtrTriggerSelector))
		{
			if(campara.TriggerSelector.Trim()==L"FrameStart")
			{
			    PtrTriggerSelector->FromString("FrameStart");
			}
			else if(campara.TriggerSelector.Trim()==L"Acquisitionstart")
			{
				PtrTriggerSelector->FromString("AcquisitionStart");
			}
		}

//5		//配置相机的触发源
		CEnumerationPtr PtrTriggerSource( CNodeMap.GetNode( "TriggerSource"));                   
		if ( IsWritable( PtrTriggerSource))
		{
			if(campara.TriggerSource.Trim()==L"Line1")
			{
				PtrTriggerSource->FromString("Line1");
			}
			else if(campara.TriggerSource.Trim()==L"Triggersoftware")
			{
				PtrTriggerSource->FromString("TriggerSoftware");
			}
		}

//6		//配置相机的触发过滤时间
		const CFloatPtr PtrLineDebouncerTimerAbs=CNodeMap.GetNode("LineDebouncerTimerAbs");               
		if(PtrLineDebouncerTimerAbs.IsValid())
		{
		    PtrLineDebouncerTimerAbs->SetValue(campara.LineDebouncerTimerAbs);
		}

//7		//配置相机的触发模式
		CEnumerationPtr PtrTriggerMode( CNodeMap.GetNode( "TriggerMode"));                   
		if ( IsWritable( PtrTriggerMode))
		{
			if(campara.TriggerMode.Trim()==L"Off")
			{
			    PtrTriggerMode->FromString("Off");   
			}
			else if (campara.TriggerMode.Trim()==L"On")
			{
				PtrTriggerMode->FromString("On");  
			}
		}

//8		//配置相机的信号模式
		CEnumerationPtr PtrTriggerActivation( CNodeMap.GetNode( "TriggerActivation"));                   
		if ( IsWritable( PtrTriggerActivation))
		{
			if(campara.TriggerActivation.Trim()==L"RisingEdge")
			{
				PtrTriggerActivation->FromString("RisingEdge");   
			}
			else if (campara.TriggerMode.Trim()==L"FallingEdge")
			{
				PtrTriggerMode->FromString("FallingEdge");  
			}
		}

//9		//配置相机的增益模式
		CEnumerationPtr PtrgainAuto( CNodeMap.GetNode( "GainAuto"));                     
		if ( IsWritable( PtrgainAuto))
		{
			if(campara.GainAuto.Trim()==L"Off")
			{
				PtrgainAuto->FromString("Off");   
			}
			else if (campara.GainAuto.Trim()==L"On")
			{
				PtrgainAuto->FromString("On");  
			}
		}

//10	//配置相机的增益值
		CIntegerPtr PtrgainRaw( CNodeMap.GetNode( "GainRaw"));
		if ( PtrgainRaw.IsValid())
		{
			if(campara.GainValue<PtrgainRaw->GetMin())
			{
				PtrgainRaw->SetValue(PtrgainRaw->GetMin()+campara.GainValue);
			}
			else if(campara.GainValue<PtrgainRaw->GetMax()&&campara.GainValue>PtrgainRaw->GetMin())
			{
				PtrgainRaw->SetValue(campara.GainValue);
			}
		}

//11	//配置相机的图像格式
		CEnumerationPtr PtrPixelFormat( CNodeMap.GetNode( "PixelFormat"));                     
		if ( IsWritable( PtrPixelFormat))
		{
			if(campara.PixelFormat.Trim()==L"Mono8")
			{
				PtrPixelFormat->FromString("Mono8");   
			}
			else if(campara.PixelFormat.Trim()==L"YUV422Packed")
			{
				PtrPixelFormat->FromString("YUV422Packed");  
			}
		}

//12	//配置相机伽马模式
		CEnumerationPtr PtrGammaSelector( CNodeMap.GetNode( "GammaSelector"));           
		if ( IsWritable( PtrGammaSelector))
		{
			if(campara.GammaSelector.Trim()==L"User")
			{
				PtrGammaSelector->FromString("User");   
			}
			else if (campara.GammaSelector.Trim()==L"sRGB")
			{
				PtrGammaSelector->FromString("sRGB");  
			}
		}

//13	//配置相机伽马使能
		CBooleanPtr PtrGammaEnable (CNodeMap.GetNode( "GammaEnable"));
		if(PtrGammaEnable.IsValid())
		{
			PtrGammaEnable->SetValue(campara.GammaEnable);
		}

//14	//配置相机伽马值
		const CFloatPtr PtrGamma( CNodeMap.GetNode( "Gamma"));
		if ( PtrGamma.IsValid())
		{
			PtrGamma->SetValue(campara.GammaValue);
		}

//15	//配置相机AOI中心X方向提取
		CBooleanPtr PtrCenterX (CNodeMap.GetNode( "CenterX"));
		if(PtrCenterX.IsValid())
		{
			PtrCenterX->SetValue(campara.CenterX);
		}

//16	//配置相机AOI中心Y方向提取
		CBooleanPtr PtrCenterY (CNodeMap.GetNode( "CenterY"));
		if(PtrCenterY.IsValid())
		{
			PtrCenterY->SetValue(campara.CenterY);
		}

//17	//配置相机AOIX方向偏移值
		CIntegerPtr PtrXOffset( CNodeMap.GetNode( "OffsetX"));
		if ( PtrXOffset.IsValid())
		{
			PtrXOffset->SetValue(campara.XOffset);
		}

//18	//配置相机AOIY方向偏移值
		CIntegerPtr PtrYOffset( CNodeMap.GetNode( "OffsetY"));
		if ( PtrYOffset.IsValid())
		{
			PtrYOffset->SetValue(campara.YOffset);
		}

//19	//配置相机AOIX方向宽度
		CIntegerPtr PtrWidth( CNodeMap.GetNode( "Width"));
		if ( PtrWidth.IsValid())
		{
			PtrWidth->SetValue(campara.Width);
		}

//20	//配置相机AOIY方向高度
		CIntegerPtr PtrHeight( CNodeMap.GetNode( "Height"));
		if ( PtrHeight.IsValid())
		{
			PtrHeight->SetValue(campara.Height);
		}

	}
	catch(GenICam::GenericException &yy)
	{
		CString cc;
		cc=yy.GetDescription();
		*Str=cc;
		return false;
	}
	return true;
}


//Catch One Image One time When Called and return a const bool
bool BaslerCam::CatchOneImage(CInstantCamera *m_InstantCamera,HObject *image,CString *Str)
{
	try
	{
		HObject ho_image;
		if((*m_InstantCamera).IsOpen())
		{
			(*m_InstantCamera).StartGrabbing();                                                                                                       //开始抓取
			CGrabResultPtr ptrGrabResult;
			(*m_InstantCamera).RetrieveResult( -1, ptrGrabResult, TimeoutHandling_ThrowException);                      //抓取图片
			(*m_InstantCamera).StopGrabbing();
			if(ptrGrabResult->GrabSucceeded())                                                                                                    //判断是否抓取成功
			{
				GenImage1Extern(&(ho_image),"byte",Hlong(ptrGrabResult->GetWidth()),Hlong(ptrGrabResult->GetHeight()),Hlong(ptrGrabResult->GetBuffer()),0); //转Buffer为Image类型                                                           //设置显示图片的区域
			    CopyImage(ho_image,&(*image));
			}
		}
		else
		{
			*Str=L"Catch image fail !";
		}
	}
	catch(GenICam::GenericException &yy)
	{
		CString cc;
		cc=yy.GetDescription();
		*Str=cc;
		return false;
	}
	return true;
}

//Catch One ColorImage One time When Called and return a const bool
bool BaslerCam::CatchOneImageRGB(CInstantCamera *m_InstantCamera,HObject *image,CString *Str)
{
	CGrabResultPtr ptrGrabResult;
	CPylonImage TargetImage;
	CImageFormatConverter Converter;
	Converter.OutputPixelFormat=PixelType_BGR8packed;
	try
	{
		HObject ho_image;
		if((*m_InstantCamera).IsOpen())
		{
			(*m_InstantCamera).StartGrabbing();                                                                                                       //开始抓取
			(*m_InstantCamera).RetrieveResult( -1, ptrGrabResult, TimeoutHandling_ThrowException);                      //抓取图片
			Converter.Convert(TargetImage,ptrGrabResult);
			byte *pImageBuffer=(byte*)TargetImage.GetBuffer();
			(*m_InstantCamera).StopGrabbing();
			if(ptrGrabResult->GrabSucceeded())                                                                                                    //判断是否抓取成功
			{
				GenImageInterleaved(&(ho_image),Hlong(pImageBuffer),"bgr",Hlong(ptrGrabResult->GetWidth()),Hlong(ptrGrabResult->GetHeight()),1,"byte",Hlong(ptrGrabResult->GetWidth()),Hlong(ptrGrabResult->GetHeight()),0,0,-1,0); //转Buffer为Image类型                                                           //设置显示图片的区域
				MirrorImage(ho_image,&ho_image,"row");
				CopyImage(ho_image,&(*image));
			}
		}
		else
		{
			*Str=L"Catch image fail !";
		}
	}
	catch(GenICam::GenericException &yy)
	{
		CString cc;
		cc=yy.GetDescription();
		*Str=cc;
		return false;
	}
	return true;
}


//Catch  Image All The Way Until the bool CatchStop Was False  and return a const bool
bool BaslerCam::CatchImageContinue(CInstantCamera *m_InstantCamera,bool CatchStop,HObject *image,CString *Str)
{
	try
	{
		//while (CatchStop)
		//{
		//	if(m_InstantCamera.IsOpen())
		//	{
		//		m_InstantCamera.StartGrabbing();                                                                                                       //开始抓取
		//		CGrabResultPtr ptrGrabResult;
		//		m_InstantCamera.RetrieveResult( -1, ptrGrabResult, TimeoutHandling_ThrowException);                      //抓取图片
		//		m_InstantCamera.StopGrabbing();
		//		if(ptrGrabResult->GrabSucceeded())                                                                                                    //判断是否抓取成功
		//		{
		//			GenImage1Extern(&(*image),"byte",Hlong(ptrGrabResult->GetWidth()),Hlong(ptrGrabResult->GetHeight()),Hlong(ptrGrabResult->GetBuffer()),0);                                                           //设置显示图片的区域
		//		}
		//	}
		//	else
		//	{
		//		CatchStop=false;
		//		*Str=L"Catch image fail !";
		//	}
		//}
		*Str=L"Nothing";
		return false;
	}
	catch(GenICam::GenericException &yy)
	{
		CString cc;
		cc=yy.GetDescription();
		*Str=cc;
		return false;
	}
	return true;
}


//Close Camera and return a const bool
bool BaslerCam::CloseCam(CInstantCamera *m_InstantCamera,CString *Str)
{
	try
	{
	    if((*m_InstantCamera).IsOpen())
	    {
	    	(*m_InstantCamera).Close();
	    	(*m_InstantCamera).DetachDevice();
	    	(*m_InstantCamera).DestroyDevice();
	    }
	}
	catch(GenICam::GenericException &yy)
	{
		CString cc;
		cc=yy.GetDescription();
		*Str=cc;
		return false;
	}
	return true;
}

//Close Camera and return a const bool
bool BaslerCam::FrameRate(CInstantCamera *m_InstantCamera,float FrameRateAbs,CString *Str)
{
	try
	{
		INodeMap &CNodeMap=(*m_InstantCamera).GetNodeMap(); 
		CFloatPtr PtrFramerateabs( CNodeMap.GetNode( "ResultingFrameRateAbs"));
		FrameRateAbs=PtrFramerateabs->GetValue();
	}
	catch(GenICam::GenericException &yy)
	{
		CString cc;
		cc=yy.GetDescription();
		*Str=cc;
		return false;
	}
	return true;
}