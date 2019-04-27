
// 1269Air ColumnDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "1269Air ColumnDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using   namespace    HalconCpp;
//一个像素14.65um，0.01465mm             252000pulse一圈

//轴动作
BOOL  CALLBACK  myAxisSafeFunc(UINT uAxis)
{
	//if (uAxis == AXIS_PUTPIN)
	//{
	//	if (1 == G::MCard.I[GetPinUpDownACT].sValue || 0 == G::MCard.I[GetPinUpDownORG].sValue)
	//	{
	//		G::SendMessagebox(L"取料气缸不在原位，不能动作！", L"警告", MB_ICONWARNING);
	//		return FALSE;
	//	}
	//}
	return TRUE;
}

//IO输出函数
BOOL   CALLBACK  myDoSafeFunc(UINT uIOIndex, UINT uValue)
{
	/*if ((uIOIndex == GetPinUpDownCylinder) && (uValue == 1))
	{
		long   pos = dmc_get_position(0, AXIS_PUTPIN);
		if ((pos > G::PR[MP_GetPinServoGetPinPos].paravalue - 2000) && (pos < G::PR[MP_GetPinServoGetPinPos].paravalue + 2000))
		{
			if ((1 == G::MCard.I[InPinDownORG].sValue) || (0 == G::MCard.I[InPinDownACT].sValue) || (0 == G::MCard.O[InPinDownCylinder].sValue))
			{
				G::SendMessagebox(L"落料气缸没在原位，升降气缸不能动作！", L"警告", MB_ICONWARNING);
				return FALSE;
			}
		}
	}*/

	return TRUE;
}

//相机1抓图线程（左产品总长）
DWORD     CameraGrab1(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[0] = 0;
	HObject hobjTmp;
	//定义一个新的列表元素
	ImageLinked  p1;
	//HTuple  w, h;
	while ((0 == pdlg->m_exitThread[0]))
	{
		if (1/*G::bCanGrab*//* || FALSE==G::bRunning*/)
		{
			try
			{	//生成 密码令牌 ，持有此令牌的才有机会获得本次抓图
				//LONGLONG llToken = ::GetTickCount();
				//HalconCpp::GenEmptyObj(&hobjTmp);这个函数太慢，要16ms
				//开始抓图
				//--------------------------------------------------------------------------------------------
				//同步采集                  异步采集为grabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle1);//阻塞函数 。。。开始。。。。。。。。。。。。。。。
				HalconCpp::DispImage(hobjTmp, G::HW1);
				
				/////////////////////////////////////运行过程中才把图像插入链表//////////////////////
				if (1 == G::bCanGrab)
				{
					//将新元素插入列表
					//将抓取到的图像存入列表尾
					p1.image = hobjTmp;
					G::m_criticalsection_imagelist1.Lock();
					G::Imagelist1.AddTail(p1);
					G::m_criticalsection_imagelist1.Unlock();
					G::grab1counts++;
				}
				//--------------------------------------------------------------------------------------------
				int bSave1 = ((CButton*)pdlg->GetDlgItem(IDC_saveimage1))->GetCheck();
				if (bSave1)
				{
					pdlg->imageno1++;
					CStringA str;
					str.Format("D:/savedphoto1/GO_%d.tiff", pdlg->imageno1);
					WriteImage(hobjTmp, "tiff", 0, str.GetString());
				}
			}
			catch (HalconCpp::HException &HDevExpDefaultException)
			{
				TRACE("Grab1 Failed!! \r\n");
				G::bRunning = 0;
				pdlg->MessageBox(L"相机1抓图出错,请重启软件!",L"警告",MB_ICONWARNING);
				pdlg->m_exitThread[0] = 1;
			}
			Sleep(1);
			//if (pdlg->m_exitThread[0] > 0)
			//{//退出线程
			//	TRACE("Camera1Grab Thread Exiting \r\n");
			//	break;
			//}
		}                //if(cangrab)
		Sleep(1);
	}
	TRACE("Camera1Grab Thread Exiting \r\n");
	pdlg->m_exitThread[0] = 1;
	return TRUE;
}

//相机2抓图线程（左产品左端外径）
DWORD     CameraGrab2(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[1] = 0;
	HObject hobjTmp;
	ImageLinked  p2;              	//定义一个新的列表元素
	//HTuple  w, h;
	while ((0 == pdlg->m_exitThread[1]))
	{
		if (TRUE /*== G::bCanGrab*/)
		{
			try
			{
				//HalconCpp::GenEmptyObj(&hobjTmp);这个函数太慢，要16ms
				//开始抓图
				//--------------------------------------------------------------------------------------------
				//同步采集                  异步采集为grabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle2);//阻塞函数 。。。开始。。。。。。。。。。。。。。。
				//--------------------------------------------------------------------------------------------
				int bSave2 = ((CButton*)pdlg->GetDlgItem(IDC_saveimage2))->GetCheck();
				if (bSave2)
				{
					pdlg->imageno2++;
					CStringA str;
					str.Format("D:/savedphoto2/GO_%d.tiff", pdlg->imageno2);
					WriteImage(hobjTmp, "tiff", 0, str.GetString());
				}
				//G::GTCard.SetDoBitReverse(OP_Buzzer,1,50);	//触发脉冲
				/*iCountTest++;
				pDlg->SetDlgItemInt(IDC_COUNTTEST,iCountTest);*/
			/*	GetImageSize(hobjTmp, &w, &h);
				SetPart(G::HW2, 0, 0, h, w);*/
				HalconCpp::DispImage(hobjTmp, G::HW2);
				if (G::bCanGrab)
				{
					//将新元素插入列表
					//将抓取到的图像存入列表尾
					p2.image = hobjTmp;
					G::m_criticalsection_imagelist2.Lock();
					G::Imagelist2.AddTail(p2);
					G::m_criticalsection_imagelist2.Unlock();
					G::grab2counts++;
				}
			}
			catch (HalconCpp::HException &HDevExpDefaultException)
			{
				TRACE("Camera2 Grab Failed!! \r\n");
				pdlg->m_exitThread[1] = 1;
				G::bRunning = 0;
				pdlg->MessageBox(L"相机2抓图出错,请重启软件!",L"警告",MB_ICONWARNING);
				/*HalconCpp::CloseFramegrabber(G::hv_AcqHandle2);
				Sleep(100);
				OpenFramegrabber("GigEVision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
					-1, "false", "default", "UpCam", 0, -1, &G::hv_AcqHandle2);
				SetFramegrabberParam(G::hv_AcqHandle2, "grab_timeout", -1);
				SetFramegrabberParam(G::hv_AcqHandle2, "TriggerMode", "Off");*/
				continue;
			}
			
			Sleep(1);
		}
		Sleep(1);
	}
	pdlg->m_exitThread[1] = 1;
	TRACE("Camera2Grab Thread Exiting \r\n");
	//HalconCpp::CloseFramegrabber(G::hv_AcqHandle2);
	return TRUE;
}

//相机3抓图线程（左产品右端外径）
DWORD     CameraGrab3(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[2] = 0;
	HObject hobjTmp;
	ImageLinked  p3;
	//HTuple  w, h;
	while ((0 == pdlg->m_exitThread[2]))
	{
		if (1/*TRUE == G::bCanGrab*/)
		{
			try
			{
				//HalconCpp::GenEmptyObj(&hobjTmp);这个函数太慢，要16ms
				//开始抓图
				//--------------------------------------------------------------------------------------------
				//同步采集                  异步采集为grabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle3);//阻塞函数 。。。开始。。。。。。。。。。。。。。。
				HalconCpp::DispImage(hobjTmp, G::HW3);
				
				if (G::bCanGrab)
				{
					//将新元素插入列表
					//将抓取到的图像存入列表尾
					p3.image = hobjTmp;
					G::m_criticalsection_imagelist3.Lock();
					G::Imagelist3.AddTail(p3);
					G::m_criticalsection_imagelist3.Unlock();
					G::grab3counts++;
				}
				//--------------------------------------------------------------------------------------------
				int bSave3 = ((CButton*)pdlg->GetDlgItem(IDC_saveimage3))->GetCheck();
				if (bSave3)
				{
					pdlg->imageno3++;
					CStringA str;
					str.Format("D:/savedphoto3/GO_%d.tiff", pdlg->imageno3);
					WriteImage(hobjTmp, "tiff", 0, str.GetString());
				}
			}
			catch (HalconCpp::HException &HDevExpDefaultException)
			{
				TRACE("Camera3 Grab Failed!! \r\n");
				pdlg->m_exitThread[2] = 1;
				G::bRunning = 0;
				pdlg->MessageBox(L"相机3抓图出错,请重启软件!",L"警告",MB_ICONWARNING);
				/*HalconCpp::CloseFramegrabber(G::hv_AcqHandle3);
				Sleep(100);
				OpenFramegrabber("GigEVision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
					-1, "false", "default", "UpCam", 0, -1, &G::hv_AcqHandle3);
				SetFramegrabberParam(G::hv_AcqHandle3, "grab_timeout", -1);
				SetFramegrabberParam(G::hv_AcqHandle3, "TriggerMode", "Off");*/
				continue;
			}
			//Sleep(1);
		}                  ////////if(cangrab)
		Sleep(1);
		//if (pdlg->m_exitThread[2] > 0)
		//{//退出线程
		//	TRACE("Camera1Grab Thread Exiting \r\n");
		//	break;
		//}
	}
	pdlg->m_exitThread[2] = 0;
	TRACE(L"Camera3Grab Thread Exiting \r\n");
	//HalconCpp::CloseFramegrabber(G::hv_AcqHandle3);
	return TRUE;
}

//相机4抓图线程（右产品总长）
DWORD     CameraGrab4(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[3] = 0;
	HObject hobjTmp;
	ImageLinked  p4;
	//HTuple  w, h;
	while ((0 == pdlg->m_exitThread[3]))
	{
		if (1/*TRUE == G::bCanGrab*/)
		{
			try
			{	
				//HalconCpp::GenEmptyObj(&hobjTmp);这个函数太慢，要16ms
				//开始抓图
				//--------------------------------------------------------------------------------------------
				//同步采集                  异步采集为grabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle4);//阻塞函数 。。。开始。。。。。。。。。。。。。。。
				HalconCpp::DispImage(hobjTmp, G::HW4);

				if (G::bCanGrab)
				{
					//将新元素插入列表
					//将抓取到的图像存入列表尾
					p4.image = hobjTmp;
					G::m_criticalsection_imagelist4.Lock();
					G::Imagelist4.AddTail(p4);
					G::m_criticalsection_imagelist4.Unlock();
					G::grab4counts++;
				}
				//--------------------------------------------------------------------------------------------
				int bSave4 = ((CButton*)pdlg->GetDlgItem(IDC_saveimage4))->GetCheck();
				if (bSave4)
				{
					pdlg->imageno4++;
					CStringA str;
					str.Format("D:/savedphoto4/GO_%d.tiff", pdlg->imageno3);
					WriteImage(hobjTmp, "tiff", 0, str.GetString());
				}
			}
			catch (HalconCpp::HException &HDevExpDefaultException)
			{
				TRACE("Camera4 Grab Failed!! \r\n");
				pdlg->m_exitThread[3] = 1;
				G::bRunning = 0;
				pdlg->MessageBox(L"相机4抓图出错,请重启软件!",L"警告",MB_ICONWARNING);
				/*HalconCpp::CloseFramegrabber(G::hv_AcqHandle3);
				Sleep(100);
				OpenFramegrabber("GigEVision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
				-1, "false", "default", "UpCam", 0, -1, &G::hv_AcqHandle3);
				SetFramegrabberParam(G::hv_AcqHandle3, "grab_timeout", -1);
				SetFramegrabberParam(G::hv_AcqHandle3, "TriggerMode", "Off");*/
				continue;
			}
			//Sleep(1);
		}                  ////////if(cangrab)
		Sleep(1);
	}
	//HalconCpp::CloseFramegrabber(G::hv_AcqHandle3);
	pdlg->m_exitThread[3] = 1;
	TRACE("Camera4Grab Thread Exiting \r\n");
	return TRUE;
}

