
// 1269Air ColumnDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "1269Air ColumnDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using   namespace    HalconCpp;
//һ������14.65um��0.01465mm             252000pulseһȦ

//�ᶯ��
BOOL  CALLBACK  myAxisSafeFunc(UINT uAxis)
{
	//if (uAxis == AXIS_PUTPIN)
	//{
	//	if (1 == G::MCard.I[GetPinUpDownACT].sValue || 0 == G::MCard.I[GetPinUpDownORG].sValue)
	//	{
	//		G::SendMessagebox(L"ȡ�����ײ���ԭλ�����ܶ�����", L"����", MB_ICONWARNING);
	//		return FALSE;
	//	}
	//}
	return TRUE;
}

//IO�������
BOOL   CALLBACK  myDoSafeFunc(UINT uIOIndex, UINT uValue)
{
	/*if ((uIOIndex == GetPinUpDownCylinder) && (uValue == 1))
	{
		long   pos = dmc_get_position(0, AXIS_PUTPIN);
		if ((pos > G::PR[MP_GetPinServoGetPinPos].paravalue - 2000) && (pos < G::PR[MP_GetPinServoGetPinPos].paravalue + 2000))
		{
			if ((1 == G::MCard.I[InPinDownORG].sValue) || (0 == G::MCard.I[InPinDownACT].sValue) || (0 == G::MCard.O[InPinDownCylinder].sValue))
			{
				G::SendMessagebox(L"��������û��ԭλ���������ײ��ܶ�����", L"����", MB_ICONWARNING);
				return FALSE;
			}
		}
	}*/

	return TRUE;
}

