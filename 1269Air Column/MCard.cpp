#include "StdAfx.h"
#include "MCard.h"
#include"G.h"
//Ϊ�˸������ʹ�ã�����ͳһ�������£�
/***********************************ע������************************************************/
//�����˶����ƿ� 5000ϵ��  �汾Ver1.32��֧��IO��չ	                                     
//																	
// V1.0 2015 
// V1.2 2016.5.27.18.38 <1.����MessageBox�����д��ڵ��趨.>
// V1.3 2016.6.7.10.23  <1.���� HomeAxis2 > <2.����ԭ�����湦��>
// V1.31 2016.6.15.18.23  <1. > <2.����ԭ�����湦��>
// V1.32 2016.7.23.11.23  <1.���JOGMove��Jog���߱���ͳһ����> <2.����һάλ�ñȽϹ���>
// V1.33 2016.7.23.11.23  <1.�޸�EnableAxs �߼�BUG> <2.>



/***************************************************************************************** */
/*			�������� �йؾ����ʱ�䣬�Ƕ� �ĵط�ͳһ��λΪ ������(p) �� ��(s)			   */
/*						4/8�������ͳһΪ��0��ʼ [0,1,2,3,...]						       */
/*			IO�Ŀ��Ŵӱ����ƿ���ʼΪ���0����һ����չ��Ϊ1.....[0,1,2,,,,,,]			   */
/***************************************************************************************** */