//相机5抓图线程（右产品-左端外径外观）
DWORD     CameraGrab5(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[17] = 0;
	HObject hobjTmp;
	//HTuple  w, h;
	//定义一个新的列表元素
	ImageLinked  p5;
	while ((0 == pdlg->m_exitThread[17]))
	{
		if (1/*G::bCanGrab*//* || FALSE==G::bRunning*/)
		{
			try
			{	
				//HalconCpp::GenEmptyObj(&hobjTmp);这个函数太慢，要16ms
				//开始抓图
				//--------------------------------------------------------------------------------------------
				//同步采集                  异步采集为grabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle5);//阻塞函数 。。。开始。。。。。。。。。。。。。。。
				HalconCpp::DispImage(hobjTmp, G::HW5);

				/////////////////////////////////////运行过程中才把图像插入链表//////////////////////
				if (1 == G::bCanGrab)
				{
					//将新元素插入列表
					//将抓取到的图像存入列表尾
					p5.image = hobjTmp;
					G::m_criticalsection_imagelist5.Lock();
					G::Imagelist5.AddTail(p5);
					G::m_criticalsection_imagelist5.Unlock();
					G::grab5counts++;
				}
				//--------------------------------------------------------------------------------------------
				int bSave5 = ((CButton*)pdlg->GetDlgItem(IDC_saveimage5))->GetCheck();
				if (bSave5)
				{
					pdlg->imageno5++;
					CStringA str;
					str.Format("D:/savedphoto5/GO_%d.tiff", pdlg->imageno5);
					WriteImage(hobjTmp, "tiff", 0, str.GetString());
				}
			}
			catch (HalconCpp::HException &HDevExpDefaultException)
			{
				TRACE("Grab5 Failed!! \r\n");
				G::bRunning = 0;
				pdlg->MessageBox(L"相机5抓图出错,请重启软件!",L"警告",MB_ICONWARNING);
				pdlg->m_exitThread[17] = 1;
			}
			Sleep(1);
			//if (pdlg->m_exitThread[0] > 0)
			//{//退出线程
			//	TRACE("Camera1Grab Thread Exiting \r\n");
			//	break;
			//}
		}                //if(cangrab)
		Sleep(1);
		//////////////////////////////////////////old 抓图方式   ///////////////////////////////////

		//if (TRUE == G::bCanGrab)
		//{
		//	try
		//	{	//生成 密码令牌 ，持有此令牌的才有机会获得本次抓图
		//		LONGLONG llToken = ::GetTickCount();
		//		//HalconCpp::GenEmptyObj(&hobjTmp);这个函数太慢，要16ms
		//		//开始抓图
		//		//--------------------------------------------------------------------------------------------
		//		//同步采集                  异步采集为grabimagestart
		//		HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle5);//阻塞函数 。。。开始。。。。。。。。。。。。。。。
		//		//--------------------------------------------------------------------------------------------
		//		int bSave5 = ((CButton*)pdlg->GetDlgItem(IDC_saveimage4))->GetCheck();
		//		pdlg->imageno5++;
		//		if (bSave5)
		//		{
		//			CStringA str;
		//			str.Format("D:/savedphoto5/GO_%d.tiff", pdlg->imageno5);
		//			WriteImage(hobjTmp, "tiff", 0, str.GetString());
		//		}
		//		//G::GTCard.SetDoBitReverse(OP_Buzzer,1,50);	//触发脉冲
		//		/*iCountTest++;
		//		pDlg->SetDlgItemInt(IDC_COUNTTEST,iCountTest);*/
		//		//GetImageSize(hobjTmp, &w, &h);
		//		//SetPart(G::HW5, 0, 0, h, w);
		//		HalconCpp::DispImage(hobjTmp, G::HW5);
		//	}
		//	catch (HalconCpp::HException &HDevExpDefaultException)
		//	{
		//		TRACE("Camera5 Grab Failed!! \r\n");
		//		pdlg->m_exitThread[17] = 0;
		//		G::bRunning = 0;
		//		AfxMessageBox(L"相机5抓图出错,请重启软件!");
		//		/*HalconCpp::CloseFramegrabber(G::hv_AcqHandle5);
		//		Sleep(100);
		//		OpenFramegrabber("GigEVision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
		//			-1, "false", "default", "UpCam", 0, -1, &G::hv_AcqHandle5);
		//		SetFramegrabberParam(G::hv_AcqHandle5, "grab_timeout", -1);
		//		SetFramegrabberParam(G::hv_AcqHandle5, "TriggerMode", "Off");*/
		//		continue;
		//	}
		//	if (G::bRunning)
		//	{
		//		//将新元素插入列表
		//		//定义一个新的列表元素
		//		ImageLinked  p5;
		//		//将抓取到的图像存入列表尾
		//		p5.image = hobjTmp;
		//		G::Imagelist5.AddTail(p5);
		//	}
		//	Sleep(5);
		//	//Sleep(100);
		//	if (pdlg->m_exitThread[17] > 0)
		//	{//退出线程
		//		TRACE("Camera5Grab Thread Exiting \r\n");
		//		break;
		//	}
		//}

		///////////////////////////////////////////////////////////////////////////////////////////
	}
	pdlg->m_exitThread[17] = 1;
	TRACE(L"Camera5Grab Thread Exiting \r\n");
	//HalconCpp::CloseFramegrabber(G::hv_AcqHandle5);
	return TRUE;
}

//相机6抓图线程（右产品右端外径外观）
DWORD     CameraGrab6(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[18] = 0;
	HObject hobjTmp;
	//定义一个新的列表元素
	ImageLinked  p6;
	//HTuple  w, h;
	while ((0 == pdlg->m_exitThread[18]))
	{
		if (1/*G::bCanGrab*//* || FALSE==G::bRunning*/)
		{
			try
			{	//生成 密码令牌 ，持有此令牌的才有机会获得本次抓图
				//LONGLONG llToken = ::GetTickCount();
				//HalconCpp::GenEmptyObj(&hobjTmp);这个函数太慢，要16ms
				//开始抓图
				//--------------------------------------------------------------------------------------------
				//同步采集                  异步采集为grabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle6);//阻塞函数 。。。开始。。。。。。。。。。。。。。。
				HalconCpp::DispImage(hobjTmp, G::HW6);

				/////////////////////////////////////运行过程中才把图像插入链表//////////////////////
				if (1 == G::bCanGrab)
				{
					//将新元素插入列表
					//将抓取到的图像存入列表尾
					p6.image = hobjTmp;
					G::m_criticalsection_imagelist6.Lock();
					G::Imagelist6.AddTail(p6);
					G::m_criticalsection_imagelist6.Unlock();
					G::grab6counts++;
				}
				//--------------------------------------------------------------------------------------------
				int bSave6 = ((CButton*)pdlg->GetDlgItem(IDC_saveimage6))->GetCheck();
				if (bSave6)
				{
					pdlg->imageno6++;
					CStringA str;
					str.Format("D:/savedphoto6/GO_%d.tiff", pdlg->imageno6);
					WriteImage(hobjTmp, "tiff", 0, str.GetString());
				}
			}
			catch (HalconCpp::HException &HDevExpDefaultException)
			{
				G::bRunning = 0;
				pdlg->MessageBox(L"相机6抓图出错,请重启软件!",L"警告",MB_ICONWARNING);
				pdlg->m_exitThread[18] = 1;
			}
			Sleep(1);
			//if (pdlg->m_exitThread[0] > 0)
			//{//退出线程
			//	TRACE("Camera1Grab Thread Exiting \r\n");
			//	break;
			//}
		}                //if(cangrab)
		Sleep(1);
	}
	TRACE("Camera6Grab Thread Exiting \r\n");
	pdlg->m_exitThread[18] = 1;
	return TRUE;
}

//4复位线程
DWORD      Reset(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[4] = 0;
	int  Resetstep = 10;
	int  dwt=0;
	while (0 == pdlg->m_exitThread[4])
	{
		switch (Resetstep)
		{
		case 0:
			pdlg->m_exitThread[4] = 1;
			G::bResetOK = 1;
			pdlg->GetDlgItem(IDC_RST_BUTTON)->EnableWindow(TRUE);
			pdlg->GetDlgItem(IDC_START_BUTTON)->EnableWindow(TRUE);
			pdlg->GetDlgItem(IDC_REALTIME_BUTTON)->EnableWindow(TRUE);
			pdlg->MessageBox(L"复位完成！",L"提示",MB_OK);
			break;
		case 1:             //复位失败
			pdlg->m_exitThread[4] = 1;
			pdlg->GetDlgItem(IDC_START_BUTTON)->EnableWindow(TRUE);
			pdlg->GetDlgItem(IDC_RST_BUTTON)->EnableWindow(TRUE);
			pdlg->GetDlgItem(IDC_REALTIME_BUTTON)->EnableWindow(TRUE);
			pdlg->GetDlgItem(IDC_GRAB_BUTTON)->EnableWindow(TRUE);
			G::bResetOK = 0;
			G::bLeftRstOK = 0;
			G::bRightRstOK = 0;
			Sleep(2);
			break;
		case 10:
			if(0==dwt)
			{
				//开启吹气电磁阀
				G::MCard.SetDoBit(LEFTRADIOTUBENG, 1);
				G::MCard.SetDoBit(RIGHTRADIOTUBENG, 1);
				//复位两条轴
				if (MCARD_ERROR == G::MCard.HomeAxis2(AXIS_LEFT, 1, 1, 2))
				{
					pdlg->MessageBox(L"左端轴复位失败，请重新复位！", L"警告", MB_ICONERROR);
					dwt = 0;
					Resetstep = 1;                            //复位失败
					pdlg->m_exitThread[4] = 1;
					break;
				}
				if (MCARD_ERROR == G::MCard.HomeAxis2(AXIS_RIGHT, 1, 1, 2))
				{
					pdlg->MessageBox(L"右端轴复位失败，请重新复位！", L"警告", MB_ICONERROR);
					dwt = 0;
					Resetstep = 1;
					pdlg->m_exitThread[4] = 1;
					break;
				}
				dwt= ::GetTickCount()+G::PR[MP_RstTimeout].paravalue;
			}
			else if ((1 == G::MCard.A[AXIS_LEFT].AS.bORGSnr) && (1 == G::MCard.A[AXIS_LEFT].AS.bDone)&&(1==G::MCard.A[AXIS_RIGHT].AS.bORGSnr)&&(1==G::MCard.A[AXIS_RIGHT].AS.bDone))
			{
				dwt=0;
				Resetstep=20;
			}
			else if(dwt<::GetTickCount())
			{
				if (0 == G::MCard.A[AXIS_LEFT].AS.bORGSnr)
				{
					pdlg->MessageBox(L"左端轴复位超时！", L"警告", MB_ICONERROR);
					Resetstep = 1;
					break;
				}
				if (0 == G::MCard.A[AXIS_RIGHT].AS.bORGSnr)
				{
					pdlg->MessageBox(L"轴复位超时!", L"警告", MB_ICONERROR);
					Resetstep = 1;
					break;
				}
			}
			Sleep(2);
			break;
		case 20:
			G::MCard.SetDoBit(TRIGGER1, 0);
			G::MCard.SetDoBit(TRIGGER2, 0);

			G::MCard.SetAxisCurPos(AXIS_LEFT, 0);
			G::MCard.SetAxisEncPos(AXIS_LEFT, 0);
			G::MCard.A[AXIS_LEFT].AS.bHome = 1;
			G::bLeftRstOK = 1;

			G::MCard.SetAxisCurPos(AXIS_RIGHT, 0);
			G::MCard.SetAxisEncPos(AXIS_RIGHT, 0);
			G::MCard.A[AXIS_RIGHT].AS.bHome = 1;
			G::bRightRstOK = 1;

			Resetstep = 0;
			Sleep(10);
			break;

		}
		Sleep(10);
	}
	TRACE(L"Reset Thread Exit\n");
	pdlg->m_exitThread[4] = 1;
	return 0;
}

//5实时线程
DWORD      RealTimeGrab(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[5] = 0;
	int  dwt = 0;
	while (0 == pdlg->m_exitThread[5])
	{
		//G::CameraTrigger(1);
		//G::CameraTrigger(2);
		dmc_reverse_outbit(0,14,30);
		dmc_reverse_outbit(0,15,30);
		Sleep(10);
	}
	TRACE(L"RealTimeGrab Thread Exit\n");
	pdlg->m_exitThread[5] = 1;
	return 0;
}

//6轴运动线程
DWORD     AxisMove(LPVOID lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[6] = 0;
	int  step = 10;
	int  ntimes = 0;
	while (0 == pdlg->m_exitThread[6])
	{
		//运行中
		if (TRUE == G::bRunning)
		{
			//首次开始执行指令
			if (ntimes == 0)
			{
				//持续输出脉冲
				//0,负方向   1正方向
				if (G::bLeftNoShield == 1)
				{
					dmc_set_profile(0, AXIS_LEFT, 1000, G::PR[MP_LeftRunSpeed].paravalue, 0.02, 0.02, 1000);
					dmc_vmove(0, AXIS_LEFT, 1);
				}
					
				if (1 == G::bRightNoShield)
				{
					dmc_set_profile(0, AXIS_RIGHT, 1000, G::PR[MP_RightRunSpeed].paravalue, 0.02, 0.02, 1000);
					dmc_vmove(0, AXIS_RIGHT, 1);
				}

				ntimes = 1;
			}
			Sleep(2);
		}
		//停止轴运动
		else
		{
			if(1==ntimes)
			{
				//0减速停止  1紧急停止
			    G::MCard.StopAxis(AXIS_LEFT, 1);
				G::MCard.StopAxis(AXIS_RIGHT, 1);
			    ntimes = 0;
			}
		}
		Sleep(5);
		/*switch (step)
		{
		case 0:
			pdlg->GetDlgItem(IDC_RST_BUTTON)->EnableWindow(TRUE);
			pdlg->GetDlgItem(IDC_START_BUTTON)->EnableWindow(TRUE);
			pdlg->GetDlgItem(IDC_REALTIME_BUTTON)->EnableWindow(TRUE);
			pdlg->GetDlgItem(IDC_GRAB_BUTTON)->EnableWindow(TRUE);
			break;
		}*/
	}
	TRACE(L"AxisMove Thread Exit\n");
	pdlg->m_exitThread[6] = 1;
	return 0;
}