//���1ץͼ�̣߳����Ʒ�ܳ���
DWORD     CameraGrab1(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[0] = 0;
	HObject hobjTmp;
	//����һ���µ��б�Ԫ��
	ImageLinked  p1;
	//HTuple  w, h;
	while ((0 == pdlg->m_exitThread[0]))
	{
		if (1/*G::bCanGrab*//* || FALSE==G::bRunning*/)
		{
			try
			{	//���� �������� �����д����ƵĲ��л����ñ���ץͼ
				//LONGLONG llToken = ::GetTickCount();
				//HalconCpp::GenEmptyObj(&hobjTmp);�������̫����Ҫ16ms
				//��ʼץͼ
				//--------------------------------------------------------------------------------------------
				//ͬ���ɼ�                  �첽�ɼ�Ϊgrabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle1);//�������� ��������ʼ������������������������������
				HalconCpp::DispImage(hobjTmp, G::HW1);
				
				/////////////////////////////////////���й����вŰ�ͼ���������//////////////////////
				if (1 == G::bCanGrab)
				{
					//����Ԫ�ز����б�
					//��ץȡ����ͼ������б�β
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
				pdlg->MessageBox(L"���1ץͼ����,���������!",L"����",MB_ICONWARNING);
				pdlg->m_exitThread[0] = 1;
			}
			Sleep(1);
			//if (pdlg->m_exitThread[0] > 0)
			//{//�˳��߳�
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

//���2ץͼ�̣߳����Ʒ����⾶��
DWORD     CameraGrab2(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[1] = 0;
	HObject hobjTmp;
	ImageLinked  p2;              	//����һ���µ��б�Ԫ��
	//HTuple  w, h;
	while ((0 == pdlg->m_exitThread[1]))
	{
		if (TRUE /*== G::bCanGrab*/)
		{
			try
			{
				//HalconCpp::GenEmptyObj(&hobjTmp);�������̫����Ҫ16ms
				//��ʼץͼ
				//--------------------------------------------------------------------------------------------
				//ͬ���ɼ�                  �첽�ɼ�Ϊgrabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle2);//�������� ��������ʼ������������������������������
				//--------------------------------------------------------------------------------------------
				int bSave2 = ((CButton*)pdlg->GetDlgItem(IDC_saveimage2))->GetCheck();
				if (bSave2)
				{
					pdlg->imageno2++;
					CStringA str;
					str.Format("D:/savedphoto2/GO_%d.tiff", pdlg->imageno2);
					WriteImage(hobjTmp, "tiff", 0, str.GetString());
				}
				//G::GTCard.SetDoBitReverse(OP_Buzzer,1,50);	//��������
				/*iCountTest++;
				pDlg->SetDlgItemInt(IDC_COUNTTEST,iCountTest);*/
			/*	GetImageSize(hobjTmp, &w, &h);
				SetPart(G::HW2, 0, 0, h, w);*/
				HalconCpp::DispImage(hobjTmp, G::HW2);
				if (G::bCanGrab)
				{
					//����Ԫ�ز����б�
					//��ץȡ����ͼ������б�β
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
				pdlg->MessageBox(L"���2ץͼ����,���������!",L"����",MB_ICONWARNING);
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

//���3ץͼ�̣߳����Ʒ�Ҷ��⾶��
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
				//HalconCpp::GenEmptyObj(&hobjTmp);�������̫����Ҫ16ms
				//��ʼץͼ
				//--------------------------------------------------------------------------------------------
				//ͬ���ɼ�                  �첽�ɼ�Ϊgrabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle3);//�������� ��������ʼ������������������������������
				HalconCpp::DispImage(hobjTmp, G::HW3);
				
				if (G::bCanGrab)
				{
					//����Ԫ�ز����б�
					//��ץȡ����ͼ������б�β
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
				pdlg->MessageBox(L"���3ץͼ����,���������!",L"����",MB_ICONWARNING);
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
		//{//�˳��߳�
		//	TRACE("Camera1Grab Thread Exiting \r\n");
		//	break;
		//}
	}
	pdlg->m_exitThread[2] = 0;
	TRACE(L"Camera3Grab Thread Exiting \r\n");
	//HalconCpp::CloseFramegrabber(G::hv_AcqHandle3);
	return TRUE;
}

//���4ץͼ�̣߳��Ҳ�Ʒ�ܳ���
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
				//HalconCpp::GenEmptyObj(&hobjTmp);�������̫����Ҫ16ms
				//��ʼץͼ
				//--------------------------------------------------------------------------------------------
				//ͬ���ɼ�                  �첽�ɼ�Ϊgrabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle4);//�������� ��������ʼ������������������������������
				HalconCpp::DispImage(hobjTmp, G::HW4);

				if (G::bCanGrab)
				{
					//����Ԫ�ز����б�
					//��ץȡ����ͼ������б�β
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
				pdlg->MessageBox(L"���4ץͼ����,���������!",L"����",MB_ICONWARNING);
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

//���5ץͼ�̣߳��Ҳ�Ʒ-����⾶��ۣ�
DWORD     CameraGrab5(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[17] = 0;
	HObject hobjTmp;
	//HTuple  w, h;
	//����һ���µ��б�Ԫ��
	ImageLinked  p5;
	while ((0 == pdlg->m_exitThread[17]))
	{
		if (1/*G::bCanGrab*//* || FALSE==G::bRunning*/)
		{
			try
			{	
				//HalconCpp::GenEmptyObj(&hobjTmp);�������̫����Ҫ16ms
				//��ʼץͼ
				//--------------------------------------------------------------------------------------------
				//ͬ���ɼ�                  �첽�ɼ�Ϊgrabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle5);//�������� ��������ʼ������������������������������
				HalconCpp::DispImage(hobjTmp, G::HW5);

				/////////////////////////////////////���й����вŰ�ͼ���������//////////////////////
				if (1 == G::bCanGrab)
				{
					//����Ԫ�ز����б�
					//��ץȡ����ͼ������б�β
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
				pdlg->MessageBox(L"���5ץͼ����,���������!",L"����",MB_ICONWARNING);
				pdlg->m_exitThread[17] = 1;
			}
			Sleep(1);
			//if (pdlg->m_exitThread[0] > 0)
			//{//�˳��߳�
			//	TRACE("Camera1Grab Thread Exiting \r\n");
			//	break;
			//}
		}                //if(cangrab)
		Sleep(1);
		//////////////////////////////////////////old ץͼ��ʽ   ///////////////////////////////////

		//if (TRUE == G::bCanGrab)
		//{
		//	try
		//	{	//���� �������� �����д����ƵĲ��л����ñ���ץͼ
		//		LONGLONG llToken = ::GetTickCount();
		//		//HalconCpp::GenEmptyObj(&hobjTmp);�������̫����Ҫ16ms
		//		//��ʼץͼ
		//		//--------------------------------------------------------------------------------------------
		//		//ͬ���ɼ�                  �첽�ɼ�Ϊgrabimagestart
		//		HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle5);//�������� ��������ʼ������������������������������
		//		//--------------------------------------------------------------------------------------------
		//		int bSave5 = ((CButton*)pdlg->GetDlgItem(IDC_saveimage4))->GetCheck();
		//		pdlg->imageno5++;
		//		if (bSave5)
		//		{
		//			CStringA str;
		//			str.Format("D:/savedphoto5/GO_%d.tiff", pdlg->imageno5);
		//			WriteImage(hobjTmp, "tiff", 0, str.GetString());
		//		}
		//		//G::GTCard.SetDoBitReverse(OP_Buzzer,1,50);	//��������
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
		//		AfxMessageBox(L"���5ץͼ����,���������!");
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
		//		//����Ԫ�ز����б�
		//		//����һ���µ��б�Ԫ��
		//		ImageLinked  p5;
		//		//��ץȡ����ͼ������б�β
		//		p5.image = hobjTmp;
		//		G::Imagelist5.AddTail(p5);
		//	}
		//	Sleep(5);
		//	//Sleep(100);
		//	if (pdlg->m_exitThread[17] > 0)
		//	{//�˳��߳�
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

//���6ץͼ�̣߳��Ҳ�Ʒ�Ҷ��⾶��ۣ�
DWORD     CameraGrab6(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg* pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[18] = 0;
	HObject hobjTmp;
	//����һ���µ��б�Ԫ��
	ImageLinked  p6;
	//HTuple  w, h;
	while ((0 == pdlg->m_exitThread[18]))
	{
		if (1/*G::bCanGrab*//* || FALSE==G::bRunning*/)
		{
			try
			{	//���� �������� �����д����ƵĲ��л����ñ���ץͼ
				//LONGLONG llToken = ::GetTickCount();
				//HalconCpp::GenEmptyObj(&hobjTmp);�������̫����Ҫ16ms
				//��ʼץͼ
				//--------------------------------------------------------------------------------------------
				//ͬ���ɼ�                  �첽�ɼ�Ϊgrabimagestart
				HalconCpp::GrabImage(&hobjTmp, G::hv_AcqHandle6);//�������� ��������ʼ������������������������������
				HalconCpp::DispImage(hobjTmp, G::HW6);

				/////////////////////////////////////���й����вŰ�ͼ���������//////////////////////
				if (1 == G::bCanGrab)
				{
					//����Ԫ�ز����б�
					//��ץȡ����ͼ������б�β
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
				pdlg->MessageBox(L"���6ץͼ����,���������!",L"����",MB_ICONWARNING);
				pdlg->m_exitThread[18] = 1;
			}
			Sleep(1);
			//if (pdlg->m_exitThread[0] > 0)
			//{//�˳��߳�
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

//4��λ�߳�
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
			pdlg->MessageBox(L"��λ��ɣ�",L"��ʾ",MB_OK);
			break;
		case 1:             //��λʧ��
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
				//����������ŷ�
				G::MCard.SetDoBit(LEFTRADIOTUBENG, 1);
				G::MCard.SetDoBit(RIGHTRADIOTUBENG, 1);
				//��λ������
				if (MCARD_ERROR == G::MCard.HomeAxis2(AXIS_LEFT, 1, 1, 2))
				{
					pdlg->MessageBox(L"����Ḵλʧ�ܣ������¸�λ��", L"����", MB_ICONERROR);
					dwt = 0;
					Resetstep = 1;                            //��λʧ��
					pdlg->m_exitThread[4] = 1;
					break;
				}
				if (MCARD_ERROR == G::MCard.HomeAxis2(AXIS_RIGHT, 1, 1, 2))
				{
					pdlg->MessageBox(L"�Ҷ��Ḵλʧ�ܣ������¸�λ��", L"����", MB_ICONERROR);
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
					pdlg->MessageBox(L"����Ḵλ��ʱ��", L"����", MB_ICONERROR);
					Resetstep = 1;
					break;
				}
				if (0 == G::MCard.A[AXIS_RIGHT].AS.bORGSnr)
				{
					pdlg->MessageBox(L"�Ḵλ��ʱ!", L"����", MB_ICONERROR);
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

//5ʵʱ�߳�
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

//6���˶��߳�
DWORD     AxisMove(LPVOID lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[6] = 0;
	int  step = 10;
	int  ntimes = 0;
	while (0 == pdlg->m_exitThread[6])
	{
		//������
		if (TRUE == G::bRunning)
		{
			//�״ο�ʼִ��ָ��
			if (ntimes == 0)
			{
				//�����������
				//0,������   1������
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
		//ֹͣ���˶�
		else
		{
			if(1==ntimes)
			{
				//0����ֹͣ  1����ֹͣ
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

//7��ȡλ�ô�����������߳�
DWORD    CameraTrigger(LPVOID lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[7] = 0;
	long  pos, pos1;
	while (0 == pdlg->m_exitThread[7])
	{
		if (1 == G::bRunning)
		{//20ms�ӳٽ�����һ��              �����ٶ������ռ����������
			//���������λ
			if (1 == G::bLeftNoShield)
			{
				pos = dmc_get_position(0, 0);                   //��ǰλ��
				if (((pos - G::PR[MP_LeftCamFirstPos].paravalue) >= G::PR[MP_LeftCamIntervalDis].paravalue*G::GridCounts1 - 300) && ((pos - G::PR[MP_LeftCamFirstPos].paravalue) <= G::PR[MP_LeftCamIntervalDis].paravalue*G::GridCounts1 + 300))
				{
					G::CameraTrigger(1);
					G::triggercounts1++;
					G::GridCounts1++;

					//pdlg->m_message_listbox.AddString(L"trigger1");
				}
			}
			
			//���������λ
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

//8ͼ�����߳�1(���Ʒ�ܳ���ë��)
//ѡ�б�Ե����Ե���߼���Ƿ����ë��
DWORD     ImageDispose1(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[8] = 0;
	ProductPositive   positivelink1;
	ImageLinked   imagelink1;
	CString         str;
	int  bResultSave1;
	//int  n;                //��¼����
	while (0==pdlg->m_exitThread[8])
	{
		if (G::Imagelist1.GetCount() > 0)
		{
			G::m_criticalsection_imagelist1.Lock();
			imagelink1 = G::Imagelist1.GetHead();              //��Ϊ�գ�ȡ��ͷԪ��
			//ɾ��ͼ��
			G::Imagelist1.RemoveHead();
			G::m_criticalsection_imagelist1.Unlock();
			
			//����ͼ����

			G::ImageDispose1(imagelink1.image, G::modle1);
			

			//G::m_criticalsection_imagelist1.Lock();
			//imagelink1 = G::Imagelist1.GetHead();              //��Ϊ�գ�ȡ��ͷԪ��
			////����ͼ����
			//G::ImageDispose1(imagelink1.image, G::modle1);
			////ɾ��ͼ��
			//G::Imagelist1.RemoveHead();
			//G::m_criticalsection_imagelist1.Unlock();
			//G::grab1delete++;                      //ͼ��1ɾ����������ͼ��1��������
			////list�ؼ���������
			//str.Format(L"%d", G::grab1counts - 2);
			//pdlg->m_data_list.InsertItem(G::grab1counts - 2, str);
			//str.Format(L"%.3lf mm", G::LeftLength);
			//pdlg->m_data_list.SetItemText(G::grab1counts - 2, 1, str);
			//str.Format(L"%.3lf mm", G::LeftDiameter);
			//pdlg->m_data_list.SetItemText(G::grab1counts - 2, 2, str);
			////���Ҫ������д���ļ�
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

//9ͼ�����߳�2�����Ʒ-����⾶���¿ף��ײ�Բ��ë�̣�
DWORD     ImageDispose2(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[9] = 0;
	ImageLinked   imagelink2;
	CString       str;
	ProductLateral     laterallink;
	while (0 == pdlg->m_exitThread[9])
	{
		//������۵�ͼ���б���Ϊ��
		if ((G::Imagelist2.GetCount() > 0))
		{


			////�ȴ������ͼ�����ΪNG�����Ϊ0������Ҫ�Ƚ��Ҷ�ͼ��
			////ע�⣬���Ƚ��Ҷ�ͼ��ʱ����Ҫ�ǵý��Ҷ�ͼ��ɾ��
			////��ʼ�������ͼ��
			//G::m_criticalsection_imagelist2.Lock();
			//imagelink2 = G::Imagelist2.GetHead();             //��ȡͼ��
			////����ͼ����
			////pdlg->AppearComputeLeft();                              //��۴���1-���
			////ɾ��ͼ��
			//G::Imagelist2.RemoveHead();
			//G::m_criticalsection_imagelist2.Unlock();
			////��֤�����������OK����ʼ����Ҷˣ�����ֱ�ӽ�����и������ҽ��Ҷ�ͼ��ɾ��
			//if (0 == pdlg->LeftOK1)                    //�����˲�ƷNG
			//{
			//	dirtylink.NoDirty = 0;            //ֱ���и�
			//	//ɾ���Ҷ˽ڵ�
			//	G::m_criticalsection_imagelist3.Lock();
			//	G::Imagelist3.RemoveHead();
			//	G::m_criticalsection_imagelist3.Unlock();
			//	//������������б�
			//	G::m_criticalsection_dirtylist1.Lock();
			//	G::Dirtylist1.AddTail(dirtylink);
			//	G::m_criticalsection_dirtylist1.Unlock();
			//	break;                                  //
			//}
			//else if (1 == pdlg->LeftOK1)                   //������λOK
			//{
			//	//��ʼ�ж��Ҷ����
			//	G::m_criticalsection_imagelist3.Lock();
			//	imagelink2 = G::Imagelist3.GetHead();
			//	//
			//	//pdlg->AppearComputeRight();                         //�����Ҷ����
			//	//ɾ���ڵ�
			//	G::Imagelist3.RemoveHead();
			//	G::m_criticalsection_imagelist3.Unlock();

			//	//�����Ҷ˽��
			//	//�Ҷ�ΪOK
			//	if (1 == pdlg->RightOK1)
			//		dirtylink.NoDirty = 1;
			//	else if(0==pdlg->RightOK1)
			//		dirtylink.NoDirty = 0;

			//	//����������
			//	G::m_criticalsection_dirtylist1.Lock();
			//	G::Dirtylist1.AddTail(dirtylink);
			//	G::m_criticalsection_dirtylist1.Unlock();
			//}      //else if ��˲�Ʒ
			
		}////////////////////if ��Ϊ��

		Sleep(5);
	}
	TRACE(L"ImageDispose2 Thread Exit");
	pdlg->m_exitThread[9] = 1;
	return 0;
}

//10ͼ�����߳�3�����Ʒ�Ҷ��⾶��ۣ�     ͼ���б�3
DWORD     ImageDispose3(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[10] = 0;
	ProductLateral  laterallink;
	ImageLinked   imagelink4;
	CString         str;
	int  bResultSave1;
	int    n;                                                   //��¼����
	while (0 == pdlg->m_exitThread[10])
	{
		if (G::Imagelist4.GetCount() > 0)
		{
			//G::m_criticalsection_imagelist4.Lock();
			//imagelink4 = G::Imagelist4.GetHead();
			////G::m_criticalsection_imagelist3.Unlock();
			////ͼ����
			//G::ImageDispose2(imagelink4.image, G::modle1);
			////G::m_criticalsection_imagelist3.Lock();
			////ɾ��ͼ��
			//G::Imagelist4.RemoveHead();
			//G::m_criticalsection_imagelist4.Unlock();
			//G::grab2delete++;
			////list�ؼ���������
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
			//	//	//�������������β
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
		//	//	//ɾ��ͼ��
		//	//	G::Imagelist3backup.RemoveHead();
		//	//	G::grab2delete++;
		//	//	//list�ؼ���������
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
		//	//			//�������������β
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

//11ͼ�����߳�4���Ҳ�Ʒ�ܳ���  
DWORD     ImageDispose4(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[11] = 0;
	ImageLinked   imagelink5;
	CString       str;
	ProductPositive     positivelink;
	while (0 == pdlg->m_exitThread[11])
	{
		//������۵�ͼ���б���Ϊ��
		if ((G::Imagelist5.GetCount() > 0))
		{
			////�ȴ������ͼ�����ΪNG�����Ϊ0������Ҫ�Ƚ��Ҷ�ͼ��
			////ע�⣬���Ƚ��Ҷ�ͼ��ʱ����Ҫ�ǵý��Ҷ�ͼ��ɾ��
			////��ʼ�������ͼ��
			//G::m_criticalsection_imagelist5.Lock();
			//imagelink5 = G::Imagelist5.GetHead();             //��ȡͼ��
			////����ͼ����
			////pdlg->AppearComputeLeft();                              //��۴���1-���
			////ɾ��ͼ��
			//G::Imagelist5.RemoveHead();
			//G::m_criticalsection_imagelist5.Unlock();
			////��֤�����������OK����ʼ����Ҷˣ�����ֱ�ӽ�����и������ҽ��Ҷ�ͼ��ɾ��
			//if (0 == pdlg->LeftOK2)                    //�����˲�ƷNG
			//{
			//	dirtylink.NoDirty = 0;            //ֱ���и�
			//	//ɾ���Ҷ˽ڵ�
			//	G::m_criticalsection_imagelist6.Lock();
			//	G::Imagelist6.RemoveHead();
			//	G::m_criticalsection_imagelist6.Unlock();
			//	//������������б�
			//	G::m_criticalsection_dirtylist2.Lock();
			//	G::Dirtylist2.AddTail(dirtylink);
			//	G::m_criticalsection_dirtylist2.Unlock();

			//	break;                                  //
			//}
			//else if (1 == pdlg->LeftOK2)                   //������λOK
			//{
			//	//��ʼ�ж��Ҷ����
			//	G::m_criticalsection_imagelist6.Lock();
			//	imagelink5 = G::Imagelist6.GetHead();
			//	//
			//	//pdlg->AppearComputeRight();                         //�����Ҷ����
			//	//ɾ���ڵ�
			//	G::Imagelist6.RemoveHead();
			//	G::m_criticalsection_imagelist6.Unlock();

			//	//�����Ҷ˽��
			//	//�Ҷ�ΪOK
			//	if (1 == pdlg->RightOK2)
			//		dirtylink.NoDirty = 1;
			//	else if (0 == pdlg->RightOK2)
			//		dirtylink.NoDirty = 0;

			//	//����������
			//	G::m_criticalsection_dirtylist2.Lock();
			//	G::Dirtylist2.AddTail(dirtylink);
			//	G::m_criticalsection_dirtylist2.Unlock();
			//}      //else if ��˲�Ʒ

		}////////////////////if ��Ϊ��
		Sleep(5);
	}
	TRACE(L"ImageDispose4 Thread Exit");
	pdlg->m_exitThread[11] = 1;
	return 0;
}

//20ͼ�����߳�5(�Ҳ�Ʒ����⾶���)
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

//21ͼ�����߳�6���Ҳ�Ʒ�Ҷ��⾶��ۣ�
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

//12��������߳�1�����Ʒ��
DWORD     Analysis1(LPVOID lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[12] = 0;
	ProductPositive            positivelink;
	ProductLateral             lateralleftlink,lateralrightlink;
	DisposeResult              resultlink;
	//CString                   str;
	//int                           n;                              //����������ѡ�����б�              
	while (0 == pdlg->m_exitThread[12])
	{
		//Ҫͳ�Ƴߴ���Ϣ������Ҫ������ȫ�������ݲ��ܿ�ʼ���з���
		if ((G::LeftPositiveList.GetCount() > 0)&&(G::LeftLateralList1.GetCount()>0)&&(G::LeftLateralList2.GetCount()>0))
		{
			//����Ƚϣ����ٱȽϴ���
			G::m_criticalsection_LeftPositive.Lock();
			positivelink = G::LeftPositiveList.GetHead();
			G::LeftPositiveList.RemoveHead();
			G::m_criticalsection_LeftPositive.Unlock();
			//�ӵ�һ����ʼ��ÿ��ȡһ���Ƚϣ����NG��ֱ��ɾ������ı�ı�ͷ

			//�ȱȽ��ܳ���
			//�ܳ���OK
			if ((G::PR[VP_LeftOKLengthLow].paravalue <= positivelink.TotalLength) && (positivelink.TotalLength <= G::PR[VP_LeftOKLengthHigh].paravalue) && (positivelink.ApperanceOK == AppearanceOK))
			{
				//�Ƚ����˽��
				//ȡ�����ͷ
				G::m_criticalsection_LeftLateral1.Lock();
				lateralleftlink = G::LeftLateralList1.GetHead();
				G::LeftLateralList1.RemoveHead();
				G::m_criticalsection_LeftLateral1.Unlock();
				//�Ҷ˱��ͷ
				G::m_criticalsection_LeftLateral2.Lock();
				lateralrightlink = G::LeftLateralList2.GetHead();
				G::LeftLateralList2.RemoveHead();
				G::m_criticalsection_LeftLateral2.Unlock();
				//��ʼ�Ƚ����˽��
				//���ΪOK
				if ((lateralleftlink.AppearanceOK == AppearanceOK) && (G::PR[VP_LeftOKDiameterLow].paravalue <= lateralleftlink.Diameter) && (lateralleftlink.Diameter <= G::PR[VP_LeftOKDiameterHigh].paravalue) && (AppearanceOK == lateralrightlink.AppearanceOK) && (G::PR[VP_LeftOKDiameterLow].paravalue <= lateralrightlink.Diameter) && (lateralrightlink.Diameter <= G::PR[VP_LeftOKDiameterHigh].paravalue))
				{
					resultlink.OK = 1;
					G::disp_message(G::HW1, "OK", "image", 180, 1600, "green", "false");
				}
				else         //���Ϊng
				{
					resultlink.OK = 0;
					G::disp_message(G::HW1, "NG", "image", 180, 1600, "red", "false");
					G::PR[PI_NGCounts].paravalue++;
				}
				//����������β
				G::m_criticalsection_LeftResultlist.Lock();
				G::LeftResultlist.AddTail(resultlink);
				G::m_criticalsection_LeftResultlist.Unlock();
			}
			//�ܳ���NG��ֱ��ɾ������������ͷ
			else
			{
				G::disp_message(G::HW1, "NG", "image", 180, 1600, "red", "false");
				resultlink.OK = 0;               //���ΪNG
				//����������β
				G::m_criticalsection_LeftResultlist.Lock();
				G::LeftResultlist.AddTail(resultlink);
				G::m_criticalsection_LeftResultlist.Unlock();
				//ɾ����˱�
				G::m_criticalsection_LeftLateral1.Lock();
				G::LeftLateralList1.RemoveHead();
				G::m_criticalsection_LeftLateral1.Unlock();
				//ɾ���Ҷ˱�
				G::m_criticalsection_LeftLateral2.Lock();
				G::LeftLateralList2.RemoveHead();
				G::m_criticalsection_LeftLateral2.Unlock();
			}
			//������һ
			G::PR[PI_TotalCounts].paravalue++;

		}     /////////////////////////if ��Ϊ��
		
		Sleep(5);
	}          //while
	TRACE(L"Analysis1 Thread Exit");
	pdlg->m_exitThread[12] = 1;
	return 0;
}

//13��������߳�2
DWORD     Analysis2(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[13] = 0;
	ProductPositive            positivelink;
	ProductLateral             lateralleftlink, lateralrightlink;
	DisposeResult         resultlink;                       //����б�ڵ㣬���ڲ������б�
	//int                         n;                                   //����������ѡ�����б�
	while (0 == pdlg->m_exitThread[13])
	{
		//Ҫͳ�Ƴߴ���Ϣ������Ҫ������ȫ�������ݲ��ܿ�ʼ���з���
		if ((G::RightPositiveList.GetCount() > 0) && (G::RightLateralList1.GetCount()>0) && (G::RightLateralList2.GetCount()>0))
		{
			//����Ƚϣ����ٱȽϴ���
			G::m_criticalsection_RightPositive.Lock();
			positivelink = G::RightPositiveList.GetHead();
			G::RightPositiveList.RemoveHead();
			G::m_criticalsection_RightPositive.Unlock();
			//�ӵ�һ����ʼ��ÿ��ȡһ���Ƚϣ����NG��ֱ��ɾ������ı�ı�ͷ

			//�ȱȽ��ܳ���
			//�ܳ���OK
			if ((G::PR[VP_RightOKLengthLow].paravalue <= positivelink.TotalLength) && (positivelink.TotalLength <= G::PR[VP_RightOKLengthHigh].paravalue) && (positivelink.ApperanceOK == AppearanceOK))
			{
				//�Ƚ����˽��
				//ȡ�����ͷ
				G::m_criticalsection_RightLateral1.Lock();
				lateralleftlink = G::RightLateralList1.GetHead();
				G::RightLateralList1.RemoveHead();
				G::m_criticalsection_RightLateral1.Unlock();
				//�Ҷ˱��ͷ
				G::m_criticalsection_RightLateral2.Lock();
				lateralrightlink = G::RightLateralList2.GetHead();
				G::RightLateralList2.RemoveHead();
				G::m_criticalsection_RightLateral2.Unlock();
				//��ʼ�Ƚ����˽��
				//���ΪOK
				if ((lateralleftlink.AppearanceOK == AppearanceOK) && (G::PR[VP_RightOKDiameterLow].paravalue <= lateralleftlink.Diameter) && (lateralleftlink.Diameter <= G::PR[VP_RightOKDiameterHigh].paravalue) && (AppearanceOK == lateralrightlink.AppearanceOK) && (G::PR[VP_RightOKDiameterLow].paravalue <= lateralrightlink.Diameter) && (lateralrightlink.Diameter <= G::PR[VP_RightOKDiameterHigh].paravalue))
				{
					resultlink.OK = 1;
					G::disp_message(G::HW4, "OK", "image", 180, 1600, "green", "false");
				}
				else         //���Ϊng
				{
					resultlink.OK = 0;
					G::disp_message(G::HW4, "NG", "image", 180, 1600, "red", "false");
					G::PR[PI_NGCounts].paravalue++;
				}
				//����������β
				G::m_criticalsection_RightResultlist.Lock();
				G::RightResultlist.AddTail(resultlink);
				G::m_criticalsection_RightResultlist.Unlock();
			}
			//�ܳ���NG��ֱ��ɾ������������ͷ
			else
			{
				G::disp_message(G::HW3, "NG", "image", 180, 1600, "red", "false");
				resultlink.OK = 0;               //���ΪNG
				//����������β
				G::m_criticalsection_RightResultlist.Lock();
				G::RightResultlist.AddTail(resultlink);
				G::m_criticalsection_RightResultlist.Unlock();
				//ɾ����˱�
				G::m_criticalsection_RightLateral1.Lock();
				G::RightLateralList1.RemoveHead();
				G::m_criticalsection_RightLateral1.Unlock();
				//ɾ���Ҷ˱�
				G::m_criticalsection_RightLateral2.Lock();
				G::RightLateralList2.RemoveHead();
				G::m_criticalsection_RightLateral2.Unlock();
			}
			//������һ
			G::PR[PI_TotalCounts].paravalue++;

		}     /////////////////////////if ��Ϊ��

		//if ((G::Sizelist2.GetCount() > 0)&&(G::Dirtylist2.GetCount()>0))
		//{
		//	G::m_criticalsection_sizelist2.Lock();
		//	sizelink = G::Sizelist2.GetHead();
		//	G::Sizelist2.RemoveHead();
		//	G::m_criticalsection_sizelist2.Unlock();
		//	///ȡ��۽��
		//	G::m_criticalsection_dirtylist2.Lock();
		//	dirtylink = G::Dirtylist2.GetHead();
		//	G::Dirtylist2.RemoveHead();
		//	G::m_criticalsection_dirtylist2.Unlock();
		//	//��ʾ����
		//	G::disp_message(G::HW4, ("�⾶:" + (HTuple)G::RightDiameter) + " mm", "image", 300, 100, "green", "false");
		//	G::disp_message(G::HW4, ("�ܳ�:" + (HTuple)G::RightLength) + " mm", "image", 450, 100, "green", "false");
		//	//��������
		//	if ((sizelink.Diameter >= G::PR[124].paravalue) && (sizelink.Diameter <= G::PR[125].paravalue) && (sizelink.TotalLength >= G::PR[122].paravalue) && (sizelink.TotalLength <= G::PR[123].paravalue))
		//	{
		//		resultlink.OK = 1;
		//		G::disp_message(G::HW4, "NG", "image", 200, 1600, "green", "false");
		//	}
		//	else
		//	{
		//		resultlink.OK = 0;
		//		G::disp_message(G::HW4, "NG", "image", 200, 1600, "red", "false");
		//		G::PR[PI_NGCounts].paravalue++;                                          //ng����һ
		//	}
		//	//������һ
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

//14λ��ˢ�´����߳�1
DWORD    Blow1(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[14] = 0;
	DisposeResult         resultlink1;
	CString   str;
	long ss;               //��ǰ��λ��
	//int    n;               //����������ѡ�����б�
	while (0==pdlg->m_exitThread[14])
	{
		if ((TRUE == G::bRunning) && (1==G::bLeftNoShield))
		{
			//n = G::LeftResultCounts % 2;
			//�Ƚϵ�ǰλ��
			ss = dmc_get_position(0, AXIS_LEFT);
			if (((ss - G::PR[MP_LeftNGBlowPos].paravalue) >= (G::PR[MP_LeftCamIntervalDis].paravalue*G::blow1 - 0.03*G::PR[MP_LeftRunSpeed].paravalue)) && ((ss - G::PR[MP_LeftNGBlowPos].paravalue) <= (G::PR[MP_LeftCamIntervalDis].paravalue*G::blow1 + 0.03*G::PR[MP_LeftRunSpeed].paravalue)))
			{
				//������һ
				G::blow1++;
				//ȡ������Ƚ��Ƿ���
				if (G::LeftResultlist.GetCount() > 0)
				{
					G::m_criticalsection_LeftResultlist.Lock();
					resultlink1 = G::LeftResultlist.GetHead();
					//ɾ��ͷ�ڵ�
					G::LeftResultlist.RemoveHead();
					G::m_criticalsection_LeftResultlist.Unlock();
					//pdlg->m_criticalsection_blow1.Unlock();
					if (1 != resultlink1.OK)
					{
						//NG��ŷ�����0.1S
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
				//		//NG��ŷ�����0.3S
				//		G::MCard.SetDoBit(LEFTRADIOTUBENG, 1);
				//		Sleep(5);
				//		G::MCard.SetDoBit(LEFTRADIOTUBENG, 0);
				//	}
				//	//ɾ��ͷ�ڵ�
				//	G::DisposeResultlist1.RemoveHead();
				//}
				//else if ((1 == n) && (G::DisposeResult1backup.GetCount() > 0))
				//{
				//	resultlink1 = G::DisposeResult1backup.GetHead();
				//	if (0 == resultlink1.OK)
				//	{
				//		//NG��ŷ�����0.3S
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

//15λ��ˢ�´����߳�2
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
			//��ȡ��ǰ��λ��
			pos = dmc_get_position(0, AXIS_RIGHT);
			//�ӽ�NGλ��ʱ��ʼ��¼
			if (((pos - G::PR[9].paravalue) >= (G::PR[3].paravalue*G::blow2 - 0.03*G::PR[1].paravalue)) && ((pos - G::PR[9].paravalue) <= (G::PR[3].paravalue*G::blow2 + 0.03*G::PR[1].paravalue)))
			{
				G::blow2++;
				//ȡ��һ�ڵ�
				if (G::RightResultlist.GetCount() > 0)
				{
					G::m_criticalsection_RightResultlist.Lock();
					resultlink2 = G::RightResultlist.GetHead();
					//ɾ���ڵ�
					G::RightResultlist.RemoveHead();
					G::m_criticalsection_RightResultlist.Unlock();
					//���NG
					if (1 != resultlink2.OK)
					{
						//NG��ŷ�����0.1S
						G::MCard.SetDoBitReverse(RIGHTRADIOTUBENG, 1, 10);
						//G::MCard.SetDoBit(RIGHTRADIOTUBENG, 1);
						//Sleep(5);
						//G::MCard.SetDoBit(RIGHTRADIOTUBENG, 0);
					}
				}

				//if ((0 == n) && (G::DisposeResultlist2.GetCount() > 0))
				//{
				//	resultlink2 = G::DisposeResultlist2.GetHead();
				//	//���NG
				//	if (0 == resultlink2.OK)
				//	{
				//		//NG��ŷ�����0.08S
				//		G::MCard.SetDoBit(RIGHTRADIOTUBENG, 1);
				//		Sleep(5);
				//		G::MCard.SetDoBit(RIGHTRADIOTUBENG, 0);
				//	}
				//	//ɾ���ڵ�
				//	G::DisposeResultlist2.RemoveHead();
				//}
				////ȡ����ڵ�
				//if ((1 == n) && (G::DisposeResult2backup.GetCount() > 0))
				//{
				//	resultlink2 = G::DisposeResult2backup.GetHead();
				//	//���NG
				//	if (0 == resultlink2.OK)
				//	{
				//		//NG��ŷ�����0.08S
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

			////��ȡ��ǰ��λ��
			//pos = dmc_get_position(0, 0);
			////�ӽ�NGλ��ʱ��ʼ��¼
			//if (((pos - G::PR[9].paravalue) >= (G::PR[3].paravalue*G::blow2 - 0.05*G::PR[1].paravalue)) && ((pos - G::PR[9].paravalue) <= (G::PR[3].paravalue*G::blow2 + 0.05*G::PR[1].paravalue)))
			//{
			//	pdlg->m_message_listbox.AddString(L"blow2pos");

			//	//���NG
			//	if (0 == resultlink2.OK)
			//	{
			//		//NG��ŷ�����0.08S
			//		G::MCard.SetDoBit(RIGHTRADIOTUBENG, 1);
			//		Sleep(5);
			//		G::MCard.SetDoBit(RIGHTRADIOTUBENG, 0);
			//	}
			//	G::blow2++;
			//	//ɾ��ͷ�ڵ�
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

//16ˢ�°忨״̬
DWORD   RefreshCardState(LPVOID  lpparameter)
{
	CMy1269AirColumnDlg *pdlg = (CMy1269AirColumnDlg*)lpparameter;
	pdlg->m_exitThread[16] = 0;
	int    dwt = 0;
	int    nTimes=0;
	while (0==pdlg->m_exitThread[16])
	{
		///////////////////////////////////////////������////////////////////////////////////////////////////////////
		G::MCard.GetAllDi();
		G::MCard.GetAllDo();
		//////////////////////////////////////////��ͣ����////////////////////////////////////////////////////////////
		if (G::MCard.I[SCRAM_BUTTON].sValue == 0)
		{
			if (nTimes == 0)
			{
				pdlg->MessageBox(L"��ͣ����...",L"����",MB_ICONWARNING);
				nTimes++;
			}
		}
		else
			nTimes = 0;
		//Sleep(5);

		/////////////////////////////////////////ˢ�°忨������״̬
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
		////////////////////////////////////���˶���ز�����PR����MCard
		try
		{
			G::MCard.A[AXIS_LEFT].AP.fHomeVel=G::PR[MP_LeftRstSpeed].paravalue;     //��λ�ٶ�
			//�Զ������ٶ�
			G::MCard.A[AXIS_LEFT].AP.fVel=G::PR[MP_LeftRunSpeed].paravalue;    
			//���Ḵλ�ٶ�
			G::MCard.A[AXIS_RIGHT].AP.fHomeVel = G::PR[MP_RightRstSpeed].paravalue;
			//�����Զ��ٶ�
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

////19�ߴ�����д�����ݿ��߳�
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
//		//��1
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
//			//��ĩβ�����¼�¼
//			G::db.m_pRecordset->AddNew();
//			//
//			G::db.m_pRecordset->PutCollect(L"����", _variant_t(strRptTime));
//			G::db.m_pRecordset->PutCollect(L"ʱ��", _variant_t(strtime));
//			G::db.m_pRecordset->PutCollect(L"���Ʒ�⾶", _variant_t(G::double_string(sizelink1.Diameter)));
//			G::db.m_pRecordset->PutCollect(L"���Ʒ�ܳ�", _variant_t(G::double_string(sizelink1.TotalLength)));
//			G::db.m_pRecordset->PutCollect(L"�Ҳ�Ʒ�⾶", _variant_t(G::double_string(sizelink2.Diameter)));
//			G::db.m_pRecordset->PutCollect(L"�Ҳ�Ʒ�ܳ�", _variant_t(G::double_string(sizelink2.TotalLength)));
//
//			G::db.m_pRecordset->Update();       //�������ݿ⣬�����½���������ݿ�
//
//
//		}
//		catch (_com_error c)
//		{
//			CString  err;
//			err = c.ErrorMessage();
//			AfxMessageBox(err + L"ʧ��");
//		}
//
//		////��Ӧ�����������ݾ�д
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
//		//		//��ĩβ�����¼�¼
//		//		G::db.m_pRecordset->AddNew();
//		//		//
//		//		G::db.m_pRecordset->PutCollect(L"����", _variant_t(strRptTime));
//		//		G::db.m_pRecordset->PutCollect(L"ʱ��", _variant_t(strtime));
//		//		G::db.m_pRecordset->PutCollect(L"���Ʒ�⾶", _variant_t(G::double_string(sizelink1.Diameter)));
//		//		G::db.m_pRecordset->PutCollect(L"���Ʒ�ܳ�", _variant_t(G::double_string(sizelink1.TotalLength)));
//		//		G::db.m_pRecordset->PutCollect(L"�Ҳ�Ʒ�⾶", _variant_t(G::double_string(sizelink2.Diameter)));
//		//		G::db.m_pRecordset->PutCollect(L"�Ҳ�Ʒ�ܳ�", _variant_t(G::double_string(sizelink2.TotalLength)));
//
//		//		G::db.m_pRecordset->Update();       //�������ݿ⣬�����½���������ݿ�
//
//		//		//
//		//		G::SizelistToFile1.RemoveHead();
//		//		G::SizelistToFile2.RemoveHead();
//		//	}
//		//	catch (_com_error c)
//		//	{
//		//		CString  err;
//		//		err = c.ErrorMessage();
//		//		AfxMessageBox(err + L"ʧ��");
//		//	}
//		//}	
//		Sleep(5);
//	}
//	TRACE(L"ProductionToDB Thread Exit");
//	pdlg->m_exitThread[19] = 1;
//	return 0;
//}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMy1269AirColumnDlg �Ի���



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


// CMy1269AirColumnDlg ��Ϣ�������

BOOL CMy1269AirColumnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	/*CMenu menu;
	menu.LoadMenuW(IDR_MENU1);
	SetMenu(&menu);*/

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	SkinH_Attach();
	AfxOleInit();              //��ʼ��com���
	//test=false;


	G::bOpenExcelOk = 0;

	G::CreateDatabase(L"AirColumn DB");

	G::CreateTable(ParaTableName);

	if (MCARD_ERROR == G::MCard.OpenMotionCard((CWnd*)this))
	{
		MessageBox(L"�˶����ƿ��޷��򿪣������˶����ƿ�PCI�����Ƿ��ã��Լ��Ƿ���ȷ��װ������", L"����", MB_ICONWARNING);
	}
	if (MCARD_ERROR == G::MCard.SetAxisParam(AXIS_LEFT))
	{
		MessageBox(L"����ŷ����������ô���", L"����", MB_ICONERROR);
	}
	if (MCARD_ERROR == G::MCard.SetAxisParam(AXIS_RIGHT))
	{
		MessageBox(L"�ұ��ŷ����������ô���", L"����", MB_ICONERROR);
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

	//��ȡģ��
	try
	{
		ReadShapeModel("./���������1.shm", &G::modle1);
	}
	catch (...)
	{
		AfxMessageBox(L"��ģ��ʧ��");
		return -1;
	}

	//��ʼ������
	G::InitParam();
	//��ʼ������
	G::ClearLinked();
	//��ȡ����
	ReadPara();
	ReadIO();
	//�����
	G::OpenCamera();

	m_crWhite = RGB(255, 255, 255);
	m_hrWhite = CreateSolidBrush(m_crWhite);
	font1.CreatePointFont(360, L"�����п�");
	this->GetDlgItem(IDC_TITLE_STATIC)->SetFont(&font1);

	//��ͼ�������
	imageno1 = 0;
	imageno2 = 0;
	imageno3 = 0;
	imageno4 = 0;
	imageno5 = 0; 
	imageno6 = 0;

	//�������������ʼ��
	G::GridCounts1=0;
	G::GridCounts2=0;

	G::grab1counts=0;                          //ͼ��1ץͼ����
	G::grab2counts=0;                          //ͼ��2ץͼ����
	G::grab1delete=0;                           //ͼ��1ɾ������
	G::grab2delete=0;                           //ͼ��2ɾ������
	G::triggercounts1=0;                       
	G::triggercounts2=0;
	G::blow1 = 0;                                  
	G::blow2 = 0;
	//G::LeftResultCounts = 0;                    //���Ʒ�������
	//G::RightResultCounts = 0;                  //�Ҳ�Ʒ�������
	//G::LeftDisposecounts1=0;                   ///��������봦�����б���
	//G::RightDisposecounts1=0;                 //��������봦����������
	//G::ToFileCounts1 = 0;                        //���д�����ݿ����1
	//G::ToFileCounts2 = 0;                        //���д�����ݿ����2

	//������ۼ������ʼ��
	LeftOK1 = LeftOK2  = RightOK1 = RightOK2  = -1;

	//���ô���
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

	
	//ץȡͼ���߳�
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
	//6���˶��߳�
	m_hThread[6] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AxisMove, this, 0, 0);
	CloseHandle(m_hThread[6]);
	//7 ��ȡλ�ô�������߳�
	m_hThread[7] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CameraTrigger, this, 0, 0);
	CloseHandle(m_hThread[7]);
	//8,9,10,11ͼ�����߳�
	m_hThread[8] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ImageDispose1, this, 0, 0);
	CloseHandle(m_hThread[8]);
	m_hThread[9] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ImageDispose2, this, 0, 0);
	CloseHandle(m_hThread[9]);
	m_hThread[10] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ImageDispose3, this, 0, 0);
	CloseHandle(m_hThread[10]);
	m_hThread[11] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ImageDispose4, this, 0, 0);
	CloseHandle(m_hThread[11]);
	///��������߳�
	m_hThread[12] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Analysis1, this, 0, 0);
	CloseHandle(m_hThread[12]);
	m_hThread[13] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Analysis2, this, 0, 0);
	CloseHandle(m_hThread[13]);
	//�����߳�
	m_hThread[14] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Blow1, this, 0, 0);
	CloseHandle(m_hThread[14]);
	m_hThread[15] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Blow2, this, 0, 0);
	CloseHandle(m_hThread[15]);
	//ˢ�°忨�����״̬�߳�
	m_hThread[16] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RefreshCardState, this, 0, 0);
	CloseHandle(m_hThread[16]);
	//�ߴ�����д�����ݿ��߳�
	/*m_hThread[19] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ProductionToDB, this, 0, 0);*/
	CloseHandle(m_hThread[19]);

	//SkinH_DetachEx(GetDlgItem(IDC_LEFTRESULT_STATIC)->m_hWnd);
	//SkinH_DetachEx(GetDlgItem(IDC_RIGHTRESULT_STATIC)->m_hWnd);
	
	/////////////////////////////////////////////////////////excel/////////////
	CString  filepath1 = L"D:\\�������������\\�ߴ�����";         //
	CTime  time = CTime::GetCurrentTime();
	CString strRptTime = time.Format("%Y%m%d");
	excelpath = filepath1 + strRptTime + L".xlsx";
	//if (0 == filerw.OpenExcel(excelpath))
	//	G::bOpenExcelOk = 1;
	//�б�ؼ���ʼ��
	CRect    rectlist1,rectlist2;
	GetDlgItem(IDC_DATA_LIST)->GetClientRect(&rectlist1);
	m_data_list.InsertColumn(0, L"���ƷNO", LVCFMT_CENTER, rectlist1.Width() * 2 / 11, 0);
	m_data_list.InsertColumn(1, L"�ܳ�", LVCFMT_CENTER, rectlist1.Width() *3/ 11, 1);
	m_data_list.InsertColumn(2, L"����⾶", LVCFMT_CENTER, rectlist1.Width() *3/ 11, 2);
	m_data_list.InsertColumn(3, L"�Ҳ��⾶", LVCFMT_CENTER, rectlist1.Width() *3/ 11, 3);
	GetDlgItem(IDC_DATA_LIST2)->GetClientRect(&rectlist2);
	m_datalist2.InsertColumn(0, L"�Ҳ�ƷNO", LVCFMT_CENTER, rectlist2.Width() * 2 / 11, 0);
	m_datalist2.InsertColumn(1, L"�ܳ�", LVCFMT_CENTER, rectlist2.Width() * 3 / 11, 1);
	m_datalist2.InsertColumn(2, L"����⾶", LVCFMT_CENTER, rectlist2.Width() * 3 / 11, 2);
	m_datalist2.InsertColumn(3, L"�Ҳ��⾶", LVCFMT_CENTER, rectlist2.Width() * 3 / 11, 3);

	SetTimer(1, 300, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy1269AirColumnDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy1269AirColumnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy1269AirColumnDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ExitSoft();
}


// �˳����
void CMy1269AirColumnDlg::ExitSoft()
{
	if((0==G::MCard.A[AXIS_LEFT].AS.bDone)||(0==G::MCard.A[AXIS_LEFT].AS.bDone))
	{
		AfxMessageBox(L"���˶���,����ֹͣ���˶�");
		return;
	}
	if (MessageBox(L"ȷ���˳������", L"��ʾ", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		//HalconCpp::ClearShapeModel(G::modle1);
		//filerw.release();
		//Sleep(300);
		////�رհ忨
		//G::MCard.CloseMotionCard();
		//Sleep(100);

		CDialogEx::OnCancel();
	}
}

//�˵��˳���ť
void CMy1269AirColumnDlg::OnExitMenu()
{
	// TODO:  �ڴ���������������
	ExitSoft();
}

//�˵��������ð�ť
void CMy1269AirColumnDlg::OnParasetMenu()
{
	// TODO:  �ڴ���������������
	CPSWDlg  pswdlg;
	pswdlg.DoModal();
}

//�˵�IO���ð�ť
void CMy1269AirColumnDlg::OnIoset()
{
	// TODO:  �ڴ���������������
	//������ģ̬�Ի���
	if (NULL == ioset)
	{
		ioset = new CIOSETDlg();
		ioset->Create(IDD_IOSET_DIALOG, this);
	}
	//��ʾ��ģ̬�Ի���
	ioset->ShowWindow(SW_SHOW);
}


void CMy1269AirColumnDlg::OnMotionMenu()
{
	// TODO:  �ڴ���������������
	CMotionDlg  motiondlg;
	motiondlg.DoModal();
}



// ��ȡ���� ���ν������ʹ��
int CMy1269AirColumnDlg::ReadPara()
{
	///////////////////////////////////ͨ�ò���
	//�򿪼�¼��
	try
	{
		//ע�⣺���� Open �����򿪼�¼��ʱʹ���� adOpenStatic �αꣻ���ʹ�� adOpenDynamic �α꣬GetRecordCount ���������� -1 ��
		G::db.m_pRecordset->Open("SELECT  *  FROM " + (_bstr_t)ParaTableName, G::db.m_pConnection.GetInterfacePtr(), adOpenStatic, adLockPessimistic, adCmdText);
	}
	catch (_com_error a)
	{
		CString  err;
		err = a.ErrorMessage();
		AfxMessageBox(err + L"ʧ��,���������");
	}
	int  i = G::db.m_pRecordset->GetRecordCount();
	if (i < 2)           //����û������
	{
		G::WriteCommonPara();         //��Ĭ�ϲ���д��
		return 0;
	}
	//�ǿգ����ȡ����
	/////////////////////////////////ͨ�ò���////////////////////
	try
	{
		G::db.m_pRecordset->MoveFirst();
		for (int i = 0; i < 150; i++)
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
				return 0;
			}
		}             //for
	}
	catch (_com_error a)
	{
		CString  err;
		err = a.ErrorMessage();
		AfxMessageBox(err + L"ʧ��,���������1");
	}

	

	//CFileFind finder;
	//CString    str;
	//CString    str2 = L"������";                            //������
	//CString    str3 = L"����ֵ";                            //����ֵ
	//CString    str4 = L"�������ֵ";                     //�������ֵ
	//CString    str5 = L"������Сֵ";                    //������Сֵ
	//CString    str6 = L"��ע";                             //��ע
	////�鿴�Ƿ���ڴ��ļ�
	//BOOL ifinder = finder.FindFile(G::ParaFileName/*L".//1269 Air Column para.ini"*/);
	////�ļ�Ϊ����Ĭ�ϲ���
	//if (!ifinder)                
	//{
	//	return -1;
	//}
	////�ļ����ھͶ�ȡ�ļ��в���
	//else                        
	//{
	//	try                //���������
	//	{
	//		//����˿���
	//		for (int i = 0; i < 16; i++)
	//		{
	//			str.Format(L"%d�������", i);
	//			::GetPrivateProfileString(L"InputPort", str, L"����Ĭ����", G::MCard.I[i].lpwstrPortName.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//		}
	//		//����˿���
	//		for (int i = 0; i < 16; i++)
	//		{
	//			str.Format(L"%d�������", i);
	//			::GetPrivateProfileString(L"OutPort", str, L"���Ĭ����", G::MCard.O[i].lpwstrPortName.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//		}

	//		///////////////////������ֵ
	//		//�˶�����              0��29
	//		for (int i = 0; i < 30; i++)
	//		{
	//			int j = i;
	//			str2.Format(L"������%d", i);
	//			GetPrivateProfileString(L"�˶�����", str2, L"Ĭ��", G::PR[j].paraname.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//			str3.Format(L"����ֵ%d", i);
	//			G::PR[j].paravalue = ::GetPrivateProfileInt(L"�˶�����", str3, 100, G::ParaFileName);
	//			str4.Format(L"���ֵ%d", i);
	//			G::PR[j].paraMaxValue = ::GetPrivateProfileInt(L"�˶�����", str4, 60000, G::ParaFileName);
	//			str5.Format(L"��Сֵ%d", i);
	//			G::PR[j].paraMinValue = ::GetPrivateProfileInt(L"�˶�����", str5, 0, G::ParaFileName);
	//			str6.Format(L"��ע%d", i);
	//			GetPrivateProfileString(L"�˶�����", str6, L"Ĭ��", G::PR[j].remark.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//		}
	//		//�������             30��89
	//		for (int i = 0; i < 60; i++)
	//		{
	//			int j = i+30;
	//			str2.Format(L"������%d", i);
	//			GetPrivateProfileString(L"�������", str2, L"Ĭ��", G::PR[j].paraname.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//			str3.Format(L"����ֵ%d", i);
	//			G::PR[j].paravalue = ::GetPrivateProfileInt(L"�������", str3, 100, G::ParaFileName);
	//			str4.Format(L"���ֵ%d", i);
	//			G::PR[j].paraMaxValue = ::GetPrivateProfileInt(L"�������", str4, 60000, G::ParaFileName);
	//			str5.Format(L"��Сֵ%d", i);
	//			G::PR[j].paraMinValue = ::GetPrivateProfileInt(L"�������", str5, 0, G::ParaFileName);
	//			str6.Format(L"��ע%d", i);
	//			GetPrivateProfileString(L"�������", str6, L"Ĭ��", G::PR[j].remark.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//		}
	//		//�Ӿ�����
	//		for (int i = 0; i < 60; i++)
	//		{
	//			int j = i + 90;
	//			str2.Format(L"������%d", i);
	//			GetPrivateProfileString(L"�Ӿ�����", str2, L"Ĭ��", G::PR[j].paraname.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//			str3.Format(L"����ֵ%d", i);
	//			G::PR[j].paravalue = ::GetPrivateProfileInt(L"�Ӿ�����", str3, 100, G::ParaFileName);
	//			str4.Format(L"���ֵ%d", i);
	//			G::PR[j].paraMaxValue = ::GetPrivateProfileInt(L"�Ӿ�����", str4, 60000, G::ParaFileName);
	//			str5.Format(L"��Сֵ%d", i);
	//			G::PR[j].paraMinValue = ::GetPrivateProfileInt(L"�Ӿ�����", str5, 0, G::ParaFileName);
	//			str6.Format(L"��ע%d", i);
	//			GetPrivateProfileString(L"�Ӿ�����", str6, L"Ĭ��", G::PR[j].remark.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
	//		}
	//		//��������
	//		for (int i = 0; i < 10; i++)
	//		{
	//			int  j = i + 150;
	//			G::PR[j].paravalue = ::GetPrivateProfileInt(L"��������", G::PR[j].paraname, 0, G::ParaFileName);
	//		}
	//	}
	//	catch (...)
	//	{
	//		AfxMessageBox(L"��ȡ����ʧ�ܣ�");
	//		return  -1;
	//	}

	//}

	return 0;
}


// ������д���ļ�
int CMy1269AirColumnDlg::WritePara()
{
	CString  str;

	G::WriteIOPara();
	G::WriteCommonPara();
	G::WriteProduction();

	return 0;
}

//��λ��ť
void CMy1269AirColumnDlg::OnBnClickedRstButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ReadPara();
	CString   grabitemstr;
	GetDlgItemText(IDC_REALTIME_BUTTON, grabitemstr);
	if (G::MCard.I[SCRAM_BUTTON].sValue == 0)
	{
		AfxMessageBox(L"��ͣ�Ѱ��£�");
		Sleep(10);
		return;
	}
	//ʵʱ��
	if (L"�ر�ʵʱ" == grabitemstr)
	{
		AfxMessageBox(L"ʵʱ�����У����ܸ�λ�����ȹر�ʵʱ!");
		return;
	}
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"���������У����ܸ�λ��");
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

	// TODO:  �ڴ˸��� DC ���κ�����
	int   ID = pWnd->GetDlgCtrlID();

	//if (ID = IDC_LEFTRESULT_STATIC)
	//	pDC->SetTextColor(RGB(100, 255, 100));               //��ɫ����
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//ʵʱץȡͼ��
void CMy1269AirColumnDlg::OnBnClickedRealtimeButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString   itemstr;
	if (G::MCard.I[SCRAM_BUTTON].sValue == 0)
	{
		AfxMessageBox(L"��ͣ�Ѱ��£�");
		Sleep(10);
		return;
	}
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"�����У����ܿ���ʵʱ��");
		return;
	}
	GetDlgItemText(IDC_REALTIME_BUTTON, itemstr);
	//G::bCanGrab=1;
	//��ǰ��ʵʱ״̬
	if (L"ʵʱ" == itemstr)
	{
		//����ʵʱץȡ�߳�
		SetDlgItemText(IDC_REALTIME_BUTTON, L"�ر�ʵʱ");
		m_hThread[5] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RealTimeGrab, this, 0, 0);
		CloseHandle(m_hThread[5]);
	}
	//��ǰʵʱ״̬
	else
	{
		SetDlgItemText(IDC_REALTIME_BUTTON, L"ʵʱ");
		//�˳�ʵʱ�߳�
		m_exitThread[5] = 1;

		//try
		//{
		//	//�������Ϊ��������
		//SetFramegrabberParam(G::hv_AcqHandle1, "TriggerSelector", "AcquisitionStart");
		//SetFramegrabberParam(G::hv_AcqHandle1, "TriggerMode", "On");

		///*SetFramegrabberParam(G::hv_AcqHandle2, "TriggerSelector", "AcquisitionStart");
		//SetFramegrabberParam(G::hv_AcqHandle2, "TriggerMode", "On");
		//SetFramegrabberParam(G::hv_AcqHandle3, "TriggerSelector", "AcquisitionStart");
		//SetFramegrabberParam(G::hv_AcqHandle3, "TriggerMode", "On");
		//SetFramegrabberParam(G::hv_AcqHandle4, "TriggerSelector", "AcquisitionStart");
		//SetFramegrabberParam(G::hv_AcqHandle4, "TriggerMode", "On");*/
		//SetDlgItemText(IDC_REALTIME_BUTTON, L"ʵʱ");
		//}
		//catch(...)
		//{
		//	AfxMessageBox(L"�ر�ʵʱʧ��!");
		//}
		//
	}
}

//ץͼ��ť
void CMy1269AirColumnDlg::OnBnClickedGrabButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString   grabitemstr;
	GetDlgItemText(IDC_REALTIME_BUTTON, grabitemstr);
	if (L"�ر�ʵʱ" == grabitemstr)
	{
		AfxMessageBox(L"ʵʱ�����У�����ץͼ�����ȹر�ʵʱ!");
		return;
	}
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"���й����в�����ץͼ!");
		return;
	}
	//G::bCanGrab=1;
	G::CameraTrigger(1);
	G::CameraTrigger(2);
	//G::CameraTrigger(3);
	//G::CameraTrigger(4);
}

//������ť         
void CMy1269AirColumnDlg::OnBnClickedStartButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Start();
}


// ��ʼ����        (��������ͣ������)
int CMy1269AirColumnDlg::Start()
{
	CString   strstart, strrealtime;
	GetDlgItemText(IDC_START_BUTTON, strstart);
	GetDlgItemText(IDC_REALTIME_BUTTON, strrealtime);
	//ʵʱ������
	if (L"�ر�ʵʱ" == strrealtime)
	{
		AfxMessageBox(L"ʵʱ�����У�����������");
		return -1;
	}
	if (0 == G::bResetOK)
	{
		AfxMessageBox(L"���ȸ�λ��");
		return -1;
	}
	//δ����״̬��  
	if (L"����" == strstart || L"����" == strstart)
	{
		//�״�����
		if (L"����" == strstart)
		{
			if ((0 == G::bLeftNoShield) && (0 == G::bRightNoShield))
			{
				//G::bRunning = 0;
				MessageBox(L"����ȫ�����Σ�����ִͣ�У�����������һ���ٿ�ʼִ�У�", L"��ʾ", MB_OK);
				return 0;
			}
			GetDlgItem(IDC_RST_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_REALTIME_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_GRAB_BUTTON)->EnableWindow(FALSE);
		}
		//���¶�ȡ����
		//ReadPara();
		
		//dmc_set_profile(0,0,1000,G::PR[1].paravalue,0.01,0.01,1000);
		G::bRunflag = 1;
		G::bCanGrab = 1;
		G::bRunning = 1;
		SetDlgItemText(IDC_START_BUTTON, L"��ͣ");
	}
	//������
	else
	{
		//������ֹͣ������ֹͣץͼ
		G::MCard.StopAxis(0, 1);
		//�ӳ�һ��ʱ���ٸ�λ�����б�־
		G::bRunning = 0;
		/*Sleep(200);
		G::bCanGrab = 0;*/
		SetDlgItemText(IDC_START_BUTTON, L"����");
	}

	return 0;
}


void CMy1269AirColumnDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if (1==G::bOpenExcelOk)
	//	WriteCheckDataToExcel();

	//CString  filepath1 = L"D:\\�������������\\�ߴ�����";         //
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

//���������Ϣ �˵���ť
void CMy1269AirColumnDlg::OnClearProduct()
{
	// TODO:  �ڴ���������������
	if (IDOK == MessageBox(L"ȷ�����������¼��", L"��ʾ", MB_OKCANCEL))
	{
		G::PR[110].paravalue = 0;
		G::PR[111].paravalue = 0;
		G::PR[112].paravalue = 0;
		G::PR[113].paravalue = 0;
		//G::PR[114].paravalue = 0;
		//G::PR[115].paravalue = 0;
	}

}

//ֹͣ��ť
void CMy1269AirColumnDlg::OnBnClickedStopButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	G::MCard.StopAxis(AXIS_LEFT, 1);
	G::MCard.StopAxis(AXIS_RIGHT, 1);
	/*if(0==G::bRunning)
		G::MCard.StopAxis(0,1);*/
	//�����
	G::ClearLinked();         
	//��ֵ���
	//�������������ʼ��
	G::GridCounts1 = 0;
	G::GridCounts2 = 0;

	G::grab1counts = 0;                          //ͼ��1ץͼ����
	G::grab2counts = 0;                          //ͼ��2ץͼ����
	G::grab1delete = 0;                           //ͼ��1ɾ������
	G::grab2delete = 0;                           //ͼ��2ɾ������
	G::triggercounts1 = 0;
	G::triggercounts2 = 0;
	G::blow1 = 0;
	G::blow2 = 0;
	//��־��λ
	G::bCanGrab = 0;
	G::bResetOK = 0;
	G::bRunning = 0;
	G::bRunflag = 0;
	SetDlgItemText(IDC_START_BUTTON, L"����");
	//�ָ���ťʹ��
	GetDlgItem(IDC_RST_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_REALTIME_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_GRAB_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(TRUE);
}


// ���������д��Excel
void CMy1269AirColumnDlg::WriteCheckDataToExcel()
{
	///////////////////////////////////////////���ߴ�����д���ļ�
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
	////CString  excelpath1 = L"D:\\�������������\\aaa.xlsx";

	////��Ӧ�����������ݾ�д
	//if (G::SizelistToFile1.GetCount() >= 1 && G::SizelistToFile2.GetCount() >= 1)
	//{
	//	filerw.InsertOneItem1269(excelpath);
	//}


	//CString  excelpath;                       //д���ļ�·����
	//CString  filepath1 = L"D:\\�������������\\�ߴ�����";         //
	//CTime  time = CTime::GetCurrentTime();
	//CString strRptTime = time.Format("%Y%m%d");
	//excelpath = filepath1 + strRptTime + L".xlsx";
	//int savedata = ((CButton*)GetDlgItem(IDC_DATASAVE_CHECK))->GetCheck();
	//if (savedata)             ////ѡ�񽫼�����ݱ���
	//{
	//	if (G::SizelistToFile1.GetCount() >= SaveResultMin&&G::SizelistToFile1.GetCount() >= SaveResultMin)
	//	{
	//		filerw.ResultLinkedToExcel(1, excelpath, 1, 1);
	//	}
	//}
}


// ��������״̬��Ϣ����ɫ�ƣ��ؼ����ݣ�������
void CMy1269AirColumnDlg::RefreshStatus()
{
	CString   str;
	CString   str1;
	//CheckSize   checksize;

	///////////////////////////////////////////////////������������
	G::PR[111].paravalue = G::PR[110].paravalue - G::PR[112].paravalue;           //OK����
	if (G::PR[110].paravalue != 0)
		G::PR[113].paravalue = 100 * G::PR[111].paravalue / G::PR[110].paravalue;     //����
	//������������ʾ���ؼ�
	str.Format(L"%.2lf PCS", G::PR[110].paravalue);
	SetDlgItemText(IDC_TOTALCOUNTS_STATIC, str);                 //�ܲ���
	str.Format(L"%.2lf PCS", G::PR[111].paravalue);
	SetDlgItemText(IDC_OKCOUNTS_STATIC, str);                      //OK��
	str.Format(L"%.2lf PCS", G::PR[112].paravalue);
	SetDlgItemText(IDC_NGCOUNTS_STATIC, str);                    //NG��
	str.Format(L"%.2lf", G::PR[113].paravalue);
	str1 = str + L"%";
	SetDlgItemText(IDC_YIELD_STATIC, str1);                            //����
	//UPH
	str.Format(L"%.2lf PCS/H", G::PR[1].paravalue*3600*2/3500);
	SetDlgItemText(IDC_AVGUPH_STATIC, str);
	//CT
	str.Format(L"%.2lf S", 1750/G::PR[1].paravalue);
	SetDlgItemText(IDC_CT_STATIC, str);

	//������������
	G::WriteProduction();
	///////////////////////////////////////////////////////��ɫ��
	//����״̬
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
	//������
	if (1 == G::bAlarm)
	{
		if (0 == G::MCard.O[RED_LED].sValue)
			G::MCard.SetDoBit(RED_LED, 1);
		if (0 == G::MCard.O[BUZZER].sValue)
			G::MCard.SetDoBit(BUZZER, 1);
		G::MCard.SetDoBit(GREEN_LED, 0);
		G::MCard.SetDoBit(YELLOW_LED, 0);
	}
	//����������
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
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	    test=true;
//		m_hThread[18] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TTEST, this, 0, 0);
//}


BOOL CMy1269AirColumnDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
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


// �����б����ͣ����
void CMy1269AirColumnDlg::ToPause()
{
	SetDlgItemText(IDC_START_BUTTON, L"����");
	G::bRunning = 0;
}

//�������
void CMy1269AirColumnDlg::OnBnClickedShieldleftButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString	  str;
	GetDlgItemText(IDC_SHIELDLEFT_BUTTON, str);
	//δ����
	if (L"�������" == str)
	{
		G::bLeftNoShield = 0;
		//ֹͣ���˶�
		G::MCard.StopAxis(AXIS_LEFT, 1);
		SetDlgItemText(IDC_SHIELDLEFT_BUTTON, L"�������");
		if ((0 == G::bLeftNoShield) && (0 == G::bRightNoShield))
		{
			if (1 == G::bRunning)
				ToPause();
			G::bRunning = 0;
			MessageBox(L"����ȫ�����Σ�����ִͣ�У�����������һ���ٿ�ʼִ�У�", L"��ʾ", MB_OK);
		}
	}
	//�Ѿ��������
	else if (L"�������" == str)
	{
		G::bLeftNoShield = 1;
		if (1 == G::bRunning)
			dmc_vmove(0, AXIS_LEFT, 1);
		SetDlgItemText(IDC_SHIELDLEFT_BUTTON, L"�������");
	}
}

//�����Ҷ�
void CMy1269AirColumnDlg::OnBnClickedShieldrightButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString       str;
	GetDlgItemText(IDC_SHIELDRIGHT_BUTTON, str);
	//δ����
	if (L"�����Ҷ�" == str)
	{
		G::bRightNoShield = 0;
		G::MCard.StopAxis(AXIS_RIGHT, 1);
		SetDlgItemText(IDC_SHIELDRIGHT_BUTTON, L"�����Ҷ�");
		if ((0 == G::bLeftNoShield) && (0 == G::bRightNoShield))
		{
			if (1 == G::bRunning)
				ToPause();
			G::bRunning = 0;
			MessageBox(L"����ȫ�����Σ�����ִͣ�У�����������һ���ٿ�ʼִ�У�", L"��ʾ", MB_OK);
		}
	}
	//�Ѿ��������
	else if (L"�����Ҷ�" == str)
	{
		G::bRightNoShield = 1;
		if (1 == G::bRunning)
			dmc_vmove(0, AXIS_RIGHT, 1); 
		SetDlgItemText(IDC_SHIELDRIGHT_BUTTON, L"�����Ҷ�");
	}
}


void CMy1269AirColumnDlg::ReadIO()
{
	///////////////////////////////////////IO
	CFileFind finder;
	CString    str;
	BOOL ifinder = finder.FindFile(G::ParaFileName);
	//�ļ�Ϊ����Ĭ�ϲ���
	if (!ifinder)
	{
		G::WriteIOPara();
		return ;
	}
	else
	{
		try                //���������
		{
			//����˿���
			for (int i = 0; i < 16; i++)
			{
				str.Format(L"%d�������", i);
				::GetPrivateProfileString(L"InputPort", str, L"����Ĭ����", G::MCard.I[i].lpwstrPortName.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
			}
			//����˿���
			for (int i = 0; i < 16; i++)
			{
				str.Format(L"%d�������", i);
				::GetPrivateProfileString(L"OutPort", str, L"���Ĭ����", G::MCard.O[i].lpwstrPortName.GetBuffer(MAX_PATH), MAX_PATH, G::ParaFileName);
			}
		}
		catch (...)
		{
			AfxMessageBox(L"��ȡ����ʧ�ܣ�");
			return ;
		}
	}
}

//��ർ������
void CMy1269AirColumnDlg::OnBnClickedWritedataleftButton()
{
	CFileDialog filedlg(FALSE, L"xlsx", L"���ߴ�����", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"excel�ļ�(*.xls)|*.xlsx|ALL Files(*.*)||",this);         //TRUEΪ�򿪣�FALSΪ����
	CString    path;
	// ��ʾ�����ļ��Ի���   
	if (IDOK == filedlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ����桱��ť����ѡ����ļ�·����ʾ���༭����   
		path = filedlg.GetPathName();
	}
	
	path = filedlg.GetPathName();             //��ȡ�ļ�·��
	CString   title[] = { L"��Ʒ���", L"�ܳ�", L"����⾶", L"�Ҳ��⾶" };
	int   colcount = m_data_list.GetHeaderCtrl()->GetItemCount();
	int   rowcount = m_data_list.GetItemCount();

	filerw.ListToExcel(&m_data_list, path, title, rowcount, colcount);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//�Ҳർ������
void CMy1269AirColumnDlg::OnBnClickedWritedatarightButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog filedlg(FALSE, L"xlsx", L"�Ҳ�ߴ�����", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"excel�ļ�(*.xls)|*.xlsx|ALL Files(*.*)||");         //TRUEΪ�򿪣�FALSΪ����
	CString    path;
	if (IDOK == filedlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ����桱��ť����ѡ����ļ�·����ʾ���༭����   
		path = filedlg.GetPathName();
	}
	path = filedlg.GetPathName();             //��ȡ�ļ�·��
	CString   title[] = { L"��Ʒ���", L"�ܳ�", L"����⾶", L"�Ҳ��⾶" };
	int   colcount = m_datalist2.GetHeaderCtrl()->GetItemCount();
	int   rowcount = m_datalist2.GetItemCount();

	filerw.ListToExcel(&m_datalist2, path, title, rowcount, colcount);
}

//�˵�-�Ӿ�����
void CMy1269AirColumnDlg::OnVisondebug()
{
	// TODO:  �ڴ���������������
	CVisionDebugDlg   visiondebug;
	visiondebug.DoModal();   
}