CMCard::CMCard(void)
{
	m_pMBOwner=NULL;
	CString strTmp;
	m_EncSnsValue=0;
	m_bMotionCardOpenning = FALSE;

	m_pAxsSafeFilter=NULL;
	m_pDoSafeFilter =NULL;

	//����
	for(int i=0; i<(MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * INPUT_PORT_COUNT_MAX); i++)
	{
		I[i].sCardNo = BYTE(i / ((1 + EX_IONODE_COUNT_MAX) * INPUT_PORT_COUNT_MAX));
		I[i].sNodeNo = BYTE((i % ((1 + EX_IONODE_COUNT_MAX) * INPUT_PORT_COUNT_MAX)) / INPUT_PORT_COUNT_MAX);//�˶����ƿ� =0����չ����1��ʼ��
		I[i].sPortNo = BYTE(i % INPUT_PORT_COUNT_MAX);
		I[i].sValue	= 0;
		strTmp.Format(L"[%2d-%2d-%2d]����", I[i].sCardNo,I[i].sNodeNo,I[i].sPortNo);
		I[i].lpwstrPortName =(LPWSTR)(LPCTSTR)strTmp;
	}
	

	//���
	for(int i=0; i<(MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX); i++)
	{
		O[i].sCardNo = BYTE(i /((1 + EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX));//�˶����ƿ� =0����չ����1��ʼ��
		O[i].sNodeNo = BYTE((i % ((1 + EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX)) / OUTPUT_PORT_COUNT_MAX);
		O[i].sPortNo = BYTE( i % OUTPUT_PORT_COUNT_MAX);
		O[i].sValue	= 0;
		strTmp.Format(L"[%2d-%2d-%2d]����", O[i].sCardNo,O[i].sNodeNo,O[i].sPortNo);
		O[i].lpwstrPortName =(LPWSTR)(LPCTSTR)strTmp;
	}

	//�������ʼĬ��ֵ
	for(int i=0; i<(MCARD_COUNT_MAX * AXIS_COUNT_MAX); i++)
	{
		m_bAutoHomming[i]=FALSE;
		A[i].sCardNo = BYTE( i / AXIS_COUNT_MAX );
		A[i].sAxisNo = BYTE( i % AXIS_COUNT_MAX );

		A[i].AS.bDriverAlarm			=0					;		    //����������																					
		A[i].AS.bEComplet				=1					;		    //��   ���޸�Ӧ�������θ���״̬ 0��ʾ û��������									
		A[i].AS.bMComplet				=1					;		    //��   ���޸�Ӧ�������θ���״̬ 0��ʾ û��������									
		A[i].AS.bHome					=0					;		    //ԭ�� ���޸�Ӧ�������θ���״̬ 0��ʾ û�е�ԭ��									
		A[i].AS.bPLimtSnr				=0					;		    //���巢����ɣ�ִ�����״̬�����θ���״̬ ��	0��ʾ ����ִ��							
		A[i].AS.bNLimtSnr				=0					;		    //���������ʵ����ɣ�ִ�����״̬�����θ���״̬ ��	0��ʾ ����ִ��						
		A[i].AS.bORGSnr					=0					;		    //��ʾ��ǰ���Ƿ��Ѿ�����λ		//******  ���û���������и�λ��ɺ��Լ��趨  ******//	
		A[i].AS.bINPSnr					=0					;		    //																			
		A[i].AS.bEMGSnr					=0					;		    //																			
		A[i].AS.bEZSnr					=0					;		    //																			
		A[i].AS.bRDYSnr					=0					;		    //																			
		A[i].AS.lMCurPos				=0					;		    //����ﵱǰ��λ�� (������������													
		A[i].AS.lEncCurPos				=0					;		    //���������ǰλ�� (������������													
		A[i].AS.bDone					=1					;		    //��ʾ���Ƿ��Ѿ�ֹͣ������ bMComplet��bEComplet �����,����������������Ҫ���˶�״̬��bMComplet��bEComplet �������Ǹ����ģ����Բ����

		A[i].AP.dPPU					=9000				;		    //�������嵱��  n pulse = 1 unit(mm��deg,m...)Equiv//Ĭ��=1000pulse/mm			
		A[i].AP.nHomeMode				=1					;		    //����ģʽ //0��һ�λ���//1��һ�λ���ӻ���//2�����λ���	//3��һ�λ�����ټ�һ�� EZ ������л���//4����һ�� EZ ������л���		
		A[i].AP.nHomeDir				=1					;		    //���㷽��0������1������		
		A[i].AP.fHomeVel				=/*G::PR[0].paravalue*/20000			;		    //�����ٶ�      pulse/s		
		A[i].AP.fStartVel				=1000				;		    //��ʼ�ٶ�      pulse/s		
		A[i].AP.fVel					=40000				;		    //���������ٶ�  pulse/s			
		A[i].AP.fTAcc					=(float)0.01		;		    //����ʱ�� ��λs						
		A[i].AP.fTDec					=(float)0.01		;		    //����ʱ�� ��λs				
		A[i].AP.fTSmooth				=(float)0.25		;		    //S�ٶȶζ�ƽ��ʱ�� ��0��0.5s
		A[i].AP.nAxsEncoderFactor		=1					;		    //������ϵ���������Ǳ�����
		A[i].AP.nAxsBandDist			=(int)A[i].AP.dPPU/100	;		//��λ pulse //Ĭ�ϱ���ʱ��0.1s	
		A[i].AP.nBacklash				=0					;		    //�����϶ֵ pulse
		A[i].AP.nPulseMode				=0					;		    //=0 ,1 ,2 ,3  pulse+dir, //=4 ,5  ˫����ccw/cw 
		A[i].AP.nSrPinLogic				=0					;		    //=0 ���ø�Ӧ�ź���Ч��ƽ��0������Ч��1������Ч
		A[i].AP.fTStop					=(float)0.01		;		    //Ĭ��=0.01 ��ͣ��ʱ�� ��λ��s
		A[i].AP.bPWMEnable				=0					;		    //PWMʹ�ܣ�1��ʾPWM���ã�0��ʾ������

		A[i].AP.bAxsSvnEnable			=1					;		    //��ʹ�ܣ�						1��ʾ�����ã�0��ʾ��ʹ��
		A[i].AP.nAxsSvnEnablePinLogic	=0					;		    //�����ŷ�ʹ����Ч��ƽ��		0������Ч��1������Ч
		A[i].AP.bAxsInpEnable			=0					;		    //��INP�ź�ʹ�ܣ�				1��ʾ�����ã�0��ʾ��ʹ��	
		A[i].AP.nAxsInpPinLogic			=0					;		    //���� INP �źŵ���Ч��ƽ��		0������Ч��1������Ч
		A[i].AP.bAxsAlmEnable			=1					;		    //��ALM�ź�ʹ�ܣ�				1��ʾ�����ã�0��ʾ��ʹ��	
		A[i].AP.nAxsAlmPinLogic			=1					;		    //���� ALM �źŵ���Ч��ƽ��		0������Ч��Ĭ��1������Ч
		A[i].AP.nAxsErcEnable			=1					;		    //���� ERC �ź����		��		0���͵�ƽ��1���ߵ�ƽ	
		A[i].AP.nEncoderInMode			=3					;		    //���ñ������ļ�����ʽ			0���� A/B ��(����/����) 1��1��A/B		2��2��A/B 		3��4��A/B
		A[i].AP.nAxsEZPinLogic			=1					;		    //���� EZ �źŵ���Ч��ƽ��		0������Ч��Ĭ��1������Ч
		A[i].AP.nAxsEMGPinLogic			=1					;		    //���� ��ͣEMG�źŵ���Ч��ƽ��	0������Ч��Ĭ��1������Ч		
		A[i].AP.nEMGIOPort				=0					;		    //ָ����ǰ��ӳ��Ϊ��ͣ�ĵ�ǰ����ͨ��Input�˿�
		A[i].AP.nMotorType				=1					;		    //ָ����ǰ���������ͣ�		0��ʾ������Ĭ��1��ʾ�ջ��ŷ�
	}									   
}		

CMCard::~CMCard(void)
{
}

ERROR_RESULT CMCard::SetMBOwner(CWnd* pMBOwner)
{
	if(!::IsWindow(pMBOwner->GetSafeHwnd()))
	{
		DebugInfo2(L"OpenMotionCard::SetMBOwner �����������쳣,�����ڲ����ڣ�");  
		return MCARD_ERROR;
	}
	m_pMBOwner=pMBOwner;

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::MB(LPCTSTR lpcsText)
{
	if(::IsWindow(m_pMBOwner->GetSafeHwnd()))
	{
		m_pMBOwner->MessageBox(lpcsText);
	}
	else
	{
		AfxMessageBox(lpcsText);	
	}
	return MCARD_NO_ERROR;
}

ERROR_RESULT CMCard::OpenMotionCard(CWnd* pMBOwner)
{
	if(MCARD_NO_ERROR!=SetMBOwner(pMBOwner))
	{
		return MCARD_ERROR;
	}
	ClsAllErrInfo();

	m_bMotionCardOpenning= FALSE;
	BOOL rtn;
	//�򿪲����������ἰ������չģ��
	rtn = dmc_board_init();
	if(rtn == 0)
	{
		DebugInfo2(L"OpenMotionCard::dmc_board_initû���ҵ����ƿ������߿��ƿ��쳣��");  
		return MCARD_ERROR;
	}
	else if(rtn < 0)
	{
		CString str;
		str.Format(L"OpenMotionCard::dmc_board_init����2������Ӳ����������%d�ŵ��˶����ƿ���");
		DebugInfo2(str);  
		return MCARD_ERROR;
	}
	else
		m_nActualCardCount = rtn; 


	m_bMotionCardOpenning= TRUE;

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::CloseMotionCard()
{
	dmc_board_close();
	m_bMotionCardOpenning = FALSE;
	return MCARD_NO_ERROR;
}

ERROR_RESULT	CMCard::SetAxisParam(UINT uAxsIndex)
{
	short rtn=0;
	CString str;	
	if(m_bMotionCardOpenning ==FALSE)
	{
		DebugInfo2(L"SetAxisParam::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//1.����������Ƿ����Ҫ��//�κ�һ����ϣ�return FALSE; //��ʵ��

	//��鿨�Ƿ����
	short sTmpCardNo = short( uAxsIndex / AXIS_COUNT_MAX );
	short sTmpAxisNo = short( uAxsIndex % AXIS_COUNT_MAX );
	if (sTmpCardNo < m_nActualCardCount )
	{//������
		A[uAxsIndex].sCardNo =sTmpCardNo;

		DWORD dwAxisCount=0;
		rtn = dmc_get_total_axes(WORD(sTmpCardNo),&dwAxisCount);
		if (rtn != 0)
		{
			str.Format(L"SetAxisParam::dmc_get_total_axes ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
		if ((DWORD)sTmpAxisNo < dwAxisCount)
		{
			//�������
			A[uAxsIndex].sAxisNo = sTmpAxisNo;
		}
		else
		{
			//���᲻����
			A[uAxsIndex].sAxisNo = -1;
		}
	}
	else
	{//��������
		A[uAxsIndex].sCardNo = -1;
	}

	//2.����ÿ��������˶����ƿ�
	if (A[uAxsIndex].sCardNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"�������ڣ�");
		return MCARD_ERROR;
	}
	if (A[uAxsIndex].sAxisNo != -1)//-1��ʾ������
	{
			//����ָ������������ģʽ
			rtn = dmc_set_pulse_outmode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,A[uAxsIndex].AP.nPulseMode);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_pulse_outmode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			
			//���� ORG ԭ���ź���Ч��ƽ��0������Ч��1������Ч
			rtn = dmc_set_home_pin_logic(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,A[uAxsIndex].AP.nSrPinLogic,0.0);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_home_pin_logic ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//���� EL ��λ�ź�,ʹ��״̬,��Ч��ƽ,�ƶ���ʽ
			rtn = dmc_set_el_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 1,A[uAxsIndex].AP.nSrPinLogic, 0);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_el_mode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}			
			//��������λ
			//rtn = dmc_set_softlimit(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 0, 0, 0, 0, 0);
			//if (rtn != 0)
			//{
			//	str.Format(L"SetAxisParam::dmc_set_softlimit ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			//	DebugInfo2(str);
			//	return MCARD_ERROR;
			//}
			//���õ����ٶ�S���߲���
			/*rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,double(A[uAxsIndex].AP.fStartVel) ,
				double(A[uAxsIndex].AP.fVel),double(A[uAxsIndex].AP.fTAcc),
				double(A[uAxsIndex].AP.fTDec),double(A[uAxsIndex].AP.fStartVel));
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}*/
			//rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 0, 
			//	double(A[uAxsIndex].AP.fTSmooth));
			//if (rtn != 0)
			//{
			//	str.Format(L"SetAxisParam::dmc_set_s_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			//	DebugInfo2(str);
			//	return MCARD_ERROR;
			//}

			//�����϶ֵ
			/*rtn = dmc_set_backlash_unit(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 
				double(A[uAxsIndex].AP.nBacklash)/double(A[uAxsIndex].AP.dPPU));
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_backlash_unit ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}*/


			//����ָ����� INP �ź�,INP �ź�ʹ�ܣ�0����ֹ��1������,INP �źŵ���Ч��ƽ��0������Ч��1������Ч
			//ע�⣺��ʹ�� INP �źŹ��ܺ�ֻ���� INP �ź�Ϊ��Ч״̬ʱ����Ӧ������ܽ����˶�������
			//��ʱ������״̬���������У��������˶������ƣ�			
			rtn = dmc_set_inp_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.bAxsInpEnable,
				A[uAxsIndex].AP.nAxsInpPinLogic);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_inp_mode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}			
		
			//����ָ����� ALM �ź�ALM �ź�ʹ�ܣ�0����ֹ��1������ALM �źŵ���Ч��ƽ��0������Ч��1������ЧALM �źŵ��ƶ���ʽ��0������ֹͣ��ֻ֧�ָ÷�ʽ��
			rtn = dmc_set_alm_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.bAxsAlmEnable,
				A[uAxsIndex].AP.nAxsAlmPinLogic,0);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_alm_mode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}	

			////����ָ����� ALM �ź�ALM �ź�ʹ�ܣ�0����ֹ��1������ALM �źŵ���Ч��ƽ��0������Ч��1������ЧALM �źŵ��ƶ���ʽ��0������ֹͣ��ֻ֧�ָ÷�ʽ��
			//rtn = dmc_set_alm_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,
			//	A[uAxsIndex].AP.bAxsAlmEnable, A[uAxsIndex].AP.nAxsAlmPinLogic,0);
			//if (rtn != 0)
			//{
			//	str.Format(L"SetAxisParam::dmc_set_alm_mode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			//	DebugInfo2(str);
			//	return MCARD_ERROR;
			//}

			//����ָ����� ERC �ź���������ƽ��0���͵�ƽ��1���ߵ�ƽ
			rtn = dmc_write_erc_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsErcEnable);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_write_erc_pin ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			//���ñ������ļ�����ʽ
			//	0���� A/B ��(����/����)
			//	1��1��A/B
			//	2��2��A/B
			//	3��4��A/B
			rtn = dmc_set_counter_inmode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nEncoderInMode);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_counter_inmode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//����ָ����� EZ �ź���Ч��ƽ��0������Ч��1������Ч
			rtn = dmc_set_ez_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsEZPinLogic,0,0);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_ez_mode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			//���� EMG ��ͣ�ź���Ч��ƽ��0������Ч��1������Ч
			rtn = dmc_set_emg_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 1,A[uAxsIndex].AP.nAxsEMGPinLogic);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_emg_mode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//ֱ������
			//���ü���ͣ�ź���Ч��ƽ��0������Ч��1������Ч
			rtn = dmc_set_io_dstp_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 0,0);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_io_dstp_mode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//���ü���ֹͣʱ�䣬��λ��s
			//ע �⣺�������쳣ֹͣʱ���磺��λ�źţ���Ӳ����������������ֹͣ�ź�(DSTP)��������
			//���м���ֹͣʱ�� ����ֹͣʱ�䶼Ϊ dmc_set_dec_stop_time ���������õļ���ʱ��
			//���õ� 0 �ſ���ͨ������� 0 �ӿ���Ϊ������ļ�ͣ�źţ��������ü�ͣ�ź�Ϊ��
			//��ƽ��Ч
			rtn = dmc_set_dec_stop_time(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,A[uAxsIndex].AP.fTStop);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_dec_stop_time ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}


			//double Filter;
			//CardNo = 0; //����
			//IoType = 3; //ָ����� IO �ź�����Ϊ����ͣ�ź�
			//MapIoType = 6; //�� IO ӳ�����ͣ�ͨ������˿�
			//MapIoIndex = 0; //�� IO ӳ�������ţ�ͨ������ 0
			//������ IO ӳ���ϵ
			rtn = dmc_set_axis_io_map(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 3, 6, A[uAxsIndex].AP.nEMGIOPort, 0.02);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_axis_io_map ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//������ϵ����˵������ʹ�� dmc_check_success_encoder �������������Ƿ�λʱ��������
			//�жϵı�����λ��Ϊ������������ֵ���Ա�����ϵ����ֵ�����ڼ���ᵽ
			//λ״̬�������÷�����½� 7.10 ����ᵽλ״̬���ܵ�ʵ��
			//����λ����������λ��pulse
			rtn = dmc_set_factor_error(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,  A[uAxsIndex].AP.nAxsEncoderFactor,
																	A[uAxsIndex].AP.nAxsBandDist);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_factor_error ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//short dmc_set_equiv(WORD CardNo,WORD axis, double equiv)
			//	�� �ܣ��������嵱��ֵ
			//	�� ����CardNo ����
			//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
			//	DMC5400A��0~3
			//	equiv ���嵱������λ��pulse/unit
			//	����ֵ���������
			//	ע �⣺1���ú��������ڻ������嵱���ĸ߼��˶�������������λ���岹�������岹�˶���
			//	2����ʹ�û������嵱���ĸ߼��˶����������˶�ǰ��������ʹ�øú������ø��˶�
			//	������嵱��ֵ����ֵ��������Ϊ 0
			//�������嵱��ֵ
			rtn= dmc_set_equiv(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,A[uAxsIndex].AP.dPPU);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_equiv ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

		}
		//PWM����
		//ע�⣺1��	DMC5800 �У���ʹ�� PWM ���ܺ�7 ���������˿ڣ�PUL+�� PUL-����Ϊ PWM ���
		//			ͨ�� 0��7 ����ķ���˿ڣ�DIR+�� DIR-����Ϊ PWM ���ͨ�� 1��
		//		2��	DMC5600 �У���ʹ�� PWM ���ܺ�5 ���������˿ڣ�PUL+�� PUL-����Ϊ PWM ���
		//			ͨ�� 0��5 ����ķ���˿ڣ�DIR+�� DIR-����Ϊ PWM ���ͨ�� 1��
		//		3��	��ʹ��ͨ�� 0 ʱ��Ӳ�����߿ɽ� PUL+�� GND��PUL-�� GND ������෴�źţ�����ʹ
		//			��ͨ�� 1 ʱ��Ӳ�����߿ɽ� DIR+�� GND��DIR-�� GND ������෴�źţ���
		//		4��	DMC5400A �� CN23 ����·ר������ڣ������¼ 5��
		//		4��	��ʹ�� PWM ���ܺ��˶����ƿ�����Ӧ��˿ڽ����������������źš�
		/*rtn= dmc_set_pwm_enable(A[uAxsIndex].sCardNo, A[uAxsIndex].AP.bPWMEnable);
		if (rtn != 0)
		{
			str.Format(L"SetAxisParam::dmc_set_pwm_enable ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}*/


		//short dmc_set_can_state(WORD CardNo,WORD NodeNum,WORD state,WORD Baud)
		//	�� �ܣ����� CAN-IO ͨѶ״̬��DMC5800/5600/5400A ��ר��
		//	�� ����CardNo ���ƿ�����
		//	NodeNum CAN �ڵ�����ȡֵ��Χ��1~4
		//	DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
		//	�� 263 ��
		//	state ����ͨѶ״̬��0���Ͽ���1������
		//	Baud ���ÿ��ƿ�������
		//	�����ʲ�����0,1,2,3,4,5,6
		//	��Ӧ�����ʣ�1000Kbps��800Kbps,500Kbps,250Kbps,125Kbps,100Kbps
		//	����ֵ���������
		//	ע �⣺1�����ر��˶����ƿ�ʱ��CAN-IO ͨѶ���ᱻ�Զ��Ͽ������ٴγ�ʼ���˶����ƿ�ʱ��
		//	CAN-IO ͨѶ��Ȼ����֮ǰ��״̬
		//	2�������� CAN ͨѶʱ������ʹ�� dmc_get_can_state ������ȡ CAN-IO ��ͨѶ״̬��
		//	ȷ�� CAN ͨѶ���������ӡ������ӳ����쳣ʱ�����ٴε��� dmc_set_can_state
		//	������������
		//	3�����ò�����ʱ�豣֤���ƿ���������ģ�鲨�������Ӧ
		
		////���� CAN-IO ͨѶ״̬
		/*rtn= dmc_set_can_state(A[uAxsIndex].sCardNo,EX_IONODE_COUNT_MAX,1,0);
		if (rtn != 0)
		{
			str.Format(L"SetAxisParam::dmc_set_can_state ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}*/

		//short dmc_set_da_enable(WORD CardNo,WORD enable);
		//�� �ܣ����� DA ���ʹ��
		//	�� ����CardNo ����
		//	WORD enable DA ʹ��״̬��0����ֹ��1��ʹ��
		/*rtn= dmc_set_da_enable(A[uAxsIndex].sCardNo,0);
		if (rtn != 0)
		{
			str.Format(L"SetAxisParam::dmc_set_da_enable ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}*/
		
		//G::MCard.SetAxisCurPos(AXS_TURNTABLE,0);
		//G::MCard.SetAxisEncPos(AXS_TURNTABLE,0);
		
		//�����ŷ�ʹ�ܶ˿ڵ�ƽ��0���͵�ƽ��1���ߵ�ƽ

		if ( A[uAxsIndex].AP.bAxsSvnEnable==1)
		{
			rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsSvnEnablePinLogic?0:1);//GT_AxisOn(uAxisNo+1);
			if (rtn != 0) 
			{
				str.Format(L"SetAxisParam::dmc_write_sevon_pin ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
		}
		else
		{
			rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsSvnEnablePinLogic?1:0);//GT_AxisOn(uAxisNo+1);
			if (rtn != 0) 
			{
				str.Format(L"SetAxisParam::dmc_write_sevon_pin ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
		}
	return EnableAxis(uAxsIndex,A[uAxsIndex].AP.bAxsSvnEnable);
}
//��ǰ�Զ���ԭ��
ERROR_RESULT CMCard::HomeAxis1(UINT uAxsIndex)
{
	int rtn=0;
	CString str;
	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
	{
		return MCARD_ERROR;
	}

	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"HomeAxis1::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"HomeAxis1::�����᲻���ڣ�");
		return MCARD_ERROR;
	}

	//�����ʹ��
	if( A[uAxsIndex].AP.bAxsSvnEnable ==FALSE) 
	{
		str.Format(L"HomeAxis1::��%dʹ��û�д򿪣���",uAxsIndex);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	m_bAutoHomming[uAxsIndex] = TRUE;//���� ���ڽ��и�λ ��־



	rtn=StopAxis(uAxsIndex,1);//����ֹͣ
	if (rtn != MCARD_NO_ERROR) 
	{
		DebugInfo2(L"HomeAxis :: StopAxis ,����ʧ�ܣ���");
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}
	//short dmc_set_profile(WORD CardNo,WORD axis,double Min_Vel,double Max_Vel,double
	//Tacc,double Tdec,double Stop_Vel)
	//�� �ܣ����õ����˶��ٶ�����
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//Min_Vel ��ʼ�ٶȣ���λ��pulse/s (���ֵΪ 2M)
	//Max_Vel ����ٶȣ���λ��pulse/s (���ֵΪ 2M)
	//Tacc ����ʱ�䣬��λ��s (��СֵΪ 0.001s)
	//Tdec ����ʱ�䣬��λ��s (��СֵΪ 0.001s)
	//Stop_Vel ֹͣ�ٶȣ���λ��pulse/s (���ֵΪ 2M)
	//����ֵ���������
	//���õ����ٶ�S���߲���
	
	rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,double(A[uAxsIndex].AP.fHomeVel/*double(A[uAxsIndex].AP.fStartVel*/) ,
		double(A[uAxsIndex].AP.fVel),
		double(A[uAxsIndex].AP.fTAcc),double(A[uAxsIndex].AP.fTDec),0/*double(A[uAxsIndex].AP.fStartVel)*/);
	if (rtn != 0)
	{
		str.Format(L"HomeAxis::::dmc_set_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	//short dmc_set_s_profile(WORD CardNo,WORD axis,WORD s_mode,double s_para)
	//	�� �ܣ����õ����ٶ����� S �β���ֵ
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	s_mode �����������̶�ֵΪ 0
	//	DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
	//	�� 230 ��
	//	s_para S ��ʱ�䣬��λ��s����Χ��0~0.5 s
	//	����ֵ���������
	rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,double(A[uAxsIndex].AP.fTSmooth));
	if (rtn != 0)
	{
		str.Format(L"HomeAxis::::dmc_set_s_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//short dmc_set_homemode(WORD CardNo,WORD axis,WORD home_dir,double vel_mode,
	//	WORD mode,WORD EZ_count)
	//	�� �ܣ����û�ԭ��ģʽ
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
	//	�� 222 ��
	//	home_dir ���㷽��0������1������
	//	vel_mode �����ٶ�ģʽ��
	//	0�����ٻ��㣬���Ա�ָ��ǰ��� dmc_set_profile �������õ���
	//	ʼ�ٶ�����
	//	1�����ٻ��㣬���Ա�ָ��ǰ��� dmc_set_profile �������õ���
	//	���ٶ�����
	//	mode ����ģʽ��
	//	0��һ�λ���
	//	1��һ�λ���ӻ���
	//	2�����λ���
	//	3��һ�λ�����ټ�һ�� EZ ������л���
	//	4����һ�� EZ ������л���
	//	EZ_count �����������̶�ֵΪ 0
	//	����ֵ���������
	//	ע�⣺������ģʽ mode=4 ʱ�������ٶ�ģʽ���̶�Ϊ���ٻ���
	rtn = dmc_set_homemode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,
		A[uAxsIndex].AP.nHomeDir, 0/*double(A[uAxsIndex].AP.fHomeVel)*/, A[uAxsIndex].AP.nHomeMode, 0);
	if( rtn != 0) 
	{
		str.Format(L"HomeAxis :: dmc_set_homemode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}

	//short dmc_home_move(WORD CardNo,WORD axis)
	//	�� �ܣ���ԭ���˶�
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	����ֵ���������


	//��ʼ������״̬���������û�������˺����������GetHomeAxisSts����������������״̬
	A[uAxsIndex].AS.bMComplet	= FALSE;
	A[uAxsIndex].AS.bEComplet	= FALSE;
	A[uAxsIndex].AS.bHome		= FALSE;
	rtn = dmc_home_move(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if( rtn != 0) 
	{
		str.Format(L"HomeAxis :: dmc_set_homemode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}

	//�ȵ���һ�θ���һ��״̬
	GetAxisStatus(uAxsIndex);

	return MCARD_NO_ERROR;//�ɹ����� 0��
}
//��ǰ�Զ���ԭ�� nHomeDir =1 ����nVelMode =0 ���٣�nHomeMode =4 Z�����
ERROR_RESULT CMCard::HomeAxis2(UINT uAxsIndex,int nHomeDir,int nVelMode,int nHomeMode)
{
	int rtn=0;
	CString str;
	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
	{
		return MCARD_ERROR;
	}

	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"HomeAxis1::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"HomeAxis1::�����᲻���ڣ�");
		return MCARD_ERROR;
	}

	//�����ʹ��
	if( A[uAxsIndex].AP.bAxsSvnEnable ==FALSE) 
	{
		str.Format(L"HomeAxis1::��%dʹ��û�д򿪣���",uAxsIndex);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	m_bAutoHomming[uAxsIndex] = TRUE;//���� ���ڽ��и�λ ��־



	rtn=StopAxis(uAxsIndex,1);//����ֹͣ
	if (rtn != MCARD_NO_ERROR) 
	{
		DebugInfo2(L"HomeAxis :: StopAxis ,����ʧ�ܣ���");
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}
	//short dmc_set_profile(WORD CardNo,WORD axis,double Min_Vel,double Max_Vel,double
	//Tacc,double Tdec,double Stop_Vel)
	//�� �ܣ����õ����˶��ٶ�����
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//Min_Vel ��ʼ�ٶȣ���λ��pulse/s (���ֵΪ 2M)
	//Max_Vel ����ٶȣ���λ��pulse/s (���ֵΪ 2M)
	//Tacc ����ʱ�䣬��λ��s (��СֵΪ 0.001s)
	//Tdec ����ʱ�䣬��λ��s (��СֵΪ 0.001s)
	//Stop_Vel ֹͣ�ٶȣ���λ��pulse/s (���ֵΪ 2M)
	//����ֵ���������
	//���õ����ٶ�S���߲���
	
	rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,A[uAxsIndex].AP.fHomeVel,
		double(A[uAxsIndex].AP.fHomeVel),
		double(A[uAxsIndex].AP.fTAcc),double(A[uAxsIndex].AP.fTDec),double(A[uAxsIndex].AP.fStartVel));
	if (rtn != 0)
	{
		str.Format(L"HomeAxis::::dmc_set_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	//short dmc_set_s_profile(WORD CardNo,WORD axis,WORD s_mode,double s_para)
	//	�� �ܣ����õ����ٶ����� S �β���ֵ
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	s_mode �����������̶�ֵΪ 0
	//	DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
	//	�� 230 ��
	//	s_para S ��ʱ�䣬��λ��s����Χ��0~0.5 s
	//	����ֵ���������
	rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,double(A[uAxsIndex].AP.fTSmooth));
	if (rtn != 0)
	{
		str.Format(L"HomeAxis::::dmc_set_s_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//short dmc_set_homemode(WORD CardNo,WORD axis,WORD home_dir,double vel_mode,
	//	WORD mode,WORD EZ_count)
	//	�� �ܣ����û�ԭ��ģʽ
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
	//	�� 222 ��
	//	home_dir ���㷽��0������1������
	//	vel_mode �����ٶ�ģʽ��
	//	0�����ٻ��㣬���Ա�ָ��ǰ��� dmc_set_profile �������õ���
	//	ʼ�ٶ�����
	//	1�����ٻ��㣬���Ա�ָ��ǰ��� dmc_set_profile �������õ���
	//	���ٶ�����
	//	mode ����ģʽ��
	//	0��һ�λ���
	//	1��һ�λ���ӻ���
	//	2�����λ���
	//	3��һ�λ�����ټ�һ�� EZ ������л���
	//	4����һ�� EZ ������л���
	//	EZ_count �����������̶�ֵΪ 0
	//	����ֵ���������
	//	ע�⣺������ģʽ mode=4 ʱ�������ٶ�ģʽ���̶�Ϊ���ٻ���
	rtn = dmc_set_homemode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,
		nHomeDir, nVelMode, nHomeMode, 0);
	if( rtn != 0) 
	{
		str.Format(L"HomeAxis :: dmc_set_homemode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}

	//short dmc_home_move(WORD CardNo,WORD axis)
	//	�� �ܣ���ԭ���˶�
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	����ֵ���������


	//��ʼ������״̬���������û�������˺����������GetHomeAxisSts����������������״̬
	A[uAxsIndex].AS.bMComplet	= FALSE;
	A[uAxsIndex].AS.bEComplet	= FALSE;
	A[uAxsIndex].AS.bHome		= FALSE;
	rtn = dmc_home_move(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if( rtn != 0) 
	{
		str.Format(L"HomeAxis :: dmc_set_homemode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}
	
	//�ȵ���һ�θ���һ��״̬
	GetAxisStatus(uAxsIndex);
	return MCARD_NO_ERROR;//�ɹ����� 0��
}

////��ȡHomeAxis1ִ�к�Ľ����״̬
//ERROR_RESULT CMCard::GetHomeAxis1Sts(UINT uAxisNo)
//{
//	//ֻ������ȷ����HomeAxis1 ֮����ܵ��ô˺��� �� m_bAutoHomming[x]==TRUE
//	if (m_bAutoHomming[uAxisNo])
//	{	
//		DebugInfo(L"ֻ������ȷ����HomeAxis1֮����ܵ��ô˺���,����ʧ�ܣ���");
//		return MCARD_ERROR;	
//	}	
//	unsigned short sts;
//	int rtn = GT_HomeSts(uAxisNo+1,&sts);
//	if (rtn!=MCARD_NO_ERROR)	
//	{
//		DebugInfo2(L"HomeAxis :: GT_Home ,����ʧ�ܣ���");
//		return MCARD_ERROR;
//	}
//	AS[uAxisNo].bMComplet = (int)sts;//0���Զ���ԭ���������ִ�С�1���Զ���ԭ������ɹ�ִ����ϡ�
//	AS[uAxisNo].bEComplet = (int)sts;//0���Զ���ԭ���������ִ�С�1���Զ���ԭ������ɹ�ִ����ϡ�
//
//	return MCARD_NO_ERROR;//�ɹ����� 0��
//}

//��ȡ��ǰ���λ��
ERROR_RESULT CMCard::GetAxisCurPos(UINT uAxsIndex)
{
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"GetAxisCurPos::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"GetAxisCurPos::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	//long dmc_get_position(WORD CardNo,WORD axis)
	//�� �ܣ���ȡָ������λ��
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//����ֵ��ָ������λ�ã���λ��pulse
	A[uAxsIndex].AS.lMCurPos = dmc_get_position(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);

	return  MCARD_NO_ERROR;
}
//��ȡ��ǰ���������λ��
ERROR_RESULT CMCard::GetAxisEncPos(UINT uAxsIndex)
{
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"GetAxisEncPos::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"GetAxisEncPos::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	//long dmc_get_encoder(WORD CardNo,WORD axis)
	//�� �ܣ���ȡָ����������������ֵ
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//����ֵ������������ֵ����λ��pulse
	//˵ �����˺��� axis ����Ϊ 8 ʱ���Զ����ֱ���������ֵ
	A[uAxsIndex].AS.lEncCurPos = dmc_get_encoder(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);

	return  MCARD_NO_ERROR;;
}
//��ȡ��ǰ��ĸ���״̬
ERROR_RESULT CMCard::GetAxisStatus(UINT uAxsIndex)
{
	DWORD dwSts=0;
	int rtn=0;

	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"GetAxisStatus ::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"GetAxisStatus ::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	
	//DWORD dmc_axis_io_status(WORD CardNo,WORD axis)
	//�� �ܣ���ȡָ�����й��˶��źŵ�״̬
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//����ֵ������ 9.2
	//�� 9.2 ����˶��ź�״̬
	//λ�� �ź����� ����
	//0 ALM 1����ʾ�ŷ������ź� ALM Ϊ ON�� 0��OFF
	//1 EL+ 1����ʾ��Ӳ��λ�ź� +EL Ϊ ON�� 0��OFF
	//2 EL- 1����ʾ��Ӳ��λ�źŨCEL Ϊ ON�� 0��OFF
	//3 EMG 1����ʾ��ͣ�ź� EMG Ϊ ON�� 0��OFF
	//4 ORG 1����ʾԭ���ź� ORG Ϊ ON�� 0��OFF
	//6 SL+ 1����ʾ������λ�ź�+SL Ϊ ON�� 0��OFF
	//7 SL- 1����ʾ�������λ�ź�-SL Ϊ ON�� 0��OFF
	//8 INP 1����ʾ�ŷ���λ�ź� INP Ϊ ON�� 0��OFF
	//9 EZ 1����ʾ EZ �ź�Ϊ ON�� 0��OFF
	//DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
	//�� 228 ��
	//λ�� �ź����� ����
	//10 RDY		1����ʾ�ŷ�׼���ź� RDY Ϊ ON��DMC5800 ��ר�ã� ��		0��OFF		
	//11 DSTP 1�� ��ʾ����ֹͣ�ź� DSTP Ϊ ON ��DMC5800 ��ר�ã� ��		0��OFF		
	//����λ ����
	dwSts = dmc_axis_io_status(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	A[uAxsIndex].AS.bDriverAlarm	= (((dwSts)>>0) & 1) ;	
	A[uAxsIndex].AS.bPLimtSnr		= (((dwSts)>>1) & 1) ;
	A[uAxsIndex].AS.bNLimtSnr		= (((dwSts)>>2) & 1) ;
	A[uAxsIndex].AS.bEMGSnr			= (((dwSts)>>3) & 1) ;	
	A[uAxsIndex].AS.bORGSnr			= (((dwSts)>>4) & 1) ;
	//A[uAxsIndex].AS.bDriverAlarm	= (((dwSts)>>5) & 1) ;
	//A[uAxsIndex].AS.bDriverAlarm	= (((dwSts)>>6) & 1) ;
	A[uAxsIndex].AS.bINPSnr			= (((dwSts)>>7) & 1) ;
	A[uAxsIndex].AS.bEZSnr			= (((dwSts)>>8) & 1) ;	
	//A[uAxsIndex].AS.bDriverAlarm	= (((dwSts)>>9) & 1) ;
	A[uAxsIndex].AS.bRDYSnr			= (((dwSts)>>10) & 1) ;


	//short dmc_check_done(WORD CardNo,WORD axis)
	//�� �ܣ����ָ������˶�״̬
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//����ֵ��0��ָ�����������У�1��ָ������ֹͣ
	//ע �⣺�˺��������ڵ��ᡢPVT �˶�

	//short dmc_check_success_pulse(WORD CardNo,WORD axis)
	//�� �ܣ����ָ�λ
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//����ֵ��0����ʾָ��λ�����趨��Ŀ��λ�õ�����֮��
	//1����ʾָ��λ�����趨��Ŀ��λ�õ�����֮��
	//ע �⣺1���ú���ֻ�����ڵ����˶�
	//2����⺯������ dmc_check_done ��⵽��ֹͣ����ã��������ú��ȴ��ᵽλ��
	//���أ�������ú��� 100ms ��δ��λ��������ʱ������Ϊ����λ
	
	//short dmc_check_success_encoder(WORD CardNo,WORD axis)
	//�� �ܣ�����������λ
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//����ֵ��0����ʾ������λ�����趨��Ŀ��λ�õ�����֮��
	//1����ʾ������λ�����趨��Ŀ��λ�õ�����֮��
	//ע �⣺1���ú���ֻ�����ڵ����˶�
	//2����⺯������ dmc_check_done ��⵽��ֹͣ����ã� �������ú��ȴ��ᵽλ��
	//���أ�������ú��� 100ms ��δ��λ��������ʱ������Ϊ����λ	

	dwSts = dmc_check_done(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);	
	A[uAxsIndex].AS.bDone = dwSts;
	if (dwSts == 1)
	{
		//����Ϊ�ŷ�
		if (	( A[uAxsIndex].AP.nMotorType == 1/* && A[uAxsIndex].AS.bINPSnr == 1 */)	
			/*||	( A[uAxsIndex].AP.nMotorType == 0  )*/ )
		{
			A[uAxsIndex].AS.bMComplet = BOOL(dmc_check_success_pulse	(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo));
			A[uAxsIndex].AS.bEComplet = BOOL(dmc_check_success_encoder	(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo));
		}
		else
		{
			A[uAxsIndex].AS.bMComplet	=1	;
			A[uAxsIndex].AS.bEComplet	=1	;
		}  

	}

	rtn = GetAxisCurPos(uAxsIndex);
	if (rtn!=MCARD_NO_ERROR)	
	{
		DebugInfo2(L"GetAxisStatus ::GetAxisCurPos ,����ʧ�ܣ���");
		return MCARD_ERROR;
	}

	rtn= GetAxisEncPos(uAxsIndex);
	if (rtn!=MCARD_NO_ERROR)	
	{
		DebugInfo2(L"GetAxisStatus ::GetAxisEncPos ,����ʧ�ܣ���");	
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}
//������ĵ�ǰλ��
ERROR_RESULT CMCard::SetAxisCurPos(UINT uAxsIndex,long lPos)//������λ��
{
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetAxisCurPos :: �˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"SetAxisCurPos :: �����᲻���ڣ�");
		return MCARD_ERROR;
	}

	//short dmc_set_position(WORD CardNo,WORD axis,long current_position)
	//	�� �ܣ�����ָ������λ��
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	current_position ָ������λ�ã���λ��pulse
	//	����ֵ���������

	int rtn =dmc_set_position(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,lPos);
	if (rtn!=0)	
	{
		CString str;
		str.Format(L"SetAxisCurPos :: dmc_set_position ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	return MCARD_NO_ERROR;
}
//���õ�ǰ���������λ��
ERROR_RESULT CMCard::SetAxisEncPos(UINT uAxsIndex, long lPos)
{

	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetAxisEncPos ::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"SetAxisEncPos ::�����᲻���ڣ�");
		return MCARD_ERROR;
	}

	//short dmc_set_encoder(WORD CardNo,WORD axis,long encoder_value)
	//DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
	//�� 245 ��
	//�� �ܣ�����ָ����������������ֵ
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//encoder_value ����������ֵ����λ��pulse
	//����ֵ���������
	int rtn =dmc_set_encoder(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,lPos);
	if (rtn!=0)	
	{
		CString str;
		str.Format(L"SetAxisEncPos :: dmc_set_encoder ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	return  MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::RMove2(UINT uAxsIndex, float fVel,long lDist)
{
	int rtn=0;
	CString str;


	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"RMove2::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"RMove2::�����᲻���ڣ�");
		return MCARD_ERROR;
	}


	if (TRUE != A[uAxsIndex].AP.bAxsSvnEnable)
	{
		CString str;
		str.Format(L"RMove2::��%dʹ�ܹرղ����ƶ���",uAxsIndex);
		AfxMessageBox(str);
		DebugInfo(str);
		return MCARD_ERROR;
	}
	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
	{
		return MCARD_ERROR;
	}

	rtn	= StopAxis(uAxsIndex,1);//��ֹͣ
	if (rtn != MCARD_NO_ERROR) 
	{
		DebugInfo(L"RMove2 :: StopAxis ,����ʧ�ܣ���");
		return MCARD_ERROR;
	}
//dmc_set_pulse_outmode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 0);//���� 0 �������������ʽ
	if (1==A[uAxsIndex].AS.bDone)
	{
		//short dmc_set_profile(WORD CardNo,WORD axis,double Min_Vel,double Max_Vel,double
		//Tacc,double Tdec,double Stop_Vel)
		//�� �ܣ����õ����˶��ٶ�����
		//�� ����CardNo ���ƿ�����
		//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
		//DMC5400A��0~3
		//Min_Vel ��ʼ�ٶȣ���λ��pulse/s (���ֵΪ 2M)
		//Max_Vel ����ٶȣ���λ��pulse/s (���ֵΪ 2M)
		//Tacc ����ʱ�䣬��λ��s (��СֵΪ 0.001s)
		//Tdec ����ʱ�䣬��λ��s (��СֵΪ 0.001s)
		//Stop_Vel ֹͣ�ٶȣ���λ��pulse/s (���ֵΪ 2M)
		//����ֵ���������
		//���õ����ٶ�S���߲���
		rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,double(A[uAxsIndex].AP.fStartVel) ,
			double(fVel),
			double(A[uAxsIndex].AP.fTAcc),double(A[uAxsIndex].AP.fTDec),double(A[uAxsIndex].AP.fStartVel));
		if (rtn != 0)
		{
			str.Format(L"RMove2::::dmc_set_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
		//short dmc_set_s_profile(WORD CardNo,WORD axis,WORD s_mode,double s_para)
		//	�� �ܣ����õ����ٶ����� S �β���ֵ
		//	�� ����CardNo ���ƿ�����
		//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
		//	DMC5400A��0~3
		//	s_mode �����������̶�ֵΪ 0
		//	DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
		//	�� 230 ��
		//	s_para S ��ʱ�䣬��λ��s����Χ��0~0.5 s
		//	����ֵ���������
		//rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,double(A[uAxsIndex].AP.fTSmooth));
		//if (rtn != 0)
		//{
		//	str.Format(L"RMove2::::dmc_set_s_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		//	DebugInfo2(str);
		//	return MCARD_ERROR;
		//}
	}


	//short dmc_pmove(WORD CardNo,WORD axis,long Dist,WORD posi_mode)
	//�� �ܣ�ָ�����λ�˶�
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//Dist Ŀ��λ�ã���λ��pulse
	//posi_mode �˶�ģʽ��0���������ģʽ��1����������ģʽ
	//����ֵ���������
	//ע �⣺���˶�ģʽΪ�������ģʽʱ��Ŀ��λ�ô��� 0 ʱ�����˶���С�� 0 ʱ�����˶�
	
	//��ʼ������״̬���������û�������˺����������GetAxisStatus����������������״̬
	A[uAxsIndex].AS.bMComplet = FALSE;
	A[uAxsIndex].AS.bEComplet = FALSE;


	rtn =dmc_pmove(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,lDist,0);
	if (rtn!=0)	
	{
		str.Format(L"RMove2:: :: dmc_pmove ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//�ȵ���һ�θ���һ��״̬
	GetAxisStatus(uAxsIndex);	

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::RMove(UINT uAxsIndex, long lDist)
{
	return RMove2(uAxsIndex, float(A[uAxsIndex].AP.fVel),lDist);
}
ERROR_RESULT CMCard::JogMove(UINT uAxsIndex, float fVel, BOOL bDir)
{
	int rtn=0;
	CString str;

	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"JogMove::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"JogMove::�����᲻���ڣ�");
		return MCARD_ERROR;
	}


	if (TRUE != A[uAxsIndex].AP.bAxsSvnEnable)
	{
		CString str;
		str.Format(L"JogMove::��%dʹ�ܹرղ����ƶ���",uAxsIndex);
		AfxMessageBox(str);
		DebugInfo(str);
		return MCARD_ERROR;
	}

	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
	{
		return MCARD_ERROR;
	}

	//rtn	= StopAxis(uAxsIndex,1);//��ֹͣ
	//if (rtn != MCARD_NO_ERROR) 
	//{
	//	DebugInfo(L"JogMove :: StopAxis ,����ʧ�ܣ���");
	//	return MCARD_ERROR;
	//}

	GetAxisStatus(uAxsIndex);	
	if (A[uAxsIndex].AS.bDone)
	{
		//short dmc_set_profile(WORD CardNo,WORD axis,double Min_Vel,double Max_Vel,double
		//Tacc,double Tdec,double Stop_Vel)
		//�� �ܣ����õ����˶��ٶ�����
		//�� ����CardNo ���ƿ�����
		//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
		//DMC5400A��0~3
		//Min_Vel ��ʼ�ٶȣ���λ��pulse/s (���ֵΪ 2M)
		//Max_Vel ����ٶȣ���λ��pulse/s (���ֵΪ 2M)
		//Tacc ����ʱ�䣬��λ��s (��СֵΪ 0.001s)
		//Tdec ����ʱ�䣬��λ��s (��СֵΪ 0.001s)
		//Stop_Vel ֹͣ�ٶȣ���λ��pulse/s (���ֵΪ 2M)
		//����ֵ���������
		//���õ����ٶ�S���߲���
		rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,double(A[uAxsIndex].AP.fStartVel) ,
			double(fVel),
			double(A[uAxsIndex].AP.fTAcc),double(A[uAxsIndex].AP.fTDec),double(A[uAxsIndex].AP.fStartVel));
		if (rtn != 0)
		{
			str.Format(L"RMove2::::dmc_set_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
		//short dmc_set_s_profile(WORD CardNo,WORD axis,WORD s_mode,double s_para)
		//	�� �ܣ����õ����ٶ����� S �β���ֵ
		//	�� ����CardNo ���ƿ�����
		//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
		//	DMC5400A��0~3
		//	s_mode �����������̶�ֵΪ 0
		//	DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
		//	�� 230 ��
		//	s_para S ��ʱ�䣬��λ��s����Χ��0~0.5 s
		//	����ֵ���������
		rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,double(A[uAxsIndex].AP.fTSmooth));
		if (rtn != 0)
		{
			str.Format(L"RMove2::::dmc_set_s_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}

		//short dmc_vmove(WORD CardNo,WORD axis,WORD dir)
		//�� �ܣ�ָ���������˶�
		//�� ����CardNo ���ƿ�����
		//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
		//DMC5400A��0~3
		//dir �˶�����0��������1��������
		//����ֵ���������
		//��ʼ������״̬���������û�������˺����������GetAxisStatus����������������״̬
		A[uAxsIndex].AS.bMComplet = FALSE;
		A[uAxsIndex].AS.bEComplet = FALSE;
		rtn =dmc_vmove(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,bDir);
		if (rtn!=0)	
		{
			str.Format(L"JogMove:: :: dmc_vmove ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}

	}
	else
	{
		//short dmc_change_speed(WORD CardNo,WORD axis,double Curr_Vel,double Taccdec)
		//�� �ܣ����߸ı�ָ����ĵ�ǰ�˶��ٶ�
		//DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
		//�� 231 ��
		//�� ����CardNo ���ƿ�����
		//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
		//DMC5400A��0~3
		//Curr_Vel �ı����˶��ٶȣ���λ��pulse/s
		//Taccdec �����������̶�ֵΪ 0
		//����ֵ���������
		//ע �⣺1���ú��������ڵ����˶��еı���
		//2������һ�������������Ĭ�������ٶȽ��ᱻ��дΪ Curr_Vel��Ҳ��������
		//dmc_get_profile �ض��ٶȲ���ʱ�ᷢ���� dmc_set_profile �����õĲ�һ�µ�
		//����
		//3���������˶��� Curr_Vel ��ֵ��ʾ��������٣���ֵ��ʾ��������١��ڵ�λ�˶�
		//�� Curr_Vel ֻ������ֵ

		rtn =dmc_change_speed(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,double(fVel),double(A[uAxsIndex].AP.fTDec));
		if (rtn!=0)	
		{
			str.Format(L"JogMove:: :: dmc_change_speed ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
	}
	//�ȵ���һ�θ���һ��״̬
	GetAxisStatus(uAxsIndex);	

	return MCARD_NO_ERROR;
}
//ERROR_RESULT CMCard::SetJogVel(UINT uAxsIndex, float fVel, BOOL bDir)
//{
//	int rtn=0;
//	CString str;
//
//	if (FALSE ==m_bMotionCardOpenning)
//	{
//		DebugInfo2(L"JogMove::�˶����ƿ���û�д򿪣���");
//		return MCARD_ERROR;
//	}
//	//������Ƿ����
//	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
//	{
//		DebugInfo2(L"JogMove::�����᲻���ڣ�");
//		return MCARD_ERROR;
//	}
//
//
//	if (TRUE != A[uAxsIndex].AP.bAxsSvnEnable)
//	{
//		CString str;
//		str.Format(L"JogMove::��%dʹ�ܹرղ����ƶ���",uAxsIndex);
//		AfxMessageBox(str);
//		DebugInfo(str);
//		return MCARD_ERROR;
//	}
//
//	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
//	{
//		return MCARD_ERROR;
//	}
//
//	rtn	= StopAxis(uAxsIndex,0);//��ֹͣ
//	if (rtn != MCARD_NO_ERROR) 
//	{
//		DebugInfo(L"JogMove :: StopAxis ,����ʧ�ܣ���");
//		return MCARD_ERROR;
//	}
//
//
//	//short dmc_vmove(WORD CardNo,WORD axis,WORD dir)
//	//�� �ܣ�ָ���������˶�
//	//�� ����CardNo ���ƿ�����
//	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
//	//DMC5400A��0~3
//	//dir �˶�����0��������1��������
//	//����ֵ���������
//	//��ʼ������״̬���������û�������˺����������GetAxisStatus����������������״̬
//	A[uAxsIndex].AS.bMComplet = FALSE;
//	A[uAxsIndex].AS.bEComplet = FALSE;
//	rtn =dmc_vmove(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,bDir);
//	if (rtn!=0)	
//	{
//		str.Format(L"JogMove:: :: dmc_vmove ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
//		DebugInfo2(str);
//		return MCARD_ERROR;
//	}
//	//short dmc_change_speed(WORD CardNo,WORD axis,double Curr_Vel,double Taccdec)
//	//�� �ܣ����߸ı�ָ����ĵ�ǰ�˶��ٶ�
//	//DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
//	//�� 231 ��
//	//�� ����CardNo ���ƿ�����
//	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
//	//DMC5400A��0~3
//	//Curr_Vel �ı����˶��ٶȣ���λ��pulse/s
//	//Taccdec �����������̶�ֵΪ 0
//	//����ֵ���������
//	//ע �⣺1���ú��������ڵ����˶��еı���
//	//2������һ�������������Ĭ�������ٶȽ��ᱻ��дΪ Curr_Vel��Ҳ��������
//	//dmc_get_profile �ض��ٶȲ���ʱ�ᷢ���� dmc_set_profile �����õĲ�һ�µ�
//	//����
//	//3���������˶��� Curr_Vel ��ֵ��ʾ��������٣���ֵ��ʾ��������١��ڵ�λ�˶�
//	//�� Curr_Vel ֻ������ֵ
//
//	rtn =dmc_change_speed(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,double(fVel),double(A[uAxsIndex].AP.fTDec));
//	if (rtn!=0)	
//	{
//		str.Format(L"JogMove:: :: dmc_change_speed ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
//		DebugInfo2(str);
//		return MCARD_ERROR;
//	}
//
//	//�ȵ���һ�θ���һ��״̬
//	GetAxisStatus(uAxsIndex);	
//
//	return MCARD_NO_ERROR;
//}
ERROR_RESULT CMCard::AMove2(UINT uAxsIndex, float fVel,long lPos)
{
	int rtn=0;
	CString str;


	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"AMove2::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"AMove2::�����᲻���ڣ�");
		return MCARD_ERROR;
	}


	if (TRUE != A[uAxsIndex].AP.bAxsSvnEnable)
	{
		CString str;
		str.Format(L"AMove2::��%dʹ�ܹرղ����ƶ���",uAxsIndex);
		AfxMessageBox(str);
		DebugInfo(str);
		return MCARD_ERROR;
	}
	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
	{
		return MCARD_ERROR;
	}

	rtn	= StopAxis(uAxsIndex,0);//��ֹͣ
	if (rtn != MCARD_NO_ERROR) 
	{
		DebugInfo(L"RMove2 :: StopAxis ,����ʧ�ܣ���");
		return MCARD_ERROR;
	}


	//short dmc_set_profile(WORD CardNo,WORD axis,double Min_Vel,double Max_Vel,double
	//Tacc,double Tdec,double Stop_Vel)
	//�� �ܣ����õ����˶��ٶ�����
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//Min_Vel ��ʼ�ٶȣ���λ��pulse/s (���ֵΪ 2M)
	//Max_Vel ����ٶȣ���λ��pulse/s (���ֵΪ 2M)
	//Tacc ����ʱ�䣬��λ��s (��СֵΪ 0.001s)
	//Tdec ����ʱ�䣬��λ��s (��СֵΪ 0.001s)
	//Stop_Vel ֹͣ�ٶȣ���λ��pulse/s (���ֵΪ 2M)
	//����ֵ���������
	//���õ����ٶ�S���߲���
	rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,double(A[uAxsIndex].AP.fStartVel) ,
		double(fVel),
		double(A[uAxsIndex].AP.fTAcc),double(A[uAxsIndex].AP.fTDec),double(A[uAxsIndex].AP.fStartVel));
	if (rtn != 0)
	{
		str.Format(L"AMove2::::dmc_set_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//short dmc_set_s_profile(WORD CardNo,WORD axis,WORD s_mode,double s_para)
	//	�� �ܣ����õ����ٶ����� S �β���ֵ
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	s_mode �����������̶�ֵΪ 0
	//	DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
	//	�� 230 ��
	//	s_para S ��ʱ�䣬��λ��s����Χ��0~0.5 s
	//	����ֵ���������
	rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,double(A[uAxsIndex].AP.fTSmooth));
	if (rtn != 0)
	{
		str.Format(L"AMove2::::dmc_set_s_profile ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//short dmc_pmove(WORD CardNo,WORD axis,long Dist,WORD posi_mode)
	//�� �ܣ�ָ�����λ�˶�
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//Dist Ŀ��λ�ã���λ��pulse
	//posi_mode �˶�ģʽ��0���������ģʽ��1����������ģʽ
	//����ֵ���������
	//ע �⣺���˶�ģʽΪ�������ģʽʱ��Ŀ��λ�ô��� 0 ʱ�����˶���С�� 0 ʱ�����˶�

	//��ʼ������״̬���������û�������˺����������GetAxisStatus����������������״̬
	A[uAxsIndex].AS.bMComplet = FALSE;
	A[uAxsIndex].AS.bEComplet = FALSE;
	rtn =dmc_pmove(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,lPos,1);
	if (rtn!=0)	
	{
		str.Format(L"AMove2:: :: dmc_pmove ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//�ȵ���һ�θ���һ��״̬
	GetAxisStatus(uAxsIndex);	

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::AMove(UINT uAxsIndex, long lPos)
{
	return AMove2(uAxsIndex, float(A[uAxsIndex].AP.fVel), lPos);
}
////������ŷֽ�Ϊ����Ŀ��ţ����
//ERROR_RESULT CMCard::BeAxisExist(UINT uAxsIndex ,short* sCardNo,short* sAxisNo)
//{
//	int rtn=0;
//	CString str;
//	short sTmpCardNo = short( uAxsIndex / AXIS_COUNT_MAX );
//	short sTmpAxisNo = short( uAxsIndex % AXIS_COUNT_MAX );
//	if (sTmpCardNo < m_nActualCardCount )
//	{//������
//		DWORD dwAxisCount=0;
//		rtn = dmc_get_total_axes(WORD(sTmpCardNo),&dwAxisCount);
//		if (rtn != 0)
//		{
//			str.Format(L"BeAxisExist::dmc_get_total_axes ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
//			DebugInfo2(str);
//			return MCARD_ERROR;
//		}
//		if (sTmpAxisNo < dwAxisCount)
//		{//�����
//			*sCardNo = sTmpCardNo;
//			*sAxisNo = sTmpAxisNo;
//				
//			return MCARD_NO_ERROR;
//		}
//		else
//		{
//			//���᲻���ڣ�������������
//			DebugInfo2(L"BeAxisExist::���᲻���ڣ���");
//			return MCARD_ERROR;
//		}
//	}
//	else
//	{
//		DebugInfo2(L"BeAxisExist:: ���Ų����ڣ���");
//		return MCARD_ERROR;
//	}
//}

//ָ����ֹͣ�˶�stop_mode �ƶ���ʽ��0������ֹͣ��1������ֹͣ
ERROR_RESULT CMCard::StopAxis(UINT uAxsIndex, UINT uMode)
{	
	int rtn=0;
	CString str;
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"StopAxis::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"StopAxis::�����᲻���ڣ�");
		return MCARD_ERROR;
	}

	rtn = dmc_stop(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,  uMode);
	if (rtn != 0)
	{
		str.Format(L"StopAxis::dmc_stop ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::EnableAxis(UINT uAxsIndex, BOOL bYes)
{
	int rtn=0;
	CString str;
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"EnableAxis::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"EnableAxis::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	//short dmc_write_sevon_pin(WORD CardNo,WORD axis,WORD on_off)
	//�� �ܣ�����ָ������ŷ�ʹ�ܶ˿ڵ����
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//DMC5400A��0~3
	//on_off �����ŷ�ʹ�ܶ˿ڵ�ƽ��0���͵�ƽ��1���ߵ�ƽ
	//����ֵ���������

	///****�������д����
	//if ( bYes==1)
	//{
	//	rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsSvnEnablePinLogic?0:1);//GT_AxisOn(uAxisNo+1);
	//	if (rtn != 0) 
	//	{
	//		str.Format(L"EnableAxis::dmc_write_sevon_pin ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
	//		DebugInfo2(str);
	//		return MCARD_ERROR;
	//	}
	//}
	//else
	//{
	//	rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsSvnEnablePinLogic?1:0);//GT_AxisOn(uAxisNo+1);
	//	if (rtn != 0) 
	//	{
	//		str.Format(L"EnableAxis::dmc_write_sevon_pin ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
	//		DebugInfo2(str);
	//		return MCARD_ERROR;
	//	}
	//}
	if ( bYes==1)
	{
		rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsSvnEnablePinLogic);//GT_AxisOn(uAxisNo+1);
		if (rtn != 0) 
		{
			str.Format(L"EnableAxis::dmc_write_sevon_pin ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
	}
	else
	{
		rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, !A[uAxsIndex].AP.nAxsSvnEnablePinLogic);//GT_AxisOn(uAxisNo+1);
		if (rtn != 0) 
		{
			str.Format(L"EnableAxis::dmc_write_sevon_pin ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
	}



	A[uAxsIndex].AP.bAxsSvnEnable=bYes;
	
	//������ô������ʹ�ܣ�����λ״̬������Ϊ0
	A[uAxsIndex].AS.bHome=0;

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::SetDoBit(UINT uIOIndex, UINT uSetValue)
{
	if(FALSE == DebugSupperDoSafeFilter(uIOIndex, uSetValue))
	{
		return MCARD_ERROR;
	}

	int rtn=0;
	CString str;
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetDoBit::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}


	if(O[uIOIndex].sNodeNo == 0)//�˶����ƿ������ϵ����
	{

		//short dmc_write_outbit(WORD CardNo, WORD bitno,WORD on_off)
		//	�� �ܣ�����ָ�����ƿ���ĳ������˿ڵĵ�ƽ
		//	�� ����CardNo ���ƿ�����
		//	bitno ����˿ںţ�ȡֵ��Χ��DMC5410��0~31��DMC5800/5600/5400A��0~15
		//	on_off �����ƽ��0���͵�ƽ��1���ߵ�ƽ
		//	����ֵ���������
		rtn	 = dmc_write_outbit((WORD)O[uIOIndex].sCardNo,(WORD)O[uIOIndex].sPortNo,(WORD)(uSetValue?0:1));
		if (rtn!=0)	
		{
			str.Format(L"SetDoBit:: :: dmc_write_outbit ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
		//Sleep(1);
		////short dmc_read_outbit(WORD CardNo, WORD bitno)
		////	�� �ܣ���ȡָ�����ƿ���ĳ������˿ڵĵ�ƽ
		////	�� ����CardNo ���ƿ�����
		////	bitno ����˿ںţ�ȡֵ��Χ��DMC5410��0~31��DMC5800/5600/5400A��0~15
		////	����ֵ��ָ������˿ڵĵ�ƽ��0���͵�ƽ��1���ߵ�ƽ
		//rtn = dmc_read_outbit((WORD)O[uIOIndex].sCardNo,(WORD)O[uIOIndex].sPortNo);
		//if (rtn != (int)(BSetValue?0:1) )
		//{
		//	str.Format(L"SetDoBit:: :: dmc_write_outbit  ����ʧ�� ���� dmc_read_outbit����ʧ�� ����");
		//	DebugInfo2(str);
		//	return MCARD_ERROR;
		//}
	}
	else
	{
		//short dmc_get_can_state(WORD CardNo,WORD* NodeNum,WORD* state)
		//�� �ܣ���ȡ CAN-IO ͨѶ״̬��DMC5800/5600/5400A ��ר��
		//�� ����CardNo ���ƿ�����
		//NodeNum ���� CAN �ڵ���
		//state ���� CAN-IO ͨѶ״̬��0���Ͽ���1�����ӣ�2���쳣
		//����ֵ���������
			
		//short dmc_write_can_outbit(WORD CardNo,WORD Node,WORD bitno,WORD on_off)
		//�� �ܣ�����ָ�� CAN-IO ��չģ���ĳ������˿ڵĵ�ƽ��DMC5800/5600/5400A ��ר��
		//�� ����CardNo ���ƿ�����
		//Node CAN �ڵ�ţ�ȡֵ��Χ��1~8
		//bitno ����˿ںţ�ȡֵ��Χ��0~15
		//on_off �����ƽ��0���͵�ƽ��1���ߵ�ƽ
		//����ֵ���������
		rtn	 = dmc_write_can_outbit((WORD) O[uIOIndex].sCardNo , (WORD)O[uIOIndex].sNodeNo,(WORD)O[uIOIndex].sPortNo,(WORD)(uSetValue?0:1));
		if (rtn!=0)	
		{
			str.Format(L"SetDoBit:: :: dmc_write_outbit ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
		//Sleep(1);
		////short dmc_read_can_outbit(WORD CardNo,WORD Node,WORD bitno)
		////�� �ܣ���ȡָ�� CAN-IO ��չģ���ĳ������˿ڵĵ�ƽ��DMC5800/5600/5400A ��ר��
		////�� ����CardNo ���ƿ�����
		////Node CAN �ڵ�ţ�ȡֵ��Χ��1~8
		////bitno ����˿ںţ�ȡֵ��Χ��0~15
		////����ֵ��ָ������˿ڵĵ�ƽ��0���͵�ƽ��1���ߵ�ƽ

		//rtn = dmc_read_can_outbit((WORD)O[uIOIndex].sCardNo, (WORD)O[uIOIndex].sNodeNo, (WORD)O[uIOIndex].sPortNo);
		//if (rtn != (int)( BSetValue ? 0 : 1 ) )
		//{
		//	str.Format(L"SetDoBit:: :: dmc_write_can_outbit  ����ʧ�� ���� dmc_read_can_outbit����ʧ�� ����");
		//	DebugInfo2(str);
		//	return MCARD_ERROR;
		//}

	}
	O[uIOIndex].sValue	= uSetValue;

	return MCARD_NO_ERROR;
}
//BIndex����˿ں� �� BSetValue  Ҫ���ֵ����״̬ 0��1 �� sTime  ����ʱ�� ms
ERROR_RESULT CMCard::SetDoBitReverse(UINT uIOIndex, UINT uSetValue,short sTime)
{
	int rtn=0;
	CString str;
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetDoBit::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}
	if(FALSE == DebugSupperDoSafeFilter(uIOIndex, uSetValue))
	{
		return MCARD_ERROR;
	}

	if (O[uIOIndex].sValue == (short)uSetValue)
	{
		//���ֵ�״̬�����һ�£�ֱ�ӷ���
		return MCARD_NO_ERROR;
	}
	else 
	{
		if (sTime<0)
		{
			return MCARD_NO_ERROR;
		}
		if(O[uIOIndex].sNodeNo == 0)//�˶����ƿ������ϵ����
		{
			//short dmc_write_outbit(WORD CardNo, WORD bitno,WORD on_off)
			//	�� �ܣ�����ָ�����ƿ���ĳ������˿ڵĵ�ƽ
			//	�� ����CardNo ���ƿ�����
			//	bitno ����˿ںţ�ȡֵ��Χ��DMC5410��0~31��DMC5800/5600/5400A��0~15
			//	on_off �����ƽ��0���͵�ƽ��1���ߵ�ƽ
			//	����ֵ���������
			rtn	 = dmc_write_outbit((WORD)O[uIOIndex].sCardNo,(WORD)O[uIOIndex].sPortNo,(WORD)(0?0:1));
			if (rtn!=0)	
			{
				str.Format(L"SetDoBit:: :: dmc_write_outbit ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//short dmc_reverse_outbit(WORD CardNo,WORD bitno,double reverse_time)
			//	�� �ܣ�IO �����ʱ��ת
			//	�� ����CardNo ���ƿ�����
			//	bitno ����˿ںţ�ȡֵ��Χ��0~15
			//	reverse_time ��ʱ��תʱ�䣬��λ��s
			//	����ֵ���������
			//	ע �⣺1���ú���ֻ�ܶ� OUT0~15 �˿ڽ��в���
			//	2������ʱ��תʱ���������Ϊ 0 ʱ����ʱ��ʱ��תʱ�佫Ϊ���޴�
			rtn	 = dmc_reverse_outbit((WORD)O[uIOIndex].sCardNo,(WORD)O[uIOIndex].sPortNo,(double)(double(sTime)/1000.00));
			if (rtn!=0)	
			{
				str.Format(L"SetDoBitReverse:: :: dmc_reverse_outbit ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
		}
		else
		{
			rtn	 = dmc_write_can_outbit((WORD) O[uIOIndex].sCardNo , (WORD)O[uIOIndex].sNodeNo,(WORD)O[uIOIndex].sPortNo,(WORD)(uSetValue?0:1));
			if (rtn!=0)	
			{
				str.Format(L"SetDoBit:: :: dmc_write_outbit ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			O[uIOIndex].sValue	= uSetValue;
			/////////////////////
			Sleep(sTime);
			/////////////////////

			rtn	 = dmc_write_can_outbit((WORD) O[uIOIndex].sCardNo , (WORD)O[uIOIndex].sNodeNo,(WORD)O[uIOIndex].sPortNo,(WORD)uSetValue);
			if (rtn!=0)	
			{
				str.Format(L"SetDoBit:: :: dmc_write_outbit ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			O[uIOIndex].sValue	= short(uSetValue?0:1);
		}
	}
	return MCARD_NO_ERROR;
}

ERROR_RESULT CMCard::GetDiBit(UINT uIOIndex)
{
	int rtn=0;
	CString str;
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetDoBit::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}


	if(I[uIOIndex].sNodeNo == 0)//�˶����ƿ������ϵ����
	{

		//short dmc_read_inbit(WORD CardNo, WORD bitno)
		//	�� �ܣ���ȡָ�����ƿ���ĳ������˿ڵĵ�ƽ
		//	�� ����CardNo ���ƿ�����
		//	bitno ����˿ںţ�ȡֵ��Χ��DMC5410��0~31��DMC5800/5600/5400A��0~15
		//	����ֵ��ָ��������˿ڵ�ƽ��0���͵�ƽ��1���ߵ�ƽ
		//	ע�⣺DMC5400A �� OUT12��0UT13 �������Ч
		rtn	 = dmc_read_inbit((WORD)I[uIOIndex].sCardNo,(WORD)I[uIOIndex].sPortNo);

	}
	else
	{
		//short dmc_read_can_inbit(WORD CardNo,WORD Node,WORD bitno)
		//	�� �ܣ���ȡָ�� CAN-IO ��չģ���ĳ������˿ڵĵ�ƽ��DMC5800/5600/5400A ��ר��
		//	�� ����CardNo ���ƿ�����
		//	Node CAN �ڵ�ţ�ȡֵ��Χ��1~8
		//	bitno ����˿ںţ�ȡֵ��Χ��0~15
		//	����ֵ��ָ������˿ڵĵ�ƽ��0���͵�ƽ��1���ߵ�ƽ
		rtn	 = dmc_read_can_inbit((WORD) O[uIOIndex].sCardNo , (WORD)O[uIOIndex].sNodeNo,(WORD)O[uIOIndex].sPortNo);
	}
	
	I[uIOIndex].sValue	= rtn? 0:1 ;
	
	return MCARD_NO_ERROR;
}

ERROR_RESULT CMCard::GetAllDi()
{
	DWORD rtn=0;
	CString str;
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetDoBit::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	for (WORD c = 0; c < MCARD_COUNT_MAX ; c++ )
	{
		for (WORD n = 0; n < (1+EX_IONODE_COUNT_MAX) ; n++ )
		{
			if (n == 0)
			{
				//DWORD dmc_read_inport(WORD CardNo,WORD portno)
				//	�� �ܣ���ȡָ�����ƿ���ȫ������˿ڵĵ�ƽ
				//	�� ����CardNo ���ƿ�����
				//	portno IO ��ţ�ȡֵ��Χ��DMC5410��0��DMC5800/5600/5400A��0��1
				//	����ֵ��DMC5410��bit0~bit31 ֵ�ֱ����� 0~31 ������˿ڵĵ�ƽ
				//	DMC5800/5600/5400A��bit0~bit31 �Ķ������ 9.3
				//	�� 9.3 DMC5800/5600/5400A �� dmc_read_inport ��������ֵ��λ�Ķ����
				//	�� 0 �飨portno ������ �� 1 �飨portno ������
				//	��������ֵ�� bit ����ں� ��������� ��������ֵ�� bit ����ں� ���������
				rtn	 = dmc_read_inport(c,0);
			}
			else
			{
				//DWORD dmc_read_can_inport(WORD CardNo,WORD Node,WORD PortNo)
				//	�� �ܣ���ȡָ�� CAN-IO ��չģ���ȫ������˿ڵĵ�ƽ��DMC5800/5600/5400A ��ר��
				//	�� ����CardNo ���ƿ�����
				//	Node CAN �ڵ�ţ�ȡֵ��Χ��1~8
				//	PortNo �����������̶�ֵΪ 0
				//	����ֵ��bit0~bit15 ��ֵ�ֱ����� 0~15 ������˿ڵĵ�ƽ
				rtn	 = dmc_read_can_inport(c,n,0);
			}
			for (WORD p =0; p < 16; p++)
			{
				int n1MainCardPorts =	(( 1 + EX_IONODE_COUNT_MAX ) * INPUT_PORT_COUNT_MAX);
				int n1NodePorts		= INPUT_PORT_COUNT_MAX;
				int i =(c * n1MainCardPorts ) + (n * n1NodePorts) + p;
				I[i].sValue =((~rtn)>>I[i].sPortNo) & DWORD(1);	
			}
		}
	}
	return MCARD_NO_ERROR;
}

//��ȡ����˿�״̬     Add in 2019.3.3 By Gogol
ERROR_RESULT CMCard::GetAllDo()
{
	DWORD rtn = 0;
	CString str;
	if (FALSE == m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetDoBit::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	for (WORD c = 0; c < MCARD_COUNT_MAX; c++)
	{
		for (WORD n = 0; n < (1 + EX_IONODE_COUNT_MAX); n++)
		{
			if (n == 0)
			{
				//DWORD dmc_read_outport(WORD CardNo,WORD portno)
				//	�� �ܣ���ȡָ�����ƿ���ȫ������˿ڵĵ�ƽ
				//	�� ����CardNo ���ƿ�����
				//	portno IO ��ţ������������̶�ֵλ0
				//	����ֵ��DMC5410��bit0~bit31 ֵ�ֱ����� 0~31 ������˿ڵĵ�ƽ
				//	DMC5C00/5800/5600/5400A��bit0~bit31 �Ķ������ 9.4
				//	��������ֵ�� bit ����ں� ��������� ��������ֵ�� bit ����ں� ���������
				rtn = dmc_read_outport(c, 0);
			}
			else
			{
				//DWORD dmc_read_can_outport(WORD CardNo,WORD Node,WORD PortNo)
				//	�� �ܣ���ȡָ�� CAN-IO ��չģ���ȫ������˿ڵĵ�ƽ��DMC5800/5600/5400A ��ר��
				//	�� ����CardNo ���ƿ�����
				//	Node CAN �ڵ�ţ�ȡֵ��Χ��1~8
				//	PortNo �����������̶�ֵΪ 0
				//	����ֵ��bit0~bit15 ��ֵ�ֱ����� 0~15 ������˿ڵĵ�ƽ
				rtn = dmc_read_can_outport(c, n, 0);
			}
			for (WORD p = 0; p < 16; p++)
			{
				int n1MainCardPorts = ((1 + EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX);
				int n1NodePorts = OUTPUT_PORT_COUNT_MAX;
				int i = (c * n1MainCardPorts) + (n * n1NodePorts) + p;
				O[i].sValue = ((~rtn) >> O[i].sPortNo) & DWORD(1);    //λ�ǣ�����
			}
		}
	}
	return MCARD_NO_ERROR;
}

//�����ᰲȫ�ص�����
BOOL CMCard::SetAxsSafeFilter(BOOL (CALLBACK* lpfnAxsSafeFilter )(UINT))
{
	m_pAxsSafeFilter = lpfnAxsSafeFilter;
	return TRUE;
}
BOOL CMCard::DebugSupperAxsSafeFilter(UINT uAxisNo)
{
	if( m_pAxsSafeFilter ==NULL ) 
		return TRUE;
	else
	{	
		//return ((ASFR)m_pAxsSafeFilter)(uAxisNo);
		return (*m_pAxsSafeFilter)(uAxisNo);
	}
	return TRUE;
}


//����IO��ȫ�ص�����
BOOL CMCard::SetDoSafeFilter(BOOL (CALLBACK* lpfnDoSafeFilter )(UINT,UINT))
{
	m_pDoSafeFilter = lpfnDoSafeFilter;
	return TRUE;
}
BOOL CMCard::DebugSupperDoSafeFilter(UINT uIOIndex, UINT uValue)
{
	if(m_pDoSafeFilter==NULL) 
		return TRUE;
	else
	{	
		return (*m_pDoSafeFilter)(uIOIndex,uValue);
	}
	return TRUE;
}

char* CMCard::wc2c(CString &value)
{
	int iSize;
	char* pszMultiByte; 
	iSize=WideCharToMultiByte(CP_ACP,0,(LPCTSTR)value,-1,NULL,0,NULL,NULL);
	pszMultiByte=(char*)malloc(iSize+1);
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)value,-1,pszMultiByte,iSize,NULL,NULL);
	return pszMultiByte;
}

ERROR_RESULT CMCard::StartHomeLatch(UINT uAxsIndex)
{
	
	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"HomeAxis1::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"HomeAxis1::�����᲻���ڣ�");
		return MCARD_ERROR;
	}

	//short dmc_set_homelatch_mode(WORD CardNo,WORD axis,WORD enable,WORD logic,
	//WORD source)
	//�� �ܣ�����ԭ������ģʽ��DMC5800/5600/5400A ��ר��
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5800��0~7��DMC5600��0~5,DMC5400A��0~3
	//enable ԭ������ʹ�ܣ�0����ֹ��1������
	//logic ������ʽ��0���½��أ�1��������
	//source λ��Դѡ��0��ָ��λ�ü�������1��������������
	//����ֵ���������
	short rtn = dmc_set_homelatch_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,1,0 ,0);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"StartHomeLatch::::dmc_set_homelatch_mode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	//short dmc_reset_homelatch_flag(WORD CardNo,WORD axis)
	//�� �ܣ����ԭ�������־��DMC5800/5600/5400A ��ר��
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5800��0~7��DMC5600��0~5,DMC5400A��0~3
	//����ֵ���������
	rtn = dmc_reset_homelatch_flag(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"StartHomeLatch::::dmc_reset_homelatch_flag ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::GetHomeLatchSts(UINT uAxsIndex,BOOL &bSucceed, long &lPos)
{
	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"GetHomeLatchSts::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"GetHomeLatchSts::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	//long dmc_get_homelatch_flag(WORD CardNo,WORD axis)
	//�� �ܣ���ȡԭ�������־��DMC5800/5600/5400A ��ר��
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5800��0~7��DMC5600��0~5,DMC5400A��0~3
	//����ֵ��ԭ�������־��0��δ���棬1������
	bSucceed=dmc_get_homelatch_flag(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if(bSucceed)
	{
		//long dmc_get_homelatch_value(WORD CardNo,WORD axis)
		//�� �ܣ���ȡԭ������ֵ��DMC5800/5600/5400A ��ר��
		//�� ����CardNo ���ƿ�����
		//axis ָ����ţ�ȡֵ��Χ��DMC5800��0~7��DMC5600��0~5,DMC5400A��0~3
		//����ֵ������ֵ����λ��pulse
		lPos=dmc_get_homelatch_flag(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	}

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::ResetHomeLatchSts(UINT uAxsIndex)
{
	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"ResetHomeLatchSts::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"ResetHomeLatchSts::�����᲻���ڣ�");
		return MCARD_ERROR;
	}

	//short dmc_reset_homelatch_flag(WORD CardNo,WORD axis)
	//�� �ܣ����ԭ�������־��DMC5800/5600/5400A ��ר��
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5800��0~7��DMC5600��0~5,DMC5400A��0~3
	//����ֵ���������
	short rtn = dmc_reset_homelatch_flag(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"ResetHomeLatchSts::::dmc_reset_homelatch_flag ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::EndHomeLatch(UINT uAxsIndex)
{
	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"EndHomeLatch::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"EndHomeLatch::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	
	//short dmc_reset_homelatch_flag(WORD CardNo,WORD axis)
	//�� �ܣ����ԭ�������־��DMC5800/5600/5400A ��ר��
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5800��0~7��DMC5600��0~5,DMC5400A��0~3
	//����ֵ���������
	short rtn = dmc_reset_homelatch_flag(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"EndHomeLatch::::dmc_reset_homelatch_flag ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//short dmc_set_homelatch_mode(WORD CardNo,WORD axis,WORD enable,WORD logic,
	//WORD source)
	//�� �ܣ�����ԭ������ģʽ��DMC5800/5600/5400A ��ר��
	//�� ����CardNo ���ƿ�����
	//axis ָ����ţ�ȡֵ��Χ��DMC5800��0~7��DMC5600��0~5,DMC5400A��0~3
	//enable ԭ������ʹ�ܣ�0����ֹ��1������
	//logic ������ʽ��0���½��أ�1��������
	//source λ��Դѡ��0��ָ��λ�ü�������1��������������
	//����ֵ���������
	rtn = dmc_set_homelatch_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,0,0);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"EndHomeLatch::::dmc_set_homelatch_mode ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}


//һά����λ�ñȽ�10KHz//*************************************************************************************
//����ָ�����һάλ�ñȽ�	
ERROR_RESULT CMCard::StartCompare(UINT uAxsIndex,UINT uCmpSrc)
{
	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"StartCompare::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"StartCompare::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	//short dmc_compare_set_config(WORD CardNo, WORD axis,WORD enable, WORD cmp_source)
	//	�� �ܣ�����һάλ�ñȽ���
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	enable �ȽϹ���״̬��0����ֹ��1��ʹ��
	//	cmp_source �Ƚ�Դ��0��ָ��λ�ü�������1��������������
	//	����ֵ���������
	int rtn = dmc_compare_set_config(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, WORD(1), WORD(uCmpSrc));
	if(rtn != 0)
	{
		CString str;
		str.Format(L"StartCompare::::dmc_compare_set_config ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	CS[uAxsIndex].bEnable =1;
	CS[uAxsIndex].uCmpSource = uCmpSrc;

	return ClearAllComparePoints(uAxsIndex);
}

//���ָ���������һάλ�ñȽϵ�
ERROR_RESULT CMCard::ClearAllComparePoints(UINT uAxsIndex)
{
	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"ClearAllComparePoints::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"ClearAllComparePoints::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	//short dmc_compare_clear_points(WORD CardNo,WORD axis)
	//	�� �ܣ��������ӵ�����һάλ�ñȽϵ�
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	����ֵ���������
	int rtn = dmc_compare_clear_points(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"ClearAllComparePoints::::dmc_compare_clear_points ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	CS[uAxsIndex].lCmpNum=0;

	return MCARD_NO_ERROR;
}	

//���һ���µ�һάλ�ñȽϵ�
//uCmpDir  =0 �趨λ��С�ڵ����ᵱǰλ��ʱ�����������תʱ��, =1�趨λ�ô��ڵ����ᵱǰλ��ʱ�����������תʱ��
//uFunNo;	//uOutPortNo;
//������	//����ں�                 ����Ҫ��ָ�����ϵİɣ���ȷ����...��
//1			IO ��	IO ��Ϊ�ߵ�ƽ
//2			IO ��	IO ��Ϊ�͵�ƽ
//3			IO ��	ȡ�� IO
//5			IO ��	��� 500us ����
//6			IO ��	��� 1ms ����
//7			IO ��	��� 10ms ����
//8			IO ��	��� 100ms ����
//13		���	ָֹͣ����
ERROR_RESULT CMCard::AddNewComparePoint(UINT uAxsIndex,UINT uCmpDir,LONG lNewPointPos,UINT	uFunNo,UINT	uOutPortNo)
{
	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"AddNewComparePoint::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"AddNewComparePoint::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	
	if(MCARD_ERROR == GetCompareSts(uAxsIndex,GETNO_CAN_ADD_NUM))
	{	
		DebugInfo2(L"AddNewComparePoint::GetCompareSts ����ʧ�ܣ�");
		return MCARD_ERROR;
	}
	else if( CS[uAxsIndex].lCanAddNum<=0 )
	{	
		DebugInfo2(L"AddNewComparePoint::�Ƚϻ��������������������µıȽϵ㣡");
		return MCARD_ERROR;
	}

	//short dmc_compare_add_point(WORD CardNo,WORD axis, long pos, WORD dir, WORD action,
	//	DWORD actpara)
	//	�� �ܣ����һάλ�ñȽϵ�
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
	//	�� 251 ��
	//	DMC5400A��0~3
	//	pos �Ƚ�λ�ã���λ��pulse
	//	dir �Ƚ�ģʽ��0��С�ڵ��ڣ�1�����ڵ���
	//	action �Ƚϵ㴥�����ܱ�ţ����� 9.5
	//	actpara �Ƚϵ㴥�����ܲ��������� 9.5
	//	����ֵ���������
	int rtn = dmc_compare_add_point(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,lNewPointPos,WORD(uCmpDir),WORD(uFunNo),DWORD(uOutPortNo));
	if(rtn != 0)
	{
		CString str;
		str.Format(L"AddNewComparePoint::dmc_compare_add_point ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}	

//��ѯ���µ�ǰ��Ƚ�����״̬��
//uGetNo	
//GETNO_ABLE_SRC	=0	;//�ȽϿ���״̬���Ƚ�Դ 
//GETNO_CUR_POS		=1	;//��ǰ�Ƚϵ�λ�� 
//GETNO_CMP_NUM		=2	;//�Ѿ��ȽϹ��ĵ���
//GETNO_CAN_ADD_NUM	=3	;//���Լ���ıȽϵ���
ERROR_RESULT CMCard::GetCompareSts(UINT uAxsIndex,UINT uGetNo)
{
	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"GetCompareSts::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"GetCompareSts::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	switch(uGetNo)
	{
	case 0:
		{
			//short dmc_compare_get_config(WORD CardNo, WORD axis,WORD* enable,WORD* cmp_source)
			//	�� �ܣ���ȡһάλ�ñȽ�������
			//	�� ����CardNo ���ƿ�����
			//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
			//	DMC5400A��0~3
			//	enable ���رȽϹ���״̬
			//	cmp_source ���رȽ�Դ
			//	����ֵ���������
			WORD enable=0;
			WORD cmp_source=0;
			int rtn = dmc_compare_get_config(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,&enable,&cmp_source);
			if(rtn != 0)
			{
				CString str;
				str.Format(L"GetCompareSts::dmc_compare_get_config ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			CS[uAxsIndex].bEnable	= BOOL(enable);
			CS[uAxsIndex].uCmpSource=UINT(cmp_source);
		}
		break;
	case 1:
		{
			//short dmc_compare_get_current_point(WORD CardNo,WORD axis,long* pos)
			//	�� �ܣ���ȡ��ǰһά�Ƚϵ�λ��
			//	�� ����CardNo ���ƿ�����
			//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
			//	DMC5400A��0~3
			//	pos ���ص�ǰ�Ƚϵ�λ�ã���λ��pulse
			//	����ֵ���������
			long pos=0;
			int rtn = dmc_compare_get_current_point(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,&pos);
			if(rtn != 0)
			{
				CString str;
				str.Format(L"GetCompareSts::dmc_compare_get_current_point ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			CS[uAxsIndex].lCurPos=pos;
		}
		break;
	case 2:
		{
			//short dmc_compare_get_points_runned(WORD CardNo,WORD axis,long* PointNum)
			//	�� �ܣ���ѯ�Ѿ��ȽϹ���һά�Ƚϵ����
			//	�� ����CardNo ���ƿ�����
			//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
			//	DMC5400A��0~3
			//	PointNum �����Ѿ��ȽϹ��ĵ���
			//	����ֵ���������
			long PointNum=0;
			int rtn = dmc_compare_get_points_runned(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,&PointNum);
			if(rtn != 0)
			{
				CString str;
				str.Format(L"GetCompareSts::dmc_compare_get_points_runned ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			CS[uAxsIndex].lCmpNum=PointNum;
		}
		break;
	case 3:
		{
			//short dmc_compare_get_points_remained(WORD CardNo,WORD axis,long* PointNum)
			//	�� �ܣ���ѯ���Լ����һά�Ƚϵ����
			//	DMC5000 ϵ�п��û�ʹ���ֲ� V1.0
			//	�� 252 ��
			//	�� ����CardNo ���ƿ�����
			//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
			//	DMC5400A��0~3
			//	PointNum ���ؿ��Լ���ıȽϵ���
			//	����ֵ��s�������
			long PointNum=0;
			int rtn = dmc_compare_get_points_remained(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,&PointNum);
			if(rtn != 0)
			{
				CString str;
				str.Format(L"GetCompareSts::dmc_compare_get_points_remained ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			CS[uAxsIndex].lCanAddNum=PointNum;		
		}
		break;
	}
	return MCARD_NO_ERROR;
}
//�ر�ָ�����һάλ�ñȽ�
ERROR_RESULT CMCard::EndCompare(UINT uAxsIndex)
{
	//�˶����ƿ��򿪼��
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"EndCompare::�˶����ƿ���û�д򿪣���");
		return MCARD_ERROR;
	}

	//������Ƿ����
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1��ʾ������
	{
		DebugInfo2(L"EndCompare::�����᲻���ڣ�");
		return MCARD_ERROR;
	}
	//short dmc_compare_set_config(WORD CardNo, WORD axis,WORD enable, WORD cmp_source)
	//	�� �ܣ�����һάλ�ñȽ���
	//	�� ����CardNo ���ƿ�����
	//	axis ָ����ţ�ȡֵ��Χ��DMC5410��0~3��DMC5800��0~7��DMC5600��0~5
	//	DMC5400A��0~3
	//	enable �ȽϹ���״̬��0����ֹ��1��ʹ��
	//	cmp_source �Ƚ�Դ��0��ָ��λ�ü�������1��������������
	//	����ֵ���������
	int rtn = dmc_compare_set_config(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, WORD(0), CS[uAxsIndex].uCmpSource);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"StartCompare::::dmc_compare_set_config ,ErrNo=(%d) ����ʧ�ܣ���",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	CS[uAxsIndex].bEnable =0;

	return ClearAllComparePoints(uAxsIndex);
}	

//************************************************************************************************************