//7获取位置触发相机拍照线程
DWORD    CameraTrigger(LPVOID lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[7] = 0;
	long  pos, pos1;
	while (0 == pdlg->m_exitThread[7])
	{
		if (1 == G::bRunning)
		{//20ms延迟进入下一次              根据速度算拍照间隔距离区间
			//左相机拍照位
			if (1 == G::bLeftNoShield)
			{
				pos = dmc_get_position(0, 0);                   //当前位置
				if (((pos - G::PR[MP_LeftCamFirstPos].paravalue) >= G::PR[MP_LeftCamIntervalDis].paravalue*G::GridCounts1 - 300) && ((pos - G::PR[MP_LeftCamFirstPos].paravalue) <= G::PR[MP_LeftCamIntervalDis].paravalue*G::GridCounts1 + 300))
				{
					G::CameraTrigger(1);
					G::triggercounts1++;
					G::GridCounts1++;

					//pdlg->m_message_listbox.AddString(L"trigger1");
				}
			}
			
			//右相机拍照位
			//if (((long)(G::MCard.GetAxisCurPos(0) - G::PR[4].paravalue) % (long)G::PR[5].paravalue) == 0)
			if (1 == G::bRightNoShield)
			{
				pos1 = dmc_get_position(0, 0);
				if (((pos1 - G::PR[MP_RightCamFirstPos].paravalue) >= G::PR[MP_LeftCamIntervalDis].paravalue*G::GridCounts2 - 300) && ((pos1 - G::PR[MP_RightCamFirstPos].paravalue) <= G::PR[MP_LeftCamIntervalDis].paravalue*G::GridCounts2 + 300))
				{
					G::CameraTrigger(2);
					G::triggercounts2++;
					G::GridCounts2++;

					//pdlg->m_message_listbox.AddString(L"trigger2");
				}
			}
			
			Sleep(2);
		}///////////ifbrunning
		Sleep(10);
	}
	TRACE(L"CameraTrigger Thread Exit");
	pdlg->m_exitThread[7] = 1;
	return 0;
}

//8图像处理线程1(左产品总长、毛刺)
//选中边缘，边缘两边检测是否存在毛刺
DWORD     ImageDispose1(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[8] = 0;
	ProductPositive   positivelink1;
	ImageLinked   imagelink1;
	CString         str;
	int  bResultSave1;
	//int  n;                //记录余数
	while (0==pdlg->m_exitThread[8])
	{
		if (G::Imagelist1.GetCount() > 0)
		{
			G::m_criticalsection_imagelist1.Lock();
			imagelink1 = G::Imagelist1.GetHead();              //不为空，取表头元素
			//删除图像
			G::Imagelist1.RemoveHead();
			G::m_criticalsection_imagelist1.Unlock();
			
			//进行图像处理

			G::ImageDispose1(imagelink1.image, G::modle1);
			

			//G::m_criticalsection_imagelist1.Lock();
			//imagelink1 = G::Imagelist1.GetHead();              //不为空，取表头元素
			////进行图像处理
			//G::ImageDispose1(imagelink1.image, G::modle1);
			////删除图像
			//G::Imagelist1.RemoveHead();
			//G::m_criticalsection_imagelist1.Unlock();
			//G::grab1delete++;                      //图像1删除量，代表图像1处理数量
			////list控件插入数据
			//str.Format(L"%d", G::grab1counts - 2);
			//pdlg->m_data_list.InsertItem(G::grab1counts - 2, str);
			//str.Format(L"%.3lf mm", G::LeftLength);
			//pdlg->m_data_list.SetItemText(G::grab1counts - 2, 1, str);
			//str.Format(L"%.3lf mm", G::LeftDiameter);
			//pdlg->m_data_list.SetItemText(G::grab1counts - 2, 2, str);
			////如果要将数据写到文件
			//bResultSave1 = ((CButton*)pdlg->GetDlgItem(IDC_DATASAVE_CHECK))->GetCheck();
			//if (bResultSave1)
			//{
			//	sizelink1.Diameter = G::LeftDiameter;
			//	sizelink1.TotalLength = G::LeftLength;
			//	G::m_criticalsection_sizelist1tofile.Lock();
			//	G::SizelistToFile1.AddTail(sizelink1);
			//	G::m_criticalsection_sizelist1tofile.Unlock();
			//	/*G::ToFileCounts1++;
			//	if (G::ToFileCounts1 % 2 == 1)
			//	G::SizelistToFile1.AddTail(sizelink1);
			//	else
			//	G::SizelistToFile1backup.AddTail(sizelink1);*/
			//}


		}
		Sleep(5);
	}
	TRACE(L"ImageDispose1 Thread Exit");
	pdlg->m_exitThread[8] = 1;
	return 0;
}

//9图像处理线程2（左产品-左端外径，堵孔，孔不圆，毛刺）
DWORD     ImageDispose2(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[9] = 0;
	ImageLinked   imagelink2;
	CString       str;
	ProductLateral     laterallink;
	while (0 == pdlg->m_exitThread[9])
	{
		//两端外观的图像列表都不为空
		if ((G::Imagelist2.GetCount() > 0))
		{


			////先处理左端图像，如果为NG，结果为0，不需要比较右端图像，
			////注意，不比较右端图像时，需要记得将右端图像删除
			////开始处理左端图像
			//G::m_criticalsection_imagelist2.Lock();
			//imagelink2 = G::Imagelist2.GetHead();             //获取图像
			////进行图像处理
			////pdlg->AppearComputeLeft();                              //外观处理1-左端
			////删除图像
			//G::Imagelist2.RemoveHead();
			//G::m_criticalsection_imagelist2.Unlock();
			////验证结果，如果左端OK，则开始检测右端，否则直接将结果判负，并且将右端图像删除
			//if (0 == pdlg->LeftOK1)                    //如果左端产品NG
			//{
			//	dirtylink.NoDirty = 0;            //直接判负
			//	//删除右端节点
			//	G::m_criticalsection_imagelist3.Lock();
			//	G::Imagelist3.RemoveHead();
			//	G::m_criticalsection_imagelist3.Unlock();
			//	//将结果放入结果列表
			//	G::m_criticalsection_dirtylist1.Lock();
			//	G::Dirtylist1.AddTail(dirtylink);
			//	G::m_criticalsection_dirtylist1.Unlock();
			//	break;                                  //
			//}
			//else if (1 == pdlg->LeftOK1)                   //左端外观位OK
			//{
			//	//开始判定右端外观
			//	G::m_criticalsection_imagelist3.Lock();
			//	imagelink2 = G::Imagelist3.GetHead();
			//	//
			//	//pdlg->AppearComputeRight();                         //处理右端外观
			//	//删除节点
			//	G::Imagelist3.RemoveHead();
			//	G::m_criticalsection_imagelist3.Unlock();

			//	//处理右端结果
			//	//右端为OK
			//	if (1 == pdlg->RightOK1)
			//		dirtylink.NoDirty = 1;
			//	else if(0==pdlg->RightOK1)
			//		dirtylink.NoDirty = 0;

			//	//将结果插入表
			//	G::m_criticalsection_dirtylist1.Lock();
			//	G::Dirtylist1.AddTail(dirtylink);
			//	G::m_criticalsection_dirtylist1.Unlock();
			//}      //else if 左端产品
			
		}////////////////////if 表不为空

		Sleep(5);
	}
	TRACE(L"ImageDispose2 Thread Exit");
	pdlg->m_exitThread[9] = 1;
	return 0;
}

//10图像处理线程3（左产品右端外径外观）     图像列表3
DWORD     ImageDispose3(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[10] = 0;
	ProductLateral  laterallink;
	ImageLinked   imagelink4;
	CString         str;
	int  bResultSave1;
	int    n;                                                   //记录余数
	while (0 == pdlg->m_exitThread[10])
	{
		if (G::Imagelist4.GetCount() > 0)
		{
			//G::m_criticalsection_imagelist4.Lock();
			//imagelink4 = G::Imagelist4.GetHead();
			////G::m_criticalsection_imagelist3.Unlock();
			////图像处理
			//G::ImageDispose2(imagelink4.image, G::modle1);
			////G::m_criticalsection_imagelist3.Lock();
			////删除图像
			//G::Imagelist4.RemoveHead();
			//G::m_criticalsection_imagelist4.Unlock();
			//G::grab2delete++;
			////list控件插入数据
			//str.Format(L"%d", G::grab2counts - 2);
			//pdlg->m_data_list.SetItemText(G::grab1counts - 2, 3, str);
			//str.Format(L"%.3lf mm", G::RightLength);
			//pdlg->m_data_list.SetItemText(G::grab1counts - 2, 4, str);
			//str.Format(L"%.3lf mm", G::RightDiameter);
			//pdlg->m_data_list.SetItemText(G::grab1counts - 2, 5, str);

			//bResultSave1 = ((CButton*)pdlg->GetDlgItem(IDC_DATASAVE_CHECK))->GetCheck();
			//if (bResultSave1)
			//{
			//	sizelink4.Diameter = G::RightDiameter;
			//	sizelink4.TotalLength = G::RightLength;

			//	G::m_criticalsection_sizelist2tofile.Lock();
			//	G::SizelistToFile2.AddTail(sizelink4);
			//	G::m_criticalsection_sizelist2tofile.Unlock();
			//	//G::ToFileCounts2++;
			//	//if (G::ToFileCounts2 % 2 == 1)
			//	//	//将结果插入链表尾
			//	//	G::SizelistToFile2.AddTail(sizelink3);
			//	//else
			//	//	G::SizelistToFile2backup.AddTail(sizelink3);
			//}
		}
		//if (G::bRunning)
		//{
		//	
		//	//else if ((1 == n) && (G::Imagelist3backup.GetCount() > 0))
		//	//{
		//	//	imagelink3 = G::Imagelist3backup.GetHead();
		//	//	G::ImageDispose2(imagelink3.image, G::modle1);
		//	//	//删除图像
		//	//	G::Imagelist3backup.RemoveHead();
		//	//	G::grab2delete++;
		//	//	//list控件插入数据
		//	//	str.Format(L"%d", G::grab2counts - 2);
		//	//	pdlg->m_data_list.SetItemText(G::grab1counts - 2, 3, str);
		//	//	str.Format(L"%.3lf mm", G::RightLength);
		//	//	pdlg->m_data_list.SetItemText(G::grab1counts - 2, 4, str);
		//	//	str.Format(L"%.3lf mm", G::RightDiameter);
		//	//	pdlg->m_data_list.SetItemText(G::grab1counts - 2, 5, str);

		//	//	bResultSave1 = ((CButton*)pdlg->GetDlgItem(IDC_DATASAVE_CHECK))->GetCheck();
		//	//	if (bResultSave1)
		//	//	{
		//	//		sizelink3.Diameter = G::RightDiameter;
		//	//		sizelink3.TotalLength = G::RightLength;
		//	//		G::ToFileCounts2++;
		//	//		if (G::ToFileCounts2 % 2 == 1)
		//	//			//将结果插入链表尾
		//	//			G::SizelistToFile2.AddTail(sizelink3);
		//	//		else
		//	//			G::SizelistToFile2backup.AddTail(sizelink3);
		//	//	}
		//	//}

		//}             //if(brunning)
		Sleep(5);
	}
	TRACE(L"ImageDispose3 Thread Exit");
	pdlg->m_exitThread[10] = 1;
	return 0;
}

//11图像处理线程4（右产品总长）  
DWORD     ImageDispose4(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[11] = 0;
	ImageLinked   imagelink5;
	CString       str;
	ProductPositive     positivelink;
	while (0 == pdlg->m_exitThread[11])
	{
		//两端外观的图像列表都不为空
		if ((G::Imagelist5.GetCount() > 0))
		{
			////先处理左端图像，如果为NG，结果为0，不需要比较右端图像，
			////注意，不比较右端图像时，需要记得将右端图像删除
			////开始处理左端图像
			//G::m_criticalsection_imagelist5.Lock();
			//imagelink5 = G::Imagelist5.GetHead();             //获取图像
			////进行图像处理
			////pdlg->AppearComputeLeft();                              //外观处理1-左端
			////删除图像
			//G::Imagelist5.RemoveHead();
			//G::m_criticalsection_imagelist5.Unlock();
			////验证结果，如果左端OK，则开始检测右端，否则直接将结果判负，并且将右端图像删除
			//if (0 == pdlg->LeftOK2)                    //如果左端产品NG
			//{
			//	dirtylink.NoDirty = 0;            //直接判负
			//	//删除右端节点
			//	G::m_criticalsection_imagelist6.Lock();
			//	G::Imagelist6.RemoveHead();
			//	G::m_criticalsection_imagelist6.Unlock();
			//	//将结果放入结果列表
			//	G::m_criticalsection_dirtylist2.Lock();
			//	G::Dirtylist2.AddTail(dirtylink);
			//	G::m_criticalsection_dirtylist2.Unlock();

			//	break;                                  //
			//}
			//else if (1 == pdlg->LeftOK2)                   //左端外观位OK
			//{
			//	//开始判定右端外观
			//	G::m_criticalsection_imagelist6.Lock();
			//	imagelink5 = G::Imagelist6.GetHead();
			//	//
			//	//pdlg->AppearComputeRight();                         //处理右端外观
			//	//删除节点
			//	G::Imagelist6.RemoveHead();
			//	G::m_criticalsection_imagelist6.Unlock();

			//	//处理右端结果
			//	//右端为OK
			//	if (1 == pdlg->RightOK2)
			//		dirtylink.NoDirty = 1;
			//	else if (0 == pdlg->RightOK2)
			//		dirtylink.NoDirty = 0;

			//	//将结果插入表
			//	G::m_criticalsection_dirtylist2.Lock();
			//	G::Dirtylist2.AddTail(dirtylink);
			//	G::m_criticalsection_dirtylist2.Unlock();
			//}      //else if 左端产品

		}////////////////////if 表不为空
		Sleep(5);
	}
	TRACE(L"ImageDispose4 Thread Exit");
	pdlg->m_exitThread[11] = 1;
	return 0;
}

//20图像处理线程5(右产品左端外径外观)
DWORD   ImageDispose5(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[20] = 0;
	while (0 == pdlg->m_exitThread[20])
	{
		Sleep(5);
	}
	TRACE(L"ImageDispose5 Thread Exit");
	pdlg->m_exitThread[20] = 1;
	return 0;
}

//21图像处理线程6（右产品右端外径外观）
DWORD   ImageDispose6(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[21] = 0;
	while (0 == pdlg->m_exitThread[21])
	{
		Sleep(5);
	}
	TRACE(L"ImageDispose6 Thread Exit");
	pdlg->m_exitThread[21] = 1;
	return 0;
}

//12结果分析线程1（左产品）
DWORD     Analysis1(LPVOID lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[12] = 0;
	ProductPositive            positivelink;
	ProductLateral             lateralleftlink,lateralrightlink;
	DisposeResult              resultlink;
	//CString                   str;
	//int                           n;                              //余数，用于选择结果列表              
	while (0 == pdlg->m_exitThread[12])
	{
		//要统计尺寸信息，所以要几个表全部有内容才能开始进行分析
		if ((G::LeftPositiveList.GetCount() > 0)&&(G::LeftLateralList1.GetCount()>0)&&(G::LeftLateralList2.GetCount()>0))
		{
			//逐个比较，减少比较次数
			G::m_criticalsection_LeftPositive.Lock();
			positivelink = G::LeftPositiveList.GetHead();
			G::LeftPositiveList.RemoveHead();
			G::m_criticalsection_LeftPositive.Unlock();
			//从第一个表开始，每次取一个比较，如果NG，直接删掉后面的表的表头

			//先比较总长表
			//总长端OK
			if ((G::PR[VP_LeftOKLengthLow].paravalue <= positivelink.TotalLength) && (positivelink.TotalLength <= G::PR[VP_LeftOKLengthHigh].paravalue) && (positivelink.ApperanceOK == AppearanceOK))
			{
				//比较两端结果
				//取两表表头
				G::m_criticalsection_LeftLateral1.Lock();
				lateralleftlink = G::LeftLateralList1.GetHead();
				G::LeftLateralList1.RemoveHead();
				G::m_criticalsection_LeftLateral1.Unlock();
				//右端表表头
				G::m_criticalsection_LeftLateral2.Lock();
				lateralrightlink = G::LeftLateralList2.GetHead();
				G::LeftLateralList2.RemoveHead();
				G::m_criticalsection_LeftLateral2.Unlock();
				//开始比较两端结果
				//结果为OK
				if ((lateralleftlink.AppearanceOK == AppearanceOK) && (G::PR[VP_LeftOKDiameterLow].paravalue <= lateralleftlink.Diameter) && (lateralleftlink.Diameter <= G::PR[VP_LeftOKDiameterHigh].paravalue) && (AppearanceOK == lateralrightlink.AppearanceOK) && (G::PR[VP_LeftOKDiameterLow].paravalue <= lateralrightlink.Diameter) && (lateralrightlink.Diameter <= G::PR[VP_LeftOKDiameterHigh].paravalue))
				{
					resultlink.OK = 1;
					G::disp_message(G::HW1, "OK", "image", 180, 1600, "green", "false");
				}
				else         //结果为ng
				{
					resultlink.OK = 0;
					G::disp_message(G::HW1, "NG", "image", 180, 1600, "red", "false");
					G::PR[PI_NGCounts].paravalue++;
				}
				//将结果插入表尾
				G::m_criticalsection_LeftResultlist.Lock();
				G::LeftResultlist.AddTail(resultlink);
				G::m_criticalsection_LeftResultlist.Unlock();
			}
			//总长端NG，直接删除另外两个表头
			else
			{
				G::disp_message(G::HW1, "NG", "image", 180, 1600, "red", "false");
				resultlink.OK = 0;               //结果为NG
				//将结果插入表尾
				G::m_criticalsection_LeftResultlist.Lock();
				G::LeftResultlist.AddTail(resultlink);
				G::m_criticalsection_LeftResultlist.Unlock();
				//删除左端表
				G::m_criticalsection_LeftLateral1.Lock();
				G::LeftLateralList1.RemoveHead();
				G::m_criticalsection_LeftLateral1.Unlock();
				//删除右端表
				G::m_criticalsection_LeftLateral2.Lock();
				G::LeftLateralList2.RemoveHead();
				G::m_criticalsection_LeftLateral2.Unlock();
			}
			//产量加一
			G::PR[PI_TotalCounts].paravalue++;

		}     /////////////////////////if 表不为空
		
		Sleep(5);
	}          //while
	TRACE(L"Analysis1 Thread Exit");
	pdlg->m_exitThread[12] = 1;
	return 0;
}

//13结果分析线程2
DWORD     Analysis2(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[13] = 0;
	ProductPositive            positivelink;
	ProductLateral             lateralleftlink, lateralrightlink;
	DisposeResult         resultlink;                       //结果列表节点，用于插入结果列表
	//int                         n;                                   //余数，用于选择结果列表
	while (0 == pdlg->m_exitThread[13])
	{
		//要统计尺寸信息，所以要几个表全部有内容才能开始进行分析
		if ((G::RightPositiveList.GetCount() > 0) && (G::RightLateralList1.GetCount()>0) && (G::RightLateralList2.GetCount()>0))
		{
			//逐个比较，减少比较次数
			G::m_criticalsection_RightPositive.Lock();
			positivelink = G::RightPositiveList.GetHead();
			G::RightPositiveList.RemoveHead();
			G::m_criticalsection_RightPositive.Unlock();
			//从第一个表开始，每次取一个比较，如果NG，直接删掉后面的表的表头

			//先比较总长表
			//总长端OK
			if ((G::PR[VP_RightOKLengthLow].paravalue <= positivelink.TotalLength) && (positivelink.TotalLength <= G::PR[VP_RightOKLengthHigh].paravalue) && (positivelink.ApperanceOK == AppearanceOK))
			{
				//比较两端结果
				//取两表表头
				G::m_criticalsection_RightLateral1.Lock();
				lateralleftlink = G::RightLateralList1.GetHead();
				G::RightLateralList1.RemoveHead();
				G::m_criticalsection_RightLateral1.Unlock();
				//右端表表头
				G::m_criticalsection_RightLateral2.Lock();
				lateralrightlink = G::RightLateralList2.GetHead();
				G::RightLateralList2.RemoveHead();
				G::m_criticalsection_RightLateral2.Unlock();
				//开始比较两端结果
				//结果为OK
				if ((lateralleftlink.AppearanceOK == AppearanceOK) && (G::PR[VP_RightOKDiameterLow].paravalue <= lateralleftlink.Diameter) && (lateralleftlink.Diameter <= G::PR[VP_RightOKDiameterHigh].paravalue) && (AppearanceOK == lateralrightlink.AppearanceOK) && (G::PR[VP_RightOKDiameterLow].paravalue <= lateralrightlink.Diameter) && (lateralrightlink.Diameter <= G::PR[VP_RightOKDiameterHigh].paravalue))
				{
					resultlink.OK = 1;
					G::disp_message(G::HW4, "OK", "image", 180, 1600, "green", "false");
				}
				else         //结果为ng
				{
					resultlink.OK = 0;
					G::disp_message(G::HW4, "NG", "image", 180, 1600, "red", "false");
					G::PR[PI_NGCounts].paravalue++;
				}
				//将结果插入表尾
				G::m_criticalsection_RightResultlist.Lock();
				G::RightResultlist.AddTail(resultlink);
				G::m_criticalsection_RightResultlist.Unlock();
			}
			//总长端NG，直接删除另外两个表头
			else
			{
				G::disp_message(G::HW3, "NG", "image", 180, 1600, "red", "false");
				resultlink.OK = 0;               //结果为NG
				//将结果插入表尾
				G::m_criticalsection_RightResultlist.Lock();
				G::RightResultlist.AddTail(resultlink);
				G::m_criticalsection_RightResultlist.Unlock();
				//删除左端表
				G::m_criticalsection_RightLateral1.Lock();
				G::RightLateralList1.RemoveHead();
				G::m_criticalsection_RightLateral1.Unlock();
				//删除右端表
				G::m_criticalsection_RightLateral2.Lock();
				G::RightLateralList2.RemoveHead();
				G::m_criticalsection_RightLateral2.Unlock();
			}
			//产量加一
			G::PR[PI_TotalCounts].paravalue++;

		}     /////////////////////////if 表不为空

		//if ((G::Sizelist2.GetCount() > 0)&&(G::Dirtylist2.GetCount()>0))
		//{
		//	G::m_criticalsection_sizelist2.Lock();
		//	sizelink = G::Sizelist2.GetHead();
		//	G::Sizelist2.RemoveHead();
		//	G::m_criticalsection_sizelist2.Unlock();
		//	///取外观结果
		//	G::m_criticalsection_dirtylist2.Lock();
		//	dirtylink = G::Dirtylist2.GetHead();
		//	G::Dirtylist2.RemoveHead();
		//	G::m_criticalsection_dirtylist2.Unlock();
		//	//显示数据
		//	G::disp_message(G::HW4, ("外径:" + (HTuple)G::RightDiameter) + " mm", "image", 300, 100, "green", "false");
		//	G::disp_message(G::HW4, ("总长:" + (HTuple)G::RightLength) + " mm", "image", 450, 100, "green", "false");
		//	//分析数据
		//	if ((sizelink.Diameter >= G::PR[124].paravalue) && (sizelink.Diameter <= G::PR[125].paravalue) && (sizelink.TotalLength >= G::PR[122].paravalue) && (sizelink.TotalLength <= G::PR[123].paravalue))
		//	{
		//		resultlink.OK = 1;
		//		G::disp_message(G::HW4, "NG", "image", 200, 1600, "green", "false");
		//	}
		//	else
		//	{
		//		resultlink.OK = 0;
		//		G::disp_message(G::HW4, "NG", "image", 200, 1600, "red", "false");
		//		G::PR[PI_NGCounts].paravalue++;                                          //ng数加一
		//	}
		//	//产量加一
		//	G::PR[PI_TotalCounts].paravalue++;

		//	G::m_criticalsection_blow2.Lock();
		//	G::DisposeResultlist2.AddTail(resultlink);
		//	G::m_criticalsection_blow2.Unlock();
		//}

		Sleep(5);
	}
	TRACE(L"Analysis2 Thread Exit");
	pdlg->m_exitThread[13] = 1;
	return 0;
}

//14位置刷新吹气线程1
DWORD    Blow1(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[14] = 0;
	DisposeResult         resultlink1;
	CString   str;
	long ss;               //当前轴位置
	//int    n;               //余数，用于选择结果列表
	while (0==pdlg->m_exitThread[14])
	{
		if ((TRUE == G::bRunning) && (1==G::bLeftNoShield))
		{
			//n = G::LeftResultCounts % 2;
			//比较当前位置
			ss = dmc_get_position(0, AXIS_LEFT);
			if (((ss - G::PR[MP_LeftNGBlowPos].paravalue) >= (G::PR[MP_LeftCamIntervalDis].paravalue*G::blow1 - 0.03*G::PR[MP_LeftRunSpeed].paravalue)) && ((ss - G::PR[MP_LeftNGBlowPos].paravalue) <= (G::PR[MP_LeftCamIntervalDis].paravalue*G::blow1 + 0.03*G::PR[MP_LeftRunSpeed].paravalue)))
			{
				//次数加一
				G::blow1++;
				//取结果并比较是否吹气
				if (G::LeftResultlist.GetCount() > 0)
				{
					G::m_criticalsection_LeftResultlist.Lock();
					resultlink1 = G::LeftResultlist.GetHead();
					//删除头节点
					G::LeftResultlist.RemoveHead();
					G::m_criticalsection_LeftResultlist.Unlock();
					//pdlg->m_criticalsection_blow1.Unlock();
					if (1 != resultlink1.OK)
					{
						//NG电磁阀吹气0.1S
						G::MCard.SetDoBitReverse(LEFTRADIOTUBENG, 1, 10);
						//G::MCard.SetDoBit(LEFTRADIOTUBENG, 1);
						//Sleep(5);
						//G::MCard.SetDoBit(LEFTRADIOTUBENG, 0);
					}

				}

				//if ((0 == n) && (G::DisposeResultlist1.GetCount() > 0))
				//{
				//	resultlink1 = G::DisposeResultlist1.GetHead();
				//	if (0 == resultlink1.OK)
				//	{
				//		//NG电磁阀吹气0.3S
				//		G::MCard.SetDoBit(LEFTRADIOTUBENG, 1);
				//		Sleep(5);
				//		G::MCard.SetDoBit(LEFTRADIOTUBENG, 0);
				//	}
				//	//删除头节点
				//	G::DisposeResultlist1.RemoveHead();
				//}
				//else if ((1 == n) && (G::DisposeResult1backup.GetCount() > 0))
				//{
				//	resultlink1 = G::DisposeResult1backup.GetHead();
				//	if (0 == resultlink1.OK)
				//	{
				//		//NG电磁阀吹气0.3S
				//		G::MCard.SetDoBit(LEFTRADIOTUBENG, 1);
				//		Sleep(5);
				//		G::MCard.SetDoBit(LEFTRADIOTUBENG, 0);
				//	}
				//	G::DisposeResult1backup.RemoveHead();
				//}
			}		
			
		}////////////IF brunning
		Sleep(2);
	}
	AfxMessageBox(L"Blow1 Thread Exit");
	pdlg->m_exitThread[14] = 1;
	return 0;
}

//15位置刷新吹气线程2
DWORD   Blow2(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[15] = 0;
	DisposeResult         resultlink2;
	long        pos;
	//int           n;
	while (0==pdlg->m_exitThread[15])	
	{
		if ((TRUE == G::bRunning) && (1==G::bRightNoShield) )
		{
			//n = G::RightResultCounts % 2;
			//获取当前轴位置
			pos = dmc_get_position(0, AXIS_RIGHT);
			//接近NG位置时开始记录
			if (((pos - G::PR[9].paravalue) >= (G::PR[3].paravalue*G::blow2 - 0.03*G::PR[1].paravalue)) && ((pos - G::PR[9].paravalue) <= (G::PR[3].paravalue*G::blow2 + 0.03*G::PR[1].paravalue)))
			{
				G::blow2++;
				//取表一节点
				if (G::RightResultlist.GetCount() > 0)
				{
					G::m_criticalsection_RightResultlist.Lock();
					resultlink2 = G::RightResultlist.GetHead();
					//删除节点
					G::RightResultlist.RemoveHead();
					G::m_criticalsection_RightResultlist.Unlock();
					//如果NG
					if (1 != resultlink2.OK)
					{
						//NG电磁阀吹气0.1S
						G::MCard.SetDoBitReverse(RIGHTRADIOTUBENG, 1, 10);
						//G::MCard.SetDoBit(RIGHTRADIOTUBENG, 1);
						//Sleep(5);
						//G::MCard.SetDoBit(RIGHTRADIOTUBENG, 0);
					}
				}

				//if ((0 == n) && (G::DisposeResultlist2.GetCount() > 0))
				//{
				//	resultlink2 = G::DisposeResultlist2.GetHead();
				//	//如果NG
				//	if (0 == resultlink2.OK)
				//	{
				//		//NG电磁阀吹气0.08S
				//		G::MCard.SetDoBit(RIGHTRADIOTUBENG, 1);
				//		Sleep(5);
				//		G::MCard.SetDoBit(RIGHTRADIOTUBENG, 0);
				//	}
				//	//删除节点
				//	G::DisposeResultlist2.RemoveHead();
				//}
				////取表二节点
				//if ((1 == n) && (G::DisposeResult2backup.GetCount() > 0))
				//{
				//	resultlink2 = G::DisposeResult2backup.GetHead();
				//	//如果NG
				//	if (0 == resultlink2.OK)
				//	{
				//		//NG电磁阀吹气0.08S
				//		G::MCard.SetDoBit(RIGHTRADIOTUBENG, 1);
				//		Sleep(5);
				//		G::MCard.SetDoBit(RIGHTRADIOTUBENG, 0);
				//	}
				//	G::DisposeResult2backup.RemoveHead();
				//}
			}

			///////////////////////////V1.5  2019.1.2
			//if (n == 0)
			//{
			//	if (G::DisposeResultlist2.GetCount() == 0)
			//	{
			//		//Sleep(1);
			//		continue;
			//	}
			//	resultlink2 = G::DisposeResultlist2.GetHead();
			//}
			//else
			//{
			//	if (G::DisposeResult2backup.GetCount() == 0)
			//	{
			//		//Sleep(1);
			//		continue;
			//	}
			//	resultlink2 = G::DisposeResult2backup.GetHead();
			//}
			//pdlg->m_message_listbox.AddString(L"blow2get");

			////获取当前轴位置
			//pos = dmc_get_position(0, 0);
			////接近NG位置时开始记录
			//if (((pos - G::PR[9].paravalue) >= (G::PR[3].paravalue*G::blow2 - 0.05*G::PR[1].paravalue)) && ((pos - G::PR[9].paravalue) <= (G::PR[3].paravalue*G::blow2 + 0.05*G::PR[1].paravalue)))
			//{
			//	pdlg->m_message_listbox.AddString(L"blow2pos");

			//	//如果NG
			//	if (0 == resultlink2.OK)
			//	{
			//		//NG电磁阀吹气0.08S
			//		G::MCard.SetDoBit(RIGHTRADIOTUBENG, 1);
			//		Sleep(5);
			//		G::MCard.SetDoBit(RIGHTRADIOTUBENG, 0);
			//	}
			//	G::blow2++;
			//	//删除头节点
			//	if (n == 0)
			//		G::DisposeResultlist2.RemoveHead();
			//	else
			//		G::DisposeResult2backup.RemoveHead();
			//}			
		}    /////////IF brunning
		Sleep(2);
	}
	TRACE(L"Blow2 Thread Exit");
	pdlg->m_exitThread[15] = 1;
	return 0;
}

//16刷新板卡状态
DWORD   RefreshCardState(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[16] = 0;
	int    dwt = 0;
	int    nTimes=0;
	while (0==pdlg->m_exitThread[16])
	{
		///////////////////////////////////////////输入监控////////////////////////////////////////////////////////////
		G::MCard.GetAllDi();
		G::MCard.GetAllDo();
		//////////////////////////////////////////急停监视////////////////////////////////////////////////////////////
		if (G::MCard.I[SCRAM_BUTTON].sValue == 0)
		{
			if (nTimes == 0)
			{
				pdlg->MessageBox(L"急停按下...",L"警告",MB_ICONWARNING);
				nTimes++;
			}
		}
		else
			nTimes = 0;
		//Sleep(5);

		/////////////////////////////////////////刷新板卡及其他状态
		try
		{
			G::MCard.GetAxisStatus(AXIS_LEFT);
			G::MCard.GetAxisStatus(AXIS_RIGHT);
			pdlg->RefreshStatus();
		}
		catch(...)
		{
			Sleep(1);
		}
		////////////////////////////////////将运动相关参数从PR传到MCard
		try
		{
			G::MCard.A[AXIS_LEFT].AP.fHomeVel=G::PR[MP_LeftRstSpeed].paravalue;     //复位速度
			//自动运行速度
			G::MCard.A[AXIS_LEFT].AP.fVel=G::PR[MP_LeftRunSpeed].paravalue;    
			//右轴复位速度
			G::MCard.A[AXIS_RIGHT].AP.fHomeVel = G::PR[MP_RightRstSpeed].paravalue;
			//右轴自动速度
			G::MCard.A[AXIS_RIGHT].AP.fVel = G::PR[MP_RightRunSpeed].paravalue;
		}
		catch(...)
		{
			Sleep(1);
		}
		
		Sleep(1);
	}
	TRACE(L"CardState Thread Exit");
	pdlg->m_exitThread[16] = 1;
	return 0;
}

////19尺寸数据写入数据库线程
//DWORD   ProductionToDB(LPVOID  lpparameter)
//{
//	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
//	pdlg->m_exitThread[19] = 0;
//	CheckSize    sizelink;
//	CheckSize   sizelink1;
//	CheckSize   sizelink2;
//	CTime  time;
//	CString strRptTime;
//	CString  strtime;
//	CString    syear,smonth,sday,shour,sminute,ssecond;
//	int        n1, n2;
//	while (0 == pdlg->m_exitThread[19])
//	{
//		//表1
//		if (G::SizelistToFile1.GetCount() == 0)
//		{
//			Sleep(1);
//			continue;
//		}
//		G::m_criticalsection_sizelist1tofile.Lock();
//		sizelink1 = G::SizelistToFile1.GetHead();
//		G::SizelistToFile1.RemoveHead();
//		G::m_criticalsection_sizelist1tofile.Unlock();
//
//		if (G::SizelistToFile2.GetCount() == 0)
//		{
//			Sleep(1);
//			continue;
//		}
//		G::m_criticalsection_sizelist2tofile.Lock();
//		sizelink2 = G::SizelistToFile2.GetHead();
//		G::SizelistToFile2.RemoveHead();
//		G::m_criticalsection_sizelist2tofile.Unlock();
//
//		time = CTime::GetCurrentTime();
//		syear = time.Format("%Y");
//		smonth = time.Format("%m");
//		sday = time.Format("%d");
//		strRptTime = syear + L"/" + smonth + L"/" + sday;
//		shour = time.Format("%H");
//		sminute = time.Format("%M");
//		ssecond = time.Format("%S");
//		strtime = shour + L":" + sminute + L":" + ssecond;
//
//		try
//		{
//			/*G::db.m_pRecordset->Open("SELECT  *  FROM " + (_bstr_t)ProductionTableName, G::db.m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockPessimistic, adCmdText);*/
//			//表末尾增加新纪录
//			G::db.m_pRecordset->AddNew();
//			//
//			G::db.m_pRecordset->PutCollect(L"日期", _variant_t(strRptTime));
//			G::db.m_pRecordset->PutCollect(L"时间", _variant_t(strtime));
//			G::db.m_pRecordset->PutCollect(L"左产品外径", _variant_t(G::double_string(sizelink1.Diameter)));
//			G::db.m_pRecordset->PutCollect(L"左产品总长", _variant_t(G::double_string(sizelink1.TotalLength)));
//			G::db.m_pRecordset->PutCollect(L"右产品外径", _variant_t(G::double_string(sizelink2.Diameter)));
//			G::db.m_pRecordset->PutCollect(L"右产品总长", _variant_t(G::double_string(sizelink2.TotalLength)));
//
//			G::db.m_pRecordset->Update();       //更新数据库，将更新结果存入数据库
//
//
//		}
//		catch (_com_error c)
//		{
//			CString  err;
//			err = c.ErrorMessage();
//			AfxMessageBox(err + L"失败");
//		}
//
//		////对应链表中有数据就写
//		//if (G::SizelistToFile1.GetCount() >= 1 && G::SizelistToFile2.GetCount() >= 1)
//		//{
//		//	time = CTime::GetCurrentTime();
//		//	syear=time.Format("%Y");
//		//	smonth=time.Format("%m");
//		//	sday=time.Format("%d");
//		//	strRptTime = syear+L"/"+smonth+L"/"+sday;
//		//	shour=time.Format("%H");
//		//	sminute=time.Format("%M");
//		//	ssecond=time.Format("%S");
//		//	strtime = shour+L":"+sminute+L":"+ssecond;
//
//		//	sizelink1 = G::SizelistToFile1.GetHead();
//		//	sizelink2 = G::SizelistToFile2.GetHead();
//
//		//	try
//		//	{
//		//		/*G::db.m_pRecordset->Open("SELECT  *  FROM " + (_bstr_t)ProductionTableName, G::db.m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockPessimistic, adCmdText);*/
//		//		//表末尾增加新纪录
//		//		G::db.m_pRecordset->AddNew();
//		//		//
//		//		G::db.m_pRecordset->PutCollect(L"日期", _variant_t(strRptTime));
//		//		G::db.m_pRecordset->PutCollect(L"时间", _variant_t(strtime));
//		//		G::db.m_pRecordset->PutCollect(L"左产品外径", _variant_t(G::double_string(sizelink1.Diameter)));
//		//		G::db.m_pRecordset->PutCollect(L"左产品总长", _variant_t(G::double_string(sizelink1.TotalLength)));
//		//		G::db.m_pRecordset->PutCollect(L"右产品外径", _variant_t(G::double_string(sizelink2.Diameter)));
//		//		G::db.m_pRecordset->PutCollect(L"右产品总长", _variant_t(G::double_string(sizelink2.TotalLength)));
//
//		//		G::db.m_pRecordset->Update();       //更新数据库，将更新结果存入数据库
//
//		//		//
//		//		G::SizelistToFile1.RemoveHead();
//		//		G::SizelistToFile2.RemoveHead();
//		//	}
//		//	catch (_com_error c)
//		//	{
//		//		CString  err;
//		//		err = c.ErrorMessage();
//		//		AfxMessageBox(err + L"失败");
//		//	}
//		//}	
//		Sleep(5);
//	}
//	TRACE(L"ProductionToDB Thread Exit");
//	pdlg->m_exitThread[19] = 1;
//	return 0;
//}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnMotionMenu();
//	virtual BOOL OnInitDialog();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_COMMAND(ID_MOTION_MENU, &CAboutDlg::OnMotionMenu)
END_MESSAGE_MAP()


// CMy1269AirColumnDlg 对话框



CMy1269AirColumnDlg::CMy1269AirColumnDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy1269AirColumnDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy1269AirColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_LISTBOX, m_message_listbox);
	DDX_Control(pDX, IDC_DATA_LIST, m_data_list);
	DDX_Control(pDX, IDC_DATA_LIST2, m_datalist2);
}

BEGIN_MESSAGE_MAP(CMy1269AirColumnDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMy1269AirColumnDlg::OnBnClickedCancel)
	ON_COMMAND(ID_EXIT_MENU, &CMy1269AirColumnDlg::OnExitMenu)
	ON_COMMAND(ID_PARASET_MENU, &CMy1269AirColumnDlg::OnParasetMenu)
	ON_COMMAND(ID_IOSET, &CMy1269AirColumnDlg::OnIoset)
	ON_COMMAND(ID_MOTION_MENU, &CMy1269AirColumnDlg::OnMotionMenu)
	ON_BN_CLICKED(IDC_RST_BUTTON, &CMy1269AirColumnDlg::OnBnClickedRstButton)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_REALTIME_BUTTON, &CMy1269AirColumnDlg::OnBnClickedRealtimeButton)
	ON_BN_CLICKED(IDC_GRAB_BUTTON, &CMy1269AirColumnDlg::OnBnClickedGrabButton)
	ON_BN_CLICKED(IDC_START_BUTTON, &CMy1269AirColumnDlg::OnBnClickedStartButton)
	ON_WM_TIMER()
	ON_COMMAND(ID_ClearProduct, &CMy1269AirColumnDlg::OnClearProduct)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CMy1269AirColumnDlg::OnBnClickedStopButton)
//	ON_BN_CLICKED(IDC_BUTTON1, &CMy1269AirColumnDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_SHIELDLEFT_BUTTON, &CMy1269AirColumnDlg::OnBnClickedShieldleftButton)
ON_BN_CLICKED(IDC_SHIELDRIGHT_BUTTON, &CMy1269AirColumnDlg::OnBnClickedShieldrightButton)
ON_BN_CLICKED(IDC_WRITEDATALEFT_BUTTON, &CMy1269AirColumnDlg::OnBnClickedWritedataleftButton)
ON_BN_CLICKED(IDC_WRITEDATARIGHT_BUTTON, &CMy1269AirColumnDlg::OnBnClickedWritedatarightButton)
ON_COMMAND(ID_VisonDebug, &CMy1269AirColumnDlg::OnVisondebug)
END_MESSAGE_MAP()


// CMy1269AirColumnDlg 消息处理程序

BOOL CMy1269AirColumnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	/*CMenu menu;
	menu.LoadMenuW(IDR_MENU1);
	SetMenu(&menu);*/

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	SkinH_Attach();
	AfxOleInit();              //初始化com组件
	//test=false;


	G::bOpenExcelOk = 0;

	G::CreateDatabase(L"AirColumn DB");

	G::CreateTable(ParaTableName);

	if (MCARD_ERROR == G::MCard.OpenMotionCard((CWnd*)this))
	{
		MessageBox(L"运动控制卡无法打开，请检查运动控制卡PCI卡槽是否插好，以及是否正确安装驱动！", L"警告", MB_ICONWARNING);
	}
	if (MCARD_ERROR == G::MCard.SetAxisParam(AXIS_LEFT))
	{
		MessageBox(L"左边伺服，参数设置错误！", L"警告", MB_ICONERROR);
	}
	if (MCARD_ERROR == G::MCard.SetAxisParam(AXIS_RIGHT))
	{
		MessageBox(L"右边伺服，参数设置错误！", L"警告", MB_ICONERROR);
	}
	G::MCard.SetDoBit(TRIGGER1,0);
	G::MCard.SetDoBit(TRIGGER2,0);
	G::MCard.SetAxsSafeFilter(myAxisSafeFunc);
	G::MCard.SetDoSafeFilter(myDoSafeFunc);
	/*G::MCard.SetAxisCurPos(0,0);
	G::MCard.SetAxisEncPos(0,0);*/
	ioset=NULL;

	G::bResetOK = 0;
	G::bRunning = 0;
	G::bCanGrab = 0;
	G::bRunflag = 0;
	G::bLeftNoShield = 1;
	G::bRightNoShield = 1;

	//获取模板
	try
	{
		ReadShapeModel("./空气柱检测1.shm", &G::modle1);
	}
	catch (...)
	{
		AfxMessageBox(L"打开模板失败");
		return -1;
	}

	//初始化参数
	G::InitParam();
	//初始化链表
	G::ClearLinked();
	//读取参数
	ReadPara();
	ReadIO();
	//打开相机
	G::OpenCamera();

	m_crWhite = RGB(255, 255, 255);
	m_hrWhite = CreateSolidBrush(m_crWhite);
	font1.CreatePointFont(360, L"华文行楷");
	this->GetDlgItem(IDC_TITLE_STATIC)->SetFont(&font1);

	//存图编号清零
	imageno1 = 0;
	imageno2 = 0;
	imageno3 = 0;
	imageno4 = 0;
	imageno5 = 0; 
	imageno6 = 0;

	//相机触发计数初始化
	G::GridCounts1=0;
	G::GridCounts2=0;

	G::grab1counts=0;                          //图像1抓图数量
	G::grab2counts=0;                          //图像2抓图数量
	G::grab1delete=0;                           //图像1删除数量
	G::grab2delete=0;                           //图像2删除数量
	G::triggercounts1=0;                       
	G::triggercounts2=0;
	G::blow1 = 0;                                  
	G::blow2 = 0;
	//G::LeftResultCounts = 0;                    //左产品结果计数
	//G::RightResultCounts = 0;                  //右产品结果计数
	//G::LeftDisposecounts1=0;                   ///将结果插入处理结果列表用
	//G::RightDisposecounts1=0;                 //将结果插入处理结果链表用
	//G::ToFileCounts1 = 0;                        //结果写入数据库计数1
	//G::ToFileCounts2 = 0;                        //结果写入数据库计数2

	//两端外观检测结果初始化
	LeftOK1 = LeftOK2  = RightOK1 = RightOK2  = -1;

	//设置窗口
	HWND   HW1X = GetDlgItem(IDC_Picture1)->m_hWnd;
	GetDlgItem(IDC_Picture1)->GetClientRect(rect1);
	OpenWindow(rect1.top, rect1.left, rect1.Width(), rect1.Height(), (Hlong)HW1X, "visible", "", &G::HW1);

	HWND   HW2X = GetDlgItem(IDC_Picture2)->m_hWnd;
	GetDlgItem(IDC_Picture2)->GetClientRect(rect2);
	OpenWindow(rect2.top, rect2.left, rect2.Width(), rect2.Height(), (Hlong)HW2X, "visible", "", &G::HW2);

	HWND    HW3X = GetDlgItem(IDC_Picture3)->m_hWnd;
	GetDlgItem(IDC_Picture3)->GetClientRect(rect3);
	OpenWindow(rect3.top, rect3.left, rect3.Width(), rect3.Height(), (Hlong)HW3X, "visible", "", &G::HW3);

	HWND    HW4X = GetDlgItem(IDC_Picture4)->m_hWnd;
	GetDlgItem(IDC_Picture4)->GetClientRect(rect4);
	OpenWindow(rect4.top, rect4.left, rect4.Width(), rect4.Height(), (Hlong)HW4X, "visible", "", &G::HW4);

	HWND   HW5X = GetDlgItem(IDC_Picture5)->m_hWnd;
	GetDlgItem(IDC_Picture5)->GetClientRect(rect5);
	OpenWindow(rect5.top, rect5.left, rect5.Width(), rect5.Height(), (Hlong)HW5X, "visible", "", &G::HW5);

	HWND   HW6X = GetDlgItem(IDC_Picture6)->m_hWnd;
	GetDlgItem(IDC_Picture6)->GetClientRect(rect6);
	OpenWindow(rect6.top, rect6.left, rect6.Width(), rect6.Height(), (Hlong)HW6X, "visible", "", &G::HW6);
	SetPart(G::HW1, 0, 0, 1200, 1920);
	SetPart(G::HW2, 0, 0, rect2.Width(), rect2.Height());
	SetPart(G::HW3, 0, 0, 1200, 1920);
	SetPart(G::HW4, 0, 0, rect4.Width(), rect4.Height());
	SetPart(G::HW5, 0, 0, 2048, 2440);
	SetPart(G::HW6, 0, 0, 2048, 2440);

	
	//抓取图像线程
	m_hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CameraGrab1, this, 0, 0);
	CloseHandle(m_hThread[0]);
	/*m_hThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CameraGrab2, this, 0, 0);
	CloseHandle(m_hThread[1]);*/
	m_hThread[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CameraGrab3, this, 0, 0);
	CloseHandle(m_hThread[2]);
	/*m_hThread[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CameraGrab4, this, 0, 0);
	CloseHandle(m_hThread[3]);
	m_hThread[17] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CameraGrab5, this, 0, 0);
	CloseHandle(m_hThread[17]);
	m_hThread[18] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CameraGrab6, this, 0, 0);
	CloseHandle(m_hThread[18]);*/
	//6轴运动线程
	m_hThread[6] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AxisMove, this, 0, 0);
	CloseHandle(m_hThread[6]);
	//7 获取位置触发相机线程
	m_hThread[7] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CameraTrigger, this, 0, 0);
	CloseHandle(m_hThread[7]);
	//8,9,10,11图像处理线程
	m_hThread[8] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ImageDispose1, this, 0, 0);
	CloseHandle(m_hThread[8]);
	m_hThread[9] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ImageDispose2, this, 0, 0);
	CloseHandle(m_hThread[9]);
	m_hThread[10] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ImageDispose3, this, 0, 0);
	CloseHandle(m_hThread[10]);
	m_hThread[11] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ImageDispose4, this, 0, 0);
	CloseHandle(m_hThread[11]);
	///结果分析线程
	m_hThread[12] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Analysis1, this, 0, 0);
	CloseHandle(m_hThread[12]);
	m_hThread[13] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Analysis2, this, 0, 0);
	CloseHandle(m_hThread[13]);
	//吹气线程
	m_hThread[14] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Blow1, this, 0, 0);
	CloseHandle(m_hThread[14]);
	m_hThread[15] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Blow2, this, 0, 0);
	CloseHandle(m_hThread[15]);
	//刷新板卡及相关状态线程
	m_hThread[16] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RefreshCardState, this, 0, 0);
	CloseHandle(m_hThread[16]);
	//尺寸数据写入数据库线程
	/*m_hThread[19] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ProductionToDB, this, 0, 0);*/
	CloseHandle(m_hThread[19]);

	//SkinH_DetachEx(GetDlgItem(IDC_LEFTRESULT_STATIC)->m_hWnd);
	//SkinH_DetachEx(GetDlgItem(IDC_RIGHTRESULT_STATIC)->m_hWnd);
	
	/////////////////////////////////////////////////////////excel/////////////
	CString  filepath1 = L"D:\\空气柱检测数据\\尺寸数据";         //
	CTime  time = CTime::GetCurrentTime();
	CString strRptTime = time.Format("%Y%m%d");
	excelpath = filepath1 + strRptTime + L".xlsx";
	//if (0 == filerw.OpenExcel(excelpath))
	//	G::bOpenExcelOk = 1;
	//列表控件初始化
	CRect    rectlist1,rectlist2;
	GetDlgItem(IDC_DATA_LIST)->GetClientRect(&rectlist1);
	m_data_list.InsertColumn(0, L"左产品NO", LVCFMT_CENTER, rectlist1.Width() * 2 / 11, 0);
	m_data_list.InsertColumn(1, L"总长", LVCFMT_CENTER, rectlist1.Width() *3/ 11, 1);
	m_data_list.InsertColumn(2, L"左侧外径", LVCFMT_CENTER, rectlist1.Width() *3/ 11, 2);
	m_data_list.InsertColumn(3, L"右侧外径", LVCFMT_CENTER, rectlist1.Width() *3/ 11, 3);
	GetDlgItem(IDC_DATA_LIST2)->GetClientRect(&rectlist2);
	m_datalist2.InsertColumn(0, L"右产品NO", LVCFMT_CENTER, rectlist2.Width() * 2 / 11, 0);
	m_datalist2.InsertColumn(1, L"总长", LVCFMT_CENTER, rectlist2.Width() * 3 / 11, 1);
	m_datalist2.InsertColumn(2, L"左侧外径", LVCFMT_CENTER, rectlist2.Width() * 3 / 11, 2);
	m_datalist2.InsertColumn(3, L"右侧外径", LVCFMT_CENTER, rectlist2.Width() * 3 / 11, 3);

	SetTimer(1, 300, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy1269AirColumnDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy1269AirColumnDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy1269AirColumnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy1269AirColumnDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	ExitSoft();
}


// 退出软件
void CMy1269AirColumnDlg::ExitSoft()
{
	if((0==G::MCard.A[AXIS_LEFT].AS.bDone)||(0==G::MCard.A[AXIS_LEFT].AS.bDone))
	{
		AfxMessageBox(L"轴运动中,请先停止轴运动");
		return;
	}
	if (MessageBox(L"确定退出软件？", L"提示", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		//HalconCpp::ClearShapeModel(G::modle1);
		//filerw.release();
		//Sleep(300);
		////关闭板卡
		//G::MCard.CloseMotionCard();
		//Sleep(100);

		CDialogEx::OnCancel();
	}
}

//菜单退出按钮
void CMy1269AirColumnDlg::OnExitMenu()
{
	// TODO:  在此添加命令处理程序代码
	ExitSoft();
}

//菜单参数设置按钮
void CMy1269AirColumnDlg::OnParasetMenu()
{
	// TODO:  在此添加命令处理程序代码
	CPSWDlg  pswdlg;
	pswdlg.DoModal();
}

//菜单IO设置按钮
void CMy1269AirColumnDlg::OnIoset()
{
	// TODO:  在此添加命令处理程序代码
	//创建非模态对话框
	if (NULL == ioset)
	{
		ioset = new CIOSETDlg();
		ioset->Create(IDD_IOSET_DIALOG, this);
	}
	//显示非模态对话框
	ioset->ShowWindow(SW_SHOW);
}


void CMy1269AirColumnDlg::OnMotionMenu()
{
	// TODO:  在此添加命令处理程序代码
	CMotionDlg  motiondlg;
	motiondlg.DoModal();
}



// 读取参数 初次进入程序使用
int CMy1269AirColumnDlg::ReadPara()
{
	///////////////////////////////////通用参数
	//打开记录集
	try
	{
		//注意：在用 Open 方法打开记录集时使用了 adOpenStatic 游标；如果使用 adOpenDynamic 游标，GetRecordCount 方法将返回 -1 。
		G::db.m_pRecordset->Open("SELECT  *  FROM " + (_bstr_t)ParaTableName, G::db.m_pConnection.GetInterfacePtr(), adOpenStatic, adLockPessimistic, adCmdText);
	}
	catch (_com_error a)
	{
		CString  err;
		err = a.ErrorMessage();
		AfxMessageBox(err + L"失败,请重启软件");
	}
	int  i = G::db.m_pRecordset->GetRecordCount();
	if (i < 2)           //表中没有数据
	{
		G::WriteCommonPara();         //将默认参数写入
		return 0;
	}
	//非空，则读取参数
	/////////////////////////////////通用参数////////////////////
	try
	{
		G::db.m_pRecordset->MoveFirst();
		for (int i = 0; i < 150; i++)
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
				return 0;
			}
		}             //for
	}
	catch (_com_error a)
	{
		CString  err;
		err = a.ErrorMessage();
		AfxMessageBox(err + L"失败,请重启软件1");
	}

	

	//CFileFind finder;
	//CString    str;
	//CString    str2 = L"参数名";                            //参数名
	//CString    str3 = L"参数值";                            //参数值
	//CString    str4 = L"参数最大值";                     //参数最大值
	//CString    str5 = L"参数最小值";                    //参数最小值
	//CString    str6 = L"备注";                             //备注
	////查看是否存在此文件
	//BOOL ifinder = finder.FindFile(G::ParaFileName/*L".//1269 Air Column para.ini"*/);
	////文件为空用默认参数
	//if (!ifinder)                
	//{
	//	return -1;
	//}
	////文件存在就读取文件中参数
	//else                        
	//{
	//	try                //保护代码段
	//	{
	//		//输入端口名
	//		for (int i = 0; i < 16; i++)
	//		{
	//			str.Format(L"%d号输入口", i);
	//			::GetPrivateProfileString(L"InputPort", str, L"输入默认名", G::MCard.I[i].lpwstrPortName.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//		}
	//		//输出端口名
	//		for (int i = 0; i < 16; i++)
	//		{
	//			str.Format(L"%d号输出口", i);
	//			::GetPrivateProfileString(L"OutPort", str, L"输出默认名", G::MCard.O[i].lpwstrPortName.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//		}

	//		///////////////////各参数值
	//		//运动参数              0到29
	//		for (int i = 0; i < 30; i++)
	//		{
	//			int j = i;
	//			str2.Format(L"参数名%d", i);
	//			GetPrivateProfileString(L"运动参数", str2, L"默认", G::PR[j].paraname.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//			str3.Format(L"参数值%d", i);
	//			G::PR[j].paravalue = ::GetPrivateProfileInt(L"运动参数", str3, 100, G::ParaFileName);
	//			str4.Format(L"最大值%d", i);
	//			G::PR[j].paraMaxValue = ::GetPrivateProfileInt(L"运动参数", str4, 60000, G::ParaFileName);
	//			str5.Format(L"最小值%d", i);
	//			G::PR[j].paraMinValue = ::GetPrivateProfileInt(L"运动参数", str5, 0, G::ParaFileName);
	//			str6.Format(L"备注%d", i);
	//			GetPrivateProfileString(L"运动参数", str6, L"默认", G::PR[j].remark.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//		}
	//		//相机参数             30到89
	//		for (int i = 0; i < 60; i++)
	//		{
	//			int j = i+30;
	//			str2.Format(L"参数名%d", i);
	//			GetPrivateProfileString(L"相机参数", str2, L"默认", G::PR[j].paraname.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//			str3.Format(L"参数值%d", i);
	//			G::PR[j].paravalue = ::GetPrivateProfileInt(L"相机参数", str3, 100, G::ParaFileName);
	//			str4.Format(L"最大值%d", i);
	//			G::PR[j].paraMaxValue = ::GetPrivateProfileInt(L"相机参数", str4, 60000, G::ParaFileName);
	//			str5.Format(L"最小值%d", i);
	//			G::PR[j].paraMinValue = ::GetPrivateProfileInt(L"相机参数", str5, 0, G::ParaFileName);
	//			str6.Format(L"备注%d", i);
	//			GetPrivateProfileString(L"相机参数", str6, L"默认", G::PR[j].remark.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//		}
	//		//视觉参数
	//		for (int i = 0; i < 60; i++)
	//		{
	//			int j = i + 90;
	//			str2.Format(L"参数名%d", i);
	//			GetPrivateProfileString(L"视觉参数", str2, L"默认", G::PR[j].paraname.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//			str3.Format(L"参数值%d", i);
	//			G::PR[j].paravalue = ::GetPrivateProfileInt(L"视觉参数", str3, 100, G::ParaFileName);
	//			str4.Format(L"最大值%d", i);
	//			G::PR[j].paraMaxValue = ::GetPrivateProfileInt(L"视觉参数", str4, 60000, G::ParaFileName);
	//			str5.Format(L"最小值%d", i);
	//			G::PR[j].paraMinValue = ::GetPrivateProfileInt(L"视觉参数", str5, 0, G::ParaFileName);
	//			str6.Format(L"备注%d", i);
	//			GetPrivateProfileString(L"视觉参数", str6, L"默认", G::PR[j].remark.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//		}
	//		//生产数据
	//		for (int i = 0; i < 10; i++)
	//		{
	//			int  j = i + 150;
	//			G::PR[j].paravalue = ::GetPrivateProfileInt(L"生产数据", G::PR[j].paraname, 0, G::ParaFileName);
	//		}
	//	}
	//	catch (...)
	//	{
	//		AfxMessageBox(L"读取参数失败！");
	//		return  -1;
	//	}

	//}

	return 0;
}


// 将参数写入文件
int CMy1269AirColumnDlg::WritePara()
{
	CString  str;

	G::WriteIOPara();
	G::WriteCommonPara();
	G::WriteProduction();

	return 0;
}

//复位按钮
void CMy1269AirColumnDlg::OnBnClickedRstButton()
{
	// TODO:  在此添加控件通知处理程序代码
	ReadPara();
	CString   grabitemstr;
	GetDlgItemText(IDC_REALTIME_BUTTON, grabitemstr);
	if (G::MCard.I[SCRAM_BUTTON].sValue == 0)
	{
		AfxMessageBox(L"急停已按下！");
		Sleep(10);
		return;
	}
	//实时中
	if (L"关闭实时" == grabitemstr)
	{
		AfxMessageBox(L"实时拍照中，不能复位，请先关闭实时!");
		return;
	}
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"正在运行中，不能复位！");
		Sleep(10);
		return;
	}
	G::bRunning=0;
	G::bCanGrab=0;
	G::bRunflag = 0;

	GetDlgItem(IDC_RST_BUTTON)->EnableWindow(FALSE);
	G::bResetOK = 0;
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_REALTIME_BUTTON)->EnableWindow(FALSE);

	m_hThread[4] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Reset, this, 0, 0);
}


HBRUSH CMy1269AirColumnDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	int   ID = pWnd->GetDlgCtrlID();

	//if (ID = IDC_LEFTRESULT_STATIC)
	//	pDC->SetTextColor(RGB(100, 255, 100));               //绿色字体
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//实时抓取图像
void CMy1269AirColumnDlg::OnBnClickedRealtimeButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString   itemstr;
	if (G::MCard.I[SCRAM_BUTTON].sValue == 0)
	{
		AfxMessageBox(L"急停已按下！");
		Sleep(10);
		return;
	}
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"运行中，不能开启实时！");
		return;
	}
	GetDlgItemText(IDC_REALTIME_BUTTON, itemstr);
	//G::bCanGrab=1;
	//当前非实时状态
	if (L"实时" == itemstr)
	{
		//创建实时抓取线程
		SetDlgItemText(IDC_REALTIME_BUTTON, L"关闭实时");
		m_hThread[5] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RealTimeGrab, this, 0, 0);
		CloseHandle(m_hThread[5]);
	}
	//当前实时状态
	else
	{
		SetDlgItemText(IDC_REALTIME_BUTTON, L"实时");
		//退出实时线程
		m_exitThread[5] = 1;

		//try
		//{
		//	//设置相机为触发拍照
		//SetFramegrabberParam(G::hv_AcqHandle1, "TriggerSelector", "AcquisitionStart");
		//SetFramegrabberParam(G::hv_AcqHandle1, "TriggerMode", "On");

		///*SetFramegrabberParam(G::hv_AcqHandle2, "TriggerSelector", "AcquisitionStart");
		//SetFramegrabberParam(G::hv_AcqHandle2, "TriggerMode", "On");
		//SetFramegrabberParam(G::hv_AcqHandle3, "TriggerSelector", "AcquisitionStart");
		//SetFramegrabberParam(G::hv_AcqHandle3, "TriggerMode", "On");
		//SetFramegrabberParam(G::hv_AcqHandle4, "TriggerSelector", "AcquisitionStart");
		//SetFramegrabberParam(G::hv_AcqHandle4, "TriggerMode", "On");*/
		//SetDlgItemText(IDC_REALTIME_BUTTON, L"实时");
		//}
		//catch(...)
		//{
		//	AfxMessageBox(L"关闭实时失败!");
		//}
		//
	}
}

//抓图按钮
void CMy1269AirColumnDlg::OnBnClickedGrabButton()
{
	// TODO:  在此添加控件通知处理程序代码

	CString   grabitemstr;
	GetDlgItemText(IDC_REALTIME_BUTTON, grabitemstr);
	if (L"关闭实时" == grabitemstr)
	{
		AfxMessageBox(L"实时拍照中，不能抓图，请先关闭实时!");
		return;
	}
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"运行过程中不允许抓图!");
		return;
	}
	//G::bCanGrab=1;
	G::CameraTrigger(1);
	G::CameraTrigger(2);
	//G::CameraTrigger(3);
	//G::CameraTrigger(4);
}

//启动按钮         
void CMy1269AirColumnDlg::OnBnClickedStartButton()
{
	// TODO:  在此添加控件通知处理程序代码
	Start();
}


// 开始运行        (启动，暂停，继续)
int CMy1269AirColumnDlg::Start()
{
	CString   strstart, strrealtime;
	GetDlgItemText(IDC_START_BUTTON, strstart);
	GetDlgItemText(IDC_REALTIME_BUTTON, strrealtime);
	//实时拍照中
	if (L"关闭实时" == strrealtime)
	{
		AfxMessageBox(L"实时拍照中，不能启动！");
		return -1;
	}
	if (0 == G::bResetOK)
	{
		AfxMessageBox(L"请先复位！");
		return -1;
	}
	//未运行状态，  
	if (L"启动" == strstart || L"继续" == strstart)
	{
		//首次启动
		if (L"启动" == strstart)
		{
			if ((0 == G::bLeftNoShield) && (0 == G::bRightNoShield))
			{
				//G::bRunning = 0;
				MessageBox(L"两端全部屏蔽，已暂停执行，请至少启用一端再开始执行！", L"提示", MB_OK);
				return 0;
			}
			GetDlgItem(IDC_RST_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_REALTIME_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_GRAB_BUTTON)->EnableWindow(FALSE);
		}
		//重新读取参数
		//ReadPara();
		
		//dmc_set_profile(0,0,1000,G::PR[1].paravalue,0.01,0.01,1000);
		G::bRunflag = 1;
		G::bCanGrab = 1;
		G::bRunning = 1;
		SetDlgItemText(IDC_START_BUTTON, L"暂停");
	}
	//运行中
	else
	{
		//轴立刻停止，立刻停止抓图
		G::MCard.StopAxis(0, 1);
		//延迟一段时间再复位运行中标志
		G::bRunning = 0;
		/*Sleep(200);
		G::bCanGrab = 0;*/
		SetDlgItemText(IDC_START_BUTTON, L"继续");
	}

	return 0;
}


void CMy1269AirColumnDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//if (1==G::bOpenExcelOk)
	//	WriteCheckDataToExcel();

	//CString  filepath1 = L"D:\\空气柱检测数据\\尺寸数据";         //
	//CTime  time = CTime::GetCurrentTime();
	//CString strRptTime = time.Format("%Y%m%d");
	//excelpath = filepath1 + strRptTime + L".xlsx";
	CString   str;
	str.Format(L"%ld", G::grab1counts);
	SetDlgItemText(IDC_GRAB1COUNTS_STATIC, str);
	str.Format(L"%ld", G::grab2counts);
	SetDlgItemText(IDC_GRAB2COUNTS_STATIC, str);
	str.Format(L"%ld", G::grab1delete);
	SetDlgItemText(IDC_GRAB1DELETE_STATIC, str);
	str.Format(L"%ld", G::grab2delete);
	SetDlgItemText(IDC_GRAB2DELETE_STATIC, str);
	str.Format(L"%ld", G::triggercounts1);
	SetDlgItemText(IDC_TRIGGERCOUNTS1_STATIC, str);
	str.Format(L"%ld", G::triggercounts2);
	SetDlgItemText(IDC_TRIGGERCOUNTS2_STATIC, str);

	CDialogEx::OnTimer(nIDEvent);
}

//清除生产信息 菜单按钮
void CMy1269AirColumnDlg::OnClearProduct()
{
	// TODO:  在此添加命令处理程序代码
	if (IDOK == MessageBox(L"确定清除生产记录？", L"提示", MB_OKCANCEL))
	{
		G::PR[110].paravalue = 0;
		G::PR[111].paravalue = 0;
		G::PR[112].paravalue = 0;
		G::PR[113].paravalue = 0;
		//G::PR[114].paravalue = 0;
		//G::PR[115].paravalue = 0;
	}

}

//停止按钮
void CMy1269AirColumnDlg::OnBnClickedStopButton()
{
	// TODO:  在此添加控件通知处理程序代码
	G::MCard.StopAxis(AXIS_LEFT, 1);
	G::MCard.StopAxis(AXIS_RIGHT, 1);
	/*if(0==G::bRunning)
		G::MCard.StopAxis(0,1);*/
	//表清空
	G::ClearLinked();         
	//数值清空
	//相机触发计数初始化
	G::GridCounts1 = 0;
	G::GridCounts2 = 0;

	G::grab1counts = 0;                          //图像1抓图数量
	G::grab2counts = 0;                          //图像2抓图数量
	G::grab1delete = 0;                           //图像1删除数量
	G::grab2delete = 0;                           //图像2删除数量
	G::triggercounts1 = 0;
	G::triggercounts2 = 0;
	G::blow1 = 0;
	G::blow2 = 0;
	//标志复位
	G::bCanGrab = 0;
	G::bResetOK = 0;
	G::bRunning = 0;
	G::bRunflag = 0;
	SetDlgItemText(IDC_START_BUTTON, L"启动");
	//恢复按钮使用
	GetDlgItem(IDC_RST_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_REALTIME_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_GRAB_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(TRUE);
}


// 将检测数据写入Excel
void CMy1269AirColumnDlg::WriteCheckDataToExcel()
{
	///////////////////////////////////////////将尺寸数据写入文件
	//int dwt = 0;
	//CheckSize    sizelink;
	//sizelink.Diameter = 2;
	//sizelink.TotalLength = 15;
	////sizelink.i = 1;
	//G::SizelistToFile1.AddTail(sizelink);
	//sizelink.Diameter = 4;
	//sizelink.TotalLength = 16;
	////sizelink.i = 2;
	//G::SizelistToFile2.AddTail(sizelink);
	////CString  excelpath1 = L"D:\\空气柱检测数据\\aaa.xlsx";

	////对应链表中有数据就写
	//if (G::SizelistToFile1.GetCount() >= 1 && G::SizelistToFile2.GetCount() >= 1)
	//{
	//	filerw.InsertOneItem1269(excelpath);
	//}


	//CString  excelpath;                       //写入文件路径名
	//CString  filepath1 = L"D:\\空气柱检测数据\\尺寸数据";         //
	//CTime  time = CTime::GetCurrentTime();
	//CString strRptTime = time.Format("%Y%m%d");
	//excelpath = filepath1 + strRptTime + L".xlsx";
	//int savedata = ((CButton*)GetDlgItem(IDC_DATASAVE_CHECK))->GetCheck();
	//if (savedata)             ////选择将检测数据保存
	//{
	//	if (G::SizelistToFile1.GetCount() >= SaveResultMin&&G::SizelistToFile1.GetCount() >= SaveResultMin)
	//	{
	//		filerw.ResultLinkedToExcel(1, excelpath, 1, 1);
	//	}
	//}
}


// 更新其他状态信息，三色灯，控件数据，参数等
void CMy1269AirColumnDlg::RefreshStatus()
{
	CString   str;
	CString   str1;
	//CheckSize   checksize;

	///////////////////////////////////////////////////更新生产数据
	G::PR[111].paravalue = G::PR[110].paravalue - G::PR[112].paravalue;           //OK总数
	if (G::PR[110].paravalue != 0)
		G::PR[113].paravalue = 100 * G::PR[111].paravalue / G::PR[110].paravalue;     //良率
	//将生产数据显示到控件
	str.Format(L"%.2lf PCS", G::PR[110].paravalue);
	SetDlgItemText(IDC_TOTALCOUNTS_STATIC, str);                 //总产量
	str.Format(L"%.2lf PCS", G::PR[111].paravalue);
	SetDlgItemText(IDC_OKCOUNTS_STATIC, str);                      //OK数
	str.Format(L"%.2lf PCS", G::PR[112].paravalue);
	SetDlgItemText(IDC_NGCOUNTS_STATIC, str);                    //NG数
	str.Format(L"%.2lf", G::PR[113].paravalue);
	str1 = str + L"%";
	SetDlgItemText(IDC_YIELD_STATIC, str1);                            //良率
	//UPH
	str.Format(L"%.2lf PCS/H", G::PR[1].paravalue*3600*2/3500);
	SetDlgItemText(IDC_AVGUPH_STATIC, str);
	//CT
	str.Format(L"%.2lf S", 1750/G::PR[1].paravalue);
	SetDlgItemText(IDC_CT_STATIC, str);

	//保存生产数据
	G::WriteProduction();
	///////////////////////////////////////////////////////三色灯
	//待机状态
	if (G::bRunning == 0 && 0 == G::bAlarm)
	{
		if (G::MCard.O[YELLOW_LED].sValue == 0)
		{
			G::MCard.SetDoBit(YELLOW_LED, 1);
		}
		G::MCard.SetDoBit(GREEN_LED, 0);
		G::MCard.SetDoBit(RED_LED, 0);
		G::MCard.SetDoBit(BUZZER, 0);
	}
	//报警中
	if (1 == G::bAlarm)
	{
		if (0 == G::MCard.O[RED_LED].sValue)
			G::MCard.SetDoBit(RED_LED, 1);
		if (0 == G::MCard.O[BUZZER].sValue)
			G::MCard.SetDoBit(BUZZER, 1);
		G::MCard.SetDoBit(GREEN_LED, 0);
		G::MCard.SetDoBit(YELLOW_LED, 0);
	}
	//正常运行中
	if (1 == G::bRunning)
	{
		if (0 == G::MCard.O[GREEN_LED].sValue)
		G::MCard.SetDoBit(GREEN_LED, 1);
		G::MCard.SetDoBit(RED_LED, 0);
		G::MCard.SetDoBit(YELLOW_LED, 0);
		G::MCard.SetDoBit(BUZZER, 0);
	}
}


//void CMy1269AirColumnDlg::OnBnClickedButton1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	    test=true;
//		m_hThread[18] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TTEST, this, 0, 0);
//}


BOOL CMy1269AirColumnDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case  VK_SPACE:
			Start();
			return  TRUE;
			break;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


// 运行中变成暂停处理
void CMy1269AirColumnDlg::ToPause()
{
	SetDlgItemText(IDC_START_BUTTON, L"继续");
	G::bRunning = 0;
}

//屏蔽左端
void CMy1269AirColumnDlg::OnBnClickedShieldleftButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString	  str;
	GetDlgItemText(IDC_SHIELDLEFT_BUTTON, str);
	//未屏蔽
	if (L"屏蔽左端" == str)
	{
		G::bLeftNoShield = 0;
		//停止轴运动
		G::MCard.StopAxis(AXIS_LEFT, 1);
		SetDlgItemText(IDC_SHIELDLEFT_BUTTON, L"启用左端");
		if ((0 == G::bLeftNoShield) && (0 == G::bRightNoShield))
		{
			if (1 == G::bRunning)
				ToPause();
			G::bRunning = 0;
			MessageBox(L"两端全部屏蔽，已暂停执行，请至少启用一端再开始执行！", L"提示", MB_OK);
		}
	}
	//已经屏蔽左端
	else if (L"启用左端" == str)
	{
		G::bLeftNoShield = 1;
		if (1 == G::bRunning)
			dmc_vmove(0, AXIS_LEFT, 1);
		SetDlgItemText(IDC_SHIELDLEFT_BUTTON, L"屏蔽左端");
	}
}

//屏蔽右端
void CMy1269AirColumnDlg::OnBnClickedShieldrightButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString       str;
	GetDlgItemText(IDC_SHIELDRIGHT_BUTTON, str);
	//未屏蔽
	if (L"屏蔽右端" == str)
	{
		G::bRightNoShield = 0;
		G::MCard.StopAxis(AXIS_RIGHT, 1);
		SetDlgItemText(IDC_SHIELDRIGHT_BUTTON, L"启用右端");
		if ((0 == G::bLeftNoShield) && (0 == G::bRightNoShield))
		{
			if (1 == G::bRunning)
				ToPause();
			G::bRunning = 0;
			MessageBox(L"两端全部屏蔽，已暂停执行，请至少启用一端再开始执行！", L"提示", MB_OK);
		}
	}
	//已经屏蔽左端
	else if (L"启用右端" == str)
	{
		G::bRightNoShield = 1;
		if (1 == G::bRunning)
			dmc_vmove(0, AXIS_RIGHT, 1); 
		SetDlgItemText(IDC_SHIELDRIGHT_BUTTON, L"屏蔽右端");
	}
}


void CMy1269AirColumnDlg::ReadIO()
{
	///////////////////////////////////////IO
	CFileFind finder;
	CString    str;
	BOOL ifinder = finder.FindFile(G::ParaFileName);
	//文件为空用默认参数
	if (!ifinder)
	{
		G::WriteIOPara();
		return ;
	}
	else
	{
		try                //保护代码段
		{
			//输入端口名
			for (int i = 0; i < 16; i++)
			{
				str.Format(L"%d号输入口", i);
				::GetPrivateProfileString(L"InputPort", str, L"输入默认名", G::MCard.I[i].lpwstrPortName.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
			}
			//输出端口名
			for (int i = 0; i < 16; i++)
			{
				str.Format(L"%d号输出口", i);
				::GetPrivateProfileString(L"OutPort", str, L"输出默认名", G::MCard.O[i].lpwstrPortName.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
			}
		}
		catch (...)
		{
			AfxMessageBox(L"读取参数失败！");
			return ;
		}
	}
}

//左侧导出数据
void CMy1269AirColumnDlg::OnBnClickedWritedataleftButton()
{
	CFileDialog filedlg(FALSE, L"xlsx", L"左侧尺寸数据", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"excel文件(*.xls)|*.xlsx|ALL Files(*.*)||",this);         //TRUE为打开，FALS为保存
	CString    path;
	// 显示保存文件对话框   
	if (IDOK == filedlg.DoModal())
	{
		// 如果点击了文件对话框上的“保存”按钮，则将选择的文件路径显示到编辑框里   
		path = filedlg.GetPathName();
	}
	
	path = filedlg.GetPathName();             //获取文件路径
	CString   title[] = { L"产品编号", L"总长", L"左侧外径", L"右侧外径" };
	int   colcount = m_data_list.GetHeaderCtrl()->GetItemCount();
	int   rowcount = m_data_list.GetItemCount();

	filerw.ListToExcel(&m_data_list, path, title, rowcount, colcount);
	// TODO:  在此添加控件通知处理程序代码
}

//右侧导出数据
void CMy1269AirColumnDlg::OnBnClickedWritedatarightButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog filedlg(FALSE, L"xlsx", L"右侧尺寸数据", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"excel文件(*.xls)|*.xlsx|ALL Files(*.*)||");         //TRUE为打开，FALS为保存
	CString    path;
	if (IDOK == filedlg.DoModal())
	{
		// 如果点击了文件对话框上的“保存”按钮，则将选择的文件路径显示到编辑框里   
		path = filedlg.GetPathName();
	}
	path = filedlg.GetPathName();             //获取文件路径
	CString   title[] = { L"产品编号", L"总长", L"左侧外径", L"右侧外径" };
	int   colcount = m_datalist2.GetHeaderCtrl()->GetItemCount();
	int   rowcount = m_datalist2.GetItemCount();

	filerw.ListToExcel(&m_datalist2, path, title, rowcount, colcount);
}

//菜单-视觉控制
void CMy1269AirColumnDlg::OnVisondebug()
{
	// TODO:  在此添加命令处理程序代码
	CVisionDebugDlg   visiondebug;
	visiondebug.DoModal();   
}
