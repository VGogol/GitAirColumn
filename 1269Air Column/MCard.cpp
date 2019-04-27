#include "StdAfx.h"
#include "MCard.h"
#include"G.h"
//为了更方便的使用，现做统一声明如下：
/***********************************注意声明************************************************/
//雷赛运动控制卡 5000系列  版本Ver1.32，支持IO扩展	                                     
//																	
// V1.0 2015 
// V1.2 2016.5.27.18.38 <1.增加MessageBox的所有窗口的设定.>
// V1.3 2016.6.7.10.23  <1.增加 HomeAxis2 > <2.增加原点锁存功能>
// V1.31 2016.6.15.18.23  <1. > <2.增加原点锁存功能>
// V1.32 2016.7.23.11.23  <1.完成JOGMove与Jog在线变速统一函数> <2.增加一维位置比较功能>
// V1.33 2016.7.23.11.23  <1.修改EnableAxs 逻辑BUG> <2.>



/***************************************************************************************** */
/*			本类所有 有关距离和时间，角度 的地方统一单位为 ：脉冲(p) 和 秒(s)			   */
/*						4/8个轴序号统一为从0开始 [0,1,2,3,...]						       */
/*			IO的卡号从本控制卡开始为序号0，第一张扩展卡为1.....[0,1,2,,,,,,]			   */
/***************************************************************************************** */


CMCard::CMCard(void)
{
	m_pMBOwner=NULL;
	CString strTmp;
	m_EncSnsValue=0;
	m_bMotionCardOpenning = FALSE;

	m_pAxsSafeFilter=NULL;
	m_pDoSafeFilter =NULL;

	//输入
	for(int i=0; i<(MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * INPUT_PORT_COUNT_MAX); i++)
	{
		I[i].sCardNo = BYTE(i / ((1 + EX_IONODE_COUNT_MAX) * INPUT_PORT_COUNT_MAX));
		I[i].sNodeNo = BYTE((i % ((1 + EX_IONODE_COUNT_MAX) * INPUT_PORT_COUNT_MAX)) / INPUT_PORT_COUNT_MAX);//运动控制卡 =0，扩展卡从1开始的
		I[i].sPortNo = BYTE(i % INPUT_PORT_COUNT_MAX);
		I[i].sValue	= 0;
		strTmp.Format(L"[%2d-%2d-%2d]备用", I[i].sCardNo,I[i].sNodeNo,I[i].sPortNo);
		I[i].lpwstrPortName =(LPWSTR)(LPCTSTR)strTmp;
	}
	

	//输出
	for(int i=0; i<(MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX); i++)
	{
		O[i].sCardNo = BYTE(i /((1 + EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX));//运动控制卡 =0，扩展卡从1开始的
		O[i].sNodeNo = BYTE((i % ((1 + EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX)) / OUTPUT_PORT_COUNT_MAX);
		O[i].sPortNo = BYTE( i % OUTPUT_PORT_COUNT_MAX);
		O[i].sValue	= 0;
		strTmp.Format(L"[%2d-%2d-%2d]备用", O[i].sCardNo,O[i].sNodeNo,O[i].sPortNo);
		O[i].lpwstrPortName =(LPWSTR)(LPCTSTR)strTmp;
	}

	//设置轴初始默认值
	for(int i=0; i<(MCARD_COUNT_MAX * AXIS_COUNT_MAX); i++)
	{
		m_bAutoHomming[i]=FALSE;
		A[i].sCardNo = BYTE( i / AXIS_COUNT_MAX );
		A[i].sAxisNo = BYTE( i % AXIS_COUNT_MAX );

		A[i].AS.bDriverAlarm			=0					;		    //驱动器报警																					
		A[i].AS.bEComplet				=1					;		    //正   极限感应器，最后次更新状态 0表示 没到正极限									
		A[i].AS.bMComplet				=1					;		    //负   极限感应器，最后次更新状态 0表示 没到负极限									
		A[i].AS.bHome					=0					;		    //原点 极限感应器，最后次更新状态 0表示 没有到原点									
		A[i].AS.bPLimtSnr				=0					;		    //脉冲发送完成，执行完成状态，最后次更新状态 ，	0表示 正在执行							
		A[i].AS.bNLimtSnr				=0					;		    //编码器检测实际完成，执行完成状态，最后次更新状态 ，	0表示 正在执行						
		A[i].AS.bORGSnr					=0					;		    //表示当前轴是否已经复过位		//******  由用户程序代码中复位完成后自己设定  ******//	
		A[i].AS.bINPSnr					=0					;		    //																			
		A[i].AS.bEMGSnr					=0					;		    //																			
		A[i].AS.bEZSnr					=0					;		    //																			
		A[i].AS.bRDYSnr					=0					;		    //																			
		A[i].AS.lMCurPos				=0					;		    //轴马达当前的位置 (脉冲数）整数													
		A[i].AS.lEncCurPos				=0					;		    //轴编码器当前位置 (脉冲数）整数													
		A[i].AS.bDone					=1					;		    //提示轴是否已经停止，它比 bMComplet，bEComplet 先完成,在雷赛卡中它是主要的运动状态，bMComplet，bEComplet 这两个是辅助的，可以不检测

		A[i].AP.dPPU					=9000				;		    //设置脉冲当量  n pulse = 1 unit(mm，deg,m...)Equiv//默认=1000pulse/mm			
		A[i].AP.nHomeMode				=1					;		    //回零模式 //0：一次回零//1：一次回零加回找//2：二次回零	//3：一次回零后再记一个 EZ 脉冲进行回零//4：记一个 EZ 脉冲进行回零		
		A[i].AP.nHomeDir				=1					;		    //回零方向，0：负向，1：正向		
		A[i].AP.fHomeVel				=/*G::PR[0].paravalue*/20000			;		    //回零速度      pulse/s		
		A[i].AP.fStartVel				=1000				;		    //开始速度      pulse/s		
		A[i].AP.fVel					=40000				;		    //正常运行速度  pulse/s			
		A[i].AP.fTAcc					=(float)0.01		;		    //加速时间 单位s						
		A[i].AP.fTDec					=(float)0.01		;		    //减速时间 单位s				
		A[i].AP.fTSmooth				=(float)0.25		;		    //S速度段端平滑时间 ，0～0.5s
		A[i].AP.nAxsEncoderFactor		=1					;		    //编码器系数（可能是倍数）
		A[i].AP.nAxsBandDist			=(int)A[i].AP.dPPU/100	;		//单位 pulse //默认保持时间0.1s	
		A[i].AP.nBacklash				=0					;		    //反向间隙值 pulse
		A[i].AP.nPulseMode				=0					;		    //=0 ,1 ,2 ,3  pulse+dir, //=4 ,5  双脉冲ccw/cw 
		A[i].AP.nSrPinLogic				=0					;		    //=0 设置感应信号有效电平，0：低有效，1：高有效
		A[i].AP.fTStop					=(float)0.01		;		    //默认=0.01 急停减时间 单位：s
		A[i].AP.bPWMEnable				=0					;		    //PWM使能：1表示PWM启用，0表示不启用

		A[i].AP.bAxsSvnEnable			=1					;		    //轴使能：						1表示轴起用，0表示不使用
		A[i].AP.nAxsSvnEnablePinLogic	=0					;		    //设置伺服使能有效电平，		0：低有效，1：高有效
		A[i].AP.bAxsInpEnable			=0					;		    //轴INP信号使能：				1表示轴起用，0表示不使用	
		A[i].AP.nAxsInpPinLogic			=0					;		    //设置 INP 信号的有效电平，		0：低有效，1：高有效
		A[i].AP.bAxsAlmEnable			=1					;		    //轴ALM信号使能：				1表示轴起用，0表示不使用	
		A[i].AP.nAxsAlmPinLogic			=1					;		    //设置 ALM 信号的有效电平，		0：低有效，默认1：高有效
		A[i].AP.nAxsErcEnable			=1					;		    //设置 ERC 信号输出		，		0：低电平，1：高电平	
		A[i].AP.nEncoderInMode			=3					;		    //设置编码器的计数方式			0：非 A/B 相(脉冲/方向) 1：1×A/B		2：2×A/B 		3：4×A/B
		A[i].AP.nAxsEZPinLogic			=1					;		    //设置 EZ 信号的有效电平，		0：低有效，默认1：高有效
		A[i].AP.nAxsEMGPinLogic			=1					;		    //设置 急停EMG信号的有效电平，	0：低有效，默认1：高有效		
		A[i].AP.nEMGIOPort				=0					;		    //指定当前轴映射为急停的当前卡的通用Input端口
		A[i].AP.nMotorType				=1					;		    //指定当前轴电机的类型，		0表示步进，默认1表示闭环伺服
	}									   
}		

CMCard::~CMCard(void)
{
}

ERROR_RESULT CMCard::SetMBOwner(CWnd* pMBOwner)
{
	if(!::IsWindow(pMBOwner->GetSafeHwnd()))
	{
		DebugInfo2(L"OpenMotionCard::SetMBOwner 设置主窗口异常,主窗口不存在！");  
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
	//打开并重置所有轴及所有扩展模块
	rtn = dmc_board_init();
	if(rtn == 0)
	{
		DebugInfo2(L"OpenMotionCard::dmc_board_init没有找到控制卡，或者控制卡异常！");  
		return MCARD_ERROR;
	}
	else if(rtn < 0)
	{
		CString str;
		str.Format(L"OpenMotionCard::dmc_board_init存在2张以上硬件卡号设置%d号的运动控制卡！");
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
		DebugInfo2(L"SetAxisParam::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//1.检查所有项是否符合要求//任何一项不符合，return FALSE; //待实现

	//检查卡是否存在
	short sTmpCardNo = short( uAxsIndex / AXIS_COUNT_MAX );
	short sTmpAxisNo = short( uAxsIndex % AXIS_COUNT_MAX );
	if (sTmpCardNo < m_nActualCardCount )
	{//卡存在
		A[uAxsIndex].sCardNo =sTmpCardNo;

		DWORD dwAxisCount=0;
		rtn = dmc_get_total_axes(WORD(sTmpCardNo),&dwAxisCount);
		if (rtn != 0)
		{
			str.Format(L"SetAxisParam::dmc_get_total_axes ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
		if ((DWORD)sTmpAxisNo < dwAxisCount)
		{
			//此轴存在
			A[uAxsIndex].sAxisNo = sTmpAxisNo;
		}
		else
		{
			//此轴不存在
			A[uAxsIndex].sAxisNo = -1;
		}
	}
	else
	{//卡不存在
		A[uAxsIndex].sCardNo = -1;
	}

	//2.设置每项参数到运动控制卡
	if (A[uAxsIndex].sCardNo == -1)//-1表示不存在
	{
		DebugInfo2(L"卡不存在！");
		return MCARD_ERROR;
	}
	if (A[uAxsIndex].sAxisNo != -1)//-1表示不存在
	{
			//设置指定轴的脉冲输出模式
			rtn = dmc_set_pulse_outmode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,A[uAxsIndex].AP.nPulseMode);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_pulse_outmode ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			
			//设置 ORG 原点信号有效电平，0：低有效，1：高有效
			rtn = dmc_set_home_pin_logic(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,A[uAxsIndex].AP.nSrPinLogic,0.0);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_home_pin_logic ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//设置 EL 限位信号,使能状态,有效电平,制动方式
			rtn = dmc_set_el_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 1,A[uAxsIndex].AP.nSrPinLogic, 0);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_el_mode ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}			
			//设置软限位
			//rtn = dmc_set_softlimit(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 0, 0, 0, 0, 0);
			//if (rtn != 0)
			//{
			//	str.Format(L"SetAxisParam::dmc_set_softlimit ,ErrNo=(%d) 调用失败！！",rtn);
			//	DebugInfo2(str);
			//	return MCARD_ERROR;
			//}
			//设置单轴速度S曲线参数
			/*rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,double(A[uAxsIndex].AP.fStartVel) ,
				double(A[uAxsIndex].AP.fVel),double(A[uAxsIndex].AP.fTAcc),
				double(A[uAxsIndex].AP.fTDec),double(A[uAxsIndex].AP.fStartVel));
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_profile ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}*/
			//rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 0, 
			//	double(A[uAxsIndex].AP.fTSmooth));
			//if (rtn != 0)
			//{
			//	str.Format(L"SetAxisParam::dmc_set_s_profile ,ErrNo=(%d) 调用失败！！",rtn);
			//	DebugInfo2(str);
			//	return MCARD_ERROR;
			//}

			//反向间隙值
			/*rtn = dmc_set_backlash_unit(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 
				double(A[uAxsIndex].AP.nBacklash)/double(A[uAxsIndex].AP.dPPU));
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_backlash_unit ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}*/


			//设置指定轴的 INP 信号,INP 信号使能，0：禁止，1：允许,INP 信号的有效电平，0：低有效，1：高有效
			//注意：当使能 INP 信号功能后，只有在 INP 信号为有效状态时，对应的轴才能进行运动，否则
			//此时检测轴的状态是正在运行（即对轴运动作限制）			
			rtn = dmc_set_inp_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.bAxsInpEnable,
				A[uAxsIndex].AP.nAxsInpPinLogic);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_inp_mode ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}			
		
			//设置指定轴的 ALM 信号ALM 信号使能，0：禁止，1：允许ALM 信号的有效电平，0：低有效，1：高有效ALM 信号的制动方式，0：立即停止（只支持该方式）
			rtn = dmc_set_alm_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.bAxsAlmEnable,
				A[uAxsIndex].AP.nAxsAlmPinLogic,0);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_alm_mode ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}	

			////设置指定轴的 ALM 信号ALM 信号使能，0：禁止，1：允许ALM 信号的有效电平，0：低有效，1：高有效ALM 信号的制动方式，0：立即停止（只支持该方式）
			//rtn = dmc_set_alm_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,
			//	A[uAxsIndex].AP.bAxsAlmEnable, A[uAxsIndex].AP.nAxsAlmPinLogic,0);
			//if (rtn != 0)
			//{
			//	str.Format(L"SetAxisParam::dmc_set_alm_mode ,ErrNo=(%d) 调用失败！！",rtn);
			//	DebugInfo2(str);
			//	return MCARD_ERROR;
			//}

			//控制指定轴的 ERC 信号输出输出电平，0：低电平，1：高电平
			rtn = dmc_write_erc_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsErcEnable);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_write_erc_pin ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			//设置编码器的计数方式
			//	0：非 A/B 相(脉冲/方向)
			//	1：1×A/B
			//	2：2×A/B
			//	3：4×A/B
			rtn = dmc_set_counter_inmode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nEncoderInMode);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_counter_inmode ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//设置指定轴的 EZ 信号有效电平，0：低有效，1：高有效
			rtn = dmc_set_ez_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsEZPinLogic,0,0);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_ez_mode ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			//设置 EMG 急停信号有效电平，0：低有效，1：高有效
			rtn = dmc_set_emg_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 1,A[uAxsIndex].AP.nAxsEMGPinLogic);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_emg_mode ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//直接设置
			//设置减速停信号有效电平，0：低有效，1：高有效
			rtn = dmc_set_io_dstp_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 0,0);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_io_dstp_mode ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//设置减速停止时间，单位：s
			//注 意：当发生异常停止时，如：限位信号（软硬件）被触发、减速停止信号(DSTP)被触发等
			//进行减速停止时， 减速停止时间都为 dmc_set_dec_stop_time 函数里设置的减速时间
			//设置第 0 号卡的通用输入口 0 接口作为所有轴的急停信号，并且设置急停信号为低
			//电平有效
			rtn = dmc_set_dec_stop_time(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,A[uAxsIndex].AP.fTStop);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_dec_stop_time ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}


			//double Filter;
			//CardNo = 0; //卡号
			//IoType = 3; //指定轴的 IO 信号类型为：急停信号
			//MapIoType = 6; //轴 IO 映射类型：通用输入端口
			//MapIoIndex = 0; //轴 IO 映射索引号：通用输入 0
			//设置轴 IO 映射关系
			rtn = dmc_set_axis_io_map(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 3, 6, A[uAxsIndex].AP.nEMGIOPort, 0.02);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_axis_io_map ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//编码器系数的说明：当使用 dmc_check_success_encoder 函数检测编码器是否到位时，其用于
			//判断的编码器位置为：编码器计数值乘以编码器系数的值。关于检测轴到
			//位状态函数的用法详见章节 7.10 检测轴到位状态功能的实现
			//设置位置误差带，单位：pulse
			rtn = dmc_set_factor_error(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,  A[uAxsIndex].AP.nAxsEncoderFactor,
																	A[uAxsIndex].AP.nAxsBandDist);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_factor_error ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//short dmc_set_equiv(WORD CardNo,WORD axis, double equiv)
			//	功 能：设置脉冲当量值
			//	参 数：CardNo 卡号
			//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
			//	DMC5400A：0~3
			//	equiv 脉冲当量，单位：pulse/unit
			//	返回值：错误代码
			//	注 意：1）该函数适用于基于脉冲当量的高级运动函数（包括点位、插补、连续插补运动）
			//	2）当使用基于脉冲当量的高级运动函数进行运动前，必须先使用该函数设置各运动
			//	轴的脉冲当量值，该值不能设置为 0
			//设置脉冲当量值
			rtn= dmc_set_equiv(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,A[uAxsIndex].AP.dPPU);
			if (rtn != 0)
			{
				str.Format(L"SetAxisParam::dmc_set_equiv ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

		}
		//PWM设置
		//注意：1）	DMC5800 中，当使能 PWM 功能后，7 号轴的脉冲端口（PUL+与 PUL-）作为 PWM 输出
		//			通道 0，7 号轴的方向端口（DIR+与 DIR-）作为 PWM 输出通道 1。
		//		2）	DMC5600 中，当使能 PWM 功能后，5 号轴的脉冲端口（PUL+与 PUL-）作为 PWM 输出
		//			通道 0，5 号轴的方向端口（DIR+与 DIR-）作为 PWM 输出通道 1。
		//		3）	当使用通道 0 时，硬件接线可接 PUL+与 GND（PUL-与 GND 则输出相反信号）；当使
		//			用通道 1 时，硬件接线可接 DIR+与 GND（DIR-与 GND 则输出相反信号）。
		//		4）	DMC5400A 中 CN23 有两路专用输出口，详见附录 5。
		//		4）	当使能 PWM 功能后，运动控制卡的相应轴端口将不能输出电机控制信号。
		/*rtn= dmc_set_pwm_enable(A[uAxsIndex].sCardNo, A[uAxsIndex].AP.bPWMEnable);
		if (rtn != 0)
		{
			str.Format(L"SetAxisParam::dmc_set_pwm_enable ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}*/


		//short dmc_set_can_state(WORD CardNo,WORD NodeNum,WORD state,WORD Baud)
		//	功 能：设置 CAN-IO 通讯状态；DMC5800/5600/5400A 卡专用
		//	参 数：CardNo 控制卡卡号
		//	NodeNum CAN 节点数，取值范围：1~4
		//	DMC5000 系列卡用户使用手册 V1.0
		//	— 263 —
		//	state 设置通讯状态，0：断开，1：连接
		//	Baud 设置控制卡波特率
		//	波特率参数：0,1,2,3,4,5,6
		//	对应波特率：1000Kbps，800Kbps,500Kbps,250Kbps,125Kbps,100Kbps
		//	返回值：错误代码
		//	注 意：1）当关闭运动控制卡时，CAN-IO 通讯不会被自动断开；当再次初始化运动控制卡时，
		//	CAN-IO 通讯依然保持之前的状态
		//	2）当连接 CAN 通讯时，必须使用 dmc_get_can_state 函数读取 CAN-IO 的通讯状态，
		//	确认 CAN 通讯已正常连接。当连接出现异常时，可再次调用 dmc_set_can_state
		//	函数进行连接
		//	3）设置波特率时需保证控制卡波特率与模块波特率相对应
		
		////设置 CAN-IO 通讯状态
		/*rtn= dmc_set_can_state(A[uAxsIndex].sCardNo,EX_IONODE_COUNT_MAX,1,0);
		if (rtn != 0)
		{
			str.Format(L"SetAxisParam::dmc_set_can_state ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}*/

		//short dmc_set_da_enable(WORD CardNo,WORD enable);
		//功 能：设置 DA 输出使能
		//	参 数：CardNo 卡号
		//	WORD enable DA 使能状态，0：禁止，1：使能
		/*rtn= dmc_set_da_enable(A[uAxsIndex].sCardNo,0);
		if (rtn != 0)
		{
			str.Format(L"SetAxisParam::dmc_set_da_enable ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}*/
		
		//G::MCard.SetAxisCurPos(AXS_TURNTABLE,0);
		//G::MCard.SetAxisEncPos(AXS_TURNTABLE,0);
		
		//设置伺服使能端口电平，0：低电平，1：高电平

		if ( A[uAxsIndex].AP.bAxsSvnEnable==1)
		{
			rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsSvnEnablePinLogic?0:1);//GT_AxisOn(uAxisNo+1);
			if (rtn != 0) 
			{
				str.Format(L"SetAxisParam::dmc_write_sevon_pin ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
		}
		else
		{
			rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsSvnEnablePinLogic?1:0);//GT_AxisOn(uAxisNo+1);
			if (rtn != 0) 
			{
				str.Format(L"SetAxisParam::dmc_write_sevon_pin ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
		}
	return EnableAxis(uAxsIndex,A[uAxsIndex].AP.bAxsSvnEnable);
}
//当前自动回原点
ERROR_RESULT CMCard::HomeAxis1(UINT uAxsIndex)
{
	int rtn=0;
	CString str;
	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
	{
		return MCARD_ERROR;
	}

	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"HomeAxis1::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"HomeAxis1::卡或轴不存在！");
		return MCARD_ERROR;
	}

	//检查轴使能
	if( A[uAxsIndex].AP.bAxsSvnEnable ==FALSE) 
	{
		str.Format(L"HomeAxis1::轴%d使能没有打开！！",uAxsIndex);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	m_bAutoHomming[uAxsIndex] = TRUE;//设置 正在进行复位 标志



	rtn=StopAxis(uAxsIndex,1);//立刻停止
	if (rtn != MCARD_NO_ERROR) 
	{
		DebugInfo2(L"HomeAxis :: StopAxis ,调用失败！！");
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}
	//short dmc_set_profile(WORD CardNo,WORD axis,double Min_Vel,double Max_Vel,double
	//Tacc,double Tdec,double Stop_Vel)
	//功 能：设置单轴运动速度曲线
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//Min_Vel 起始速度，单位：pulse/s (最大值为 2M)
	//Max_Vel 最大速度，单位：pulse/s (最大值为 2M)
	//Tacc 加速时间，单位：s (最小值为 0.001s)
	//Tdec 减速时间，单位：s (最小值为 0.001s)
	//Stop_Vel 停止速度，单位：pulse/s (最大值为 2M)
	//返回值：错误代码
	//设置单轴速度S曲线参数
	
	rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,double(A[uAxsIndex].AP.fHomeVel/*double(A[uAxsIndex].AP.fStartVel*/) ,
		double(A[uAxsIndex].AP.fVel),
		double(A[uAxsIndex].AP.fTAcc),double(A[uAxsIndex].AP.fTDec),0/*double(A[uAxsIndex].AP.fStartVel)*/);
	if (rtn != 0)
	{
		str.Format(L"HomeAxis::::dmc_set_profile ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	//short dmc_set_s_profile(WORD CardNo,WORD axis,WORD s_mode,double s_para)
	//	功 能：设置单轴速度曲线 S 段参数值
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	s_mode 保留参数，固定值为 0
	//	DMC5000 系列卡用户使用手册 V1.0
	//	— 230 —
	//	s_para S 段时间，单位：s；范围：0~0.5 s
	//	返回值：错误代码
	rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,double(A[uAxsIndex].AP.fTSmooth));
	if (rtn != 0)
	{
		str.Format(L"HomeAxis::::dmc_set_s_profile ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//short dmc_set_homemode(WORD CardNo,WORD axis,WORD home_dir,double vel_mode,
	//	WORD mode,WORD EZ_count)
	//	功 能：设置回原点模式
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	DMC5000 系列卡用户使用手册 V1.0
	//	— 222 —
	//	home_dir 回零方向，0：负向，1：正向
	//	vel_mode 回零速度模式：
	//	0：低速回零，即以本指令前面的 dmc_set_profile 函数设置的起
	//	始速度运行
	//	1：高速回零，即以本指令前面的 dmc_set_profile 函数设置的最
	//	大速度运行
	//	mode 回零模式：
	//	0：一次回零
	//	1：一次回零加回找
	//	2：二次回零
	//	3：一次回零后再记一个 EZ 脉冲进行回零
	//	4：记一个 EZ 脉冲进行回零
	//	EZ_count 保留参数，固定值为 0
	//	返回值：错误代码
	//	注意：当回零模式 mode=4 时，回零速度模式将固定为低速回零
	rtn = dmc_set_homemode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,
		A[uAxsIndex].AP.nHomeDir, 0/*double(A[uAxsIndex].AP.fHomeVel)*/, A[uAxsIndex].AP.nHomeMode, 0);
	if( rtn != 0) 
	{
		str.Format(L"HomeAxis :: dmc_set_homemode ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}

	//short dmc_home_move(WORD CardNo,WORD axis)
	//	功 能：回原点运动
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	返回值：错误代码


	//初始化两个状态变量，当用户调用完此函数，后调用GetHomeAxisSts来重新重置这两个状态
	A[uAxsIndex].AS.bMComplet	= FALSE;
	A[uAxsIndex].AS.bEComplet	= FALSE;
	A[uAxsIndex].AS.bHome		= FALSE;
	rtn = dmc_home_move(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if( rtn != 0) 
	{
		str.Format(L"HomeAxis :: dmc_set_homemode ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}

	//先调用一次更新一下状态
	GetAxisStatus(uAxsIndex);

	return MCARD_NO_ERROR;//成功返回 0；
}
//当前自动回原点 nHomeDir =1 正向，nVelMode =0 低速，nHomeMode =4 Z相归零
ERROR_RESULT CMCard::HomeAxis2(UINT uAxsIndex,int nHomeDir,int nVelMode,int nHomeMode)
{
	int rtn=0;
	CString str;
	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
	{
		return MCARD_ERROR;
	}

	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"HomeAxis1::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"HomeAxis1::卡或轴不存在！");
		return MCARD_ERROR;
	}

	//检查轴使能
	if( A[uAxsIndex].AP.bAxsSvnEnable ==FALSE) 
	{
		str.Format(L"HomeAxis1::轴%d使能没有打开！！",uAxsIndex);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	m_bAutoHomming[uAxsIndex] = TRUE;//设置 正在进行复位 标志



	rtn=StopAxis(uAxsIndex,1);//立刻停止
	if (rtn != MCARD_NO_ERROR) 
	{
		DebugInfo2(L"HomeAxis :: StopAxis ,调用失败！！");
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}
	//short dmc_set_profile(WORD CardNo,WORD axis,double Min_Vel,double Max_Vel,double
	//Tacc,double Tdec,double Stop_Vel)
	//功 能：设置单轴运动速度曲线
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//Min_Vel 起始速度，单位：pulse/s (最大值为 2M)
	//Max_Vel 最大速度，单位：pulse/s (最大值为 2M)
	//Tacc 加速时间，单位：s (最小值为 0.001s)
	//Tdec 减速时间，单位：s (最小值为 0.001s)
	//Stop_Vel 停止速度，单位：pulse/s (最大值为 2M)
	//返回值：错误代码
	//设置单轴速度S曲线参数
	
	rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,A[uAxsIndex].AP.fHomeVel,
		double(A[uAxsIndex].AP.fHomeVel),
		double(A[uAxsIndex].AP.fTAcc),double(A[uAxsIndex].AP.fTDec),double(A[uAxsIndex].AP.fStartVel));
	if (rtn != 0)
	{
		str.Format(L"HomeAxis::::dmc_set_profile ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	//short dmc_set_s_profile(WORD CardNo,WORD axis,WORD s_mode,double s_para)
	//	功 能：设置单轴速度曲线 S 段参数值
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	s_mode 保留参数，固定值为 0
	//	DMC5000 系列卡用户使用手册 V1.0
	//	— 230 —
	//	s_para S 段时间，单位：s；范围：0~0.5 s
	//	返回值：错误代码
	rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,double(A[uAxsIndex].AP.fTSmooth));
	if (rtn != 0)
	{
		str.Format(L"HomeAxis::::dmc_set_s_profile ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//short dmc_set_homemode(WORD CardNo,WORD axis,WORD home_dir,double vel_mode,
	//	WORD mode,WORD EZ_count)
	//	功 能：设置回原点模式
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	DMC5000 系列卡用户使用手册 V1.0
	//	— 222 —
	//	home_dir 回零方向，0：负向，1：正向
	//	vel_mode 回零速度模式：
	//	0：低速回零，即以本指令前面的 dmc_set_profile 函数设置的起
	//	始速度运行
	//	1：高速回零，即以本指令前面的 dmc_set_profile 函数设置的最
	//	大速度运行
	//	mode 回零模式：
	//	0：一次回零
	//	1：一次回零加回找
	//	2：二次回零
	//	3：一次回零后再记一个 EZ 脉冲进行回零
	//	4：记一个 EZ 脉冲进行回零
	//	EZ_count 保留参数，固定值为 0
	//	返回值：错误代码
	//	注意：当回零模式 mode=4 时，回零速度模式将固定为低速回零
	rtn = dmc_set_homemode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,
		nHomeDir, nVelMode, nHomeMode, 0);
	if( rtn != 0) 
	{
		str.Format(L"HomeAxis :: dmc_set_homemode ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}

	//short dmc_home_move(WORD CardNo,WORD axis)
	//	功 能：回原点运动
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	返回值：错误代码


	//初始化两个状态变量，当用户调用完此函数，后调用GetHomeAxisSts来重新重置这两个状态
	A[uAxsIndex].AS.bMComplet	= FALSE;
	A[uAxsIndex].AS.bEComplet	= FALSE;
	A[uAxsIndex].AS.bHome		= FALSE;
	rtn = dmc_home_move(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if( rtn != 0) 
	{
		str.Format(L"HomeAxis :: dmc_set_homemode ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		m_bAutoHomming[uAxsIndex] = FALSE;
		return MCARD_ERROR;
	}
	
	//先调用一次更新一下状态
	GetAxisStatus(uAxsIndex);
	return MCARD_NO_ERROR;//成功返回 0；
}

////获取HomeAxis1执行后的结果和状态
//ERROR_RESULT CMCard::GetHomeAxis1Sts(UINT uAxisNo)
//{
//	//只有在正确调用HomeAxis1 之后才能调用此函数 即 m_bAutoHomming[x]==TRUE
//	if (m_bAutoHomming[uAxisNo])
//	{	
//		DebugInfo(L"只有在正确调用HomeAxis1之后才能调用此函数,调用失败！！");
//		return MCARD_ERROR;	
//	}	
//	unsigned short sts;
//	int rtn = GT_HomeSts(uAxisNo+1,&sts);
//	if (rtn!=MCARD_NO_ERROR)	
//	{
//		DebugInfo2(L"HomeAxis :: GT_Home ,调用失败！！");
//		return MCARD_ERROR;
//	}
//	AS[uAxisNo].bMComplet = (int)sts;//0：自动回原点操作正在执行。1：自动回原点操作成功执行完毕。
//	AS[uAxisNo].bEComplet = (int)sts;//0：自动回原点操作正在执行。1：自动回原点操作成功执行完毕。
//
//	return MCARD_NO_ERROR;//成功返回 0；
//}

//获取当前轴的位置
ERROR_RESULT CMCard::GetAxisCurPos(UINT uAxsIndex)
{
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"GetAxisCurPos::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"GetAxisCurPos::卡或轴不存在！");
		return MCARD_ERROR;
	}
	//long dmc_get_position(WORD CardNo,WORD axis)
	//功 能：读取指令脉冲位置
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//返回值：指令脉冲位置，单位：pulse
	A[uAxsIndex].AS.lMCurPos = dmc_get_position(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);

	return  MCARD_NO_ERROR;
}
//获取当前轴编码器的位置
ERROR_RESULT CMCard::GetAxisEncPos(UINT uAxsIndex)
{
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"GetAxisEncPos::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"GetAxisEncPos::卡或轴不存在！");
		return MCARD_ERROR;
	}
	//long dmc_get_encoder(WORD CardNo,WORD axis)
	//功 能：读取指定轴编码器脉冲计数值
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//返回值：编码器计数值，单位：pulse
	//说 明：此函数 axis 参数为 8 时可以读手轮编码器计数值
	A[uAxsIndex].AS.lEncCurPos = dmc_get_encoder(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);

	return  MCARD_NO_ERROR;;
}
//获取当前轴的各种状态
ERROR_RESULT CMCard::GetAxisStatus(UINT uAxsIndex)
{
	DWORD dwSts=0;
	int rtn=0;

	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"GetAxisStatus ::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"GetAxisStatus ::卡或轴不存在！");
		return MCARD_ERROR;
	}
	
	//DWORD dmc_axis_io_status(WORD CardNo,WORD axis)
	//功 能：读取指定轴有关运动信号的状态
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//返回值：见表 9.2
	//表 9.2 轴的运动信号状态
	//位号 信号名称 描述
	//0 ALM 1：表示伺服报警信号 ALM 为 ON； 0：OFF
	//1 EL+ 1：表示正硬限位信号 +EL 为 ON； 0：OFF
	//2 EL- 1：表示负硬限位信号–EL 为 ON； 0：OFF
	//3 EMG 1：表示急停信号 EMG 为 ON； 0：OFF
	//4 ORG 1：表示原点信号 ORG 为 ON； 0：OFF
	//6 SL+ 1：表示正软限位信号+SL 为 ON； 0：OFF
	//7 SL- 1：表示负软件限位信号-SL 为 ON； 0：OFF
	//8 INP 1：表示伺服到位信号 INP 为 ON； 0：OFF
	//9 EZ 1：表示 EZ 信号为 ON； 0：OFF
	//DMC5000 系列卡用户使用手册 V1.0
	//— 228 —
	//位号 信号名称 描述
	//10 RDY		1：表示伺服准备信号 RDY 为 ON（DMC5800 卡专用） ；		0：OFF		
	//11 DSTP 1： 表示减速停止信号 DSTP 为 ON （DMC5800 卡专用） ；		0：OFF		
	//其他位 保留
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
	//功 能：检测指定轴的运动状态
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//返回值：0：指定轴正在运行，1：指定轴已停止
	//注 意：此函数适用于单轴、PVT 运动

	//short dmc_check_success_pulse(WORD CardNo,WORD axis)
	//功 能：检测指令到位
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//返回值：0：表示指令位置在设定的目标位置的误差带之外
	//1：表示指令位置在设定的目标位置的误差带之内
	//注 意：1）该函数只适用于单轴运动
	//2）检测函数请在 dmc_check_done 检测到轴停止后调用，函数调用后会等待轴到位后
	//返回，如果调用函数 100ms 内未到位，函数超时返回认为不到位
	
	//short dmc_check_success_encoder(WORD CardNo,WORD axis)
	//功 能：检测编码器到位
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//返回值：0：表示编码器位置在设定的目标位置的误差带之外
	//1：表示编码器位置在设定的目标位置的误差带之内
	//注 意：1）该函数只适用于单轴运动
	//2）检测函数请在 dmc_check_done 检测到轴停止后调用， 函数调用后会等待轴到位后
	//返回，如果调用函数 100ms 内未到位，函数超时返回认为不到位	

	dwSts = dmc_check_done(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);	
	A[uAxsIndex].AS.bDone = dwSts;
	if (dwSts == 1)
	{
		//类型为伺服
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
		DebugInfo2(L"GetAxisStatus ::GetAxisCurPos ,调用失败！！");
		return MCARD_ERROR;
	}

	rtn= GetAxisEncPos(uAxsIndex);
	if (rtn!=MCARD_NO_ERROR)	
	{
		DebugInfo2(L"GetAxisStatus ::GetAxisEncPos ,调用失败！！");	
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}
//设置轴的当前位置
ERROR_RESULT CMCard::SetAxisCurPos(UINT uAxsIndex,long lPos)//设置轴位置
{
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetAxisCurPos :: 运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"SetAxisCurPos :: 卡或轴不存在！");
		return MCARD_ERROR;
	}

	//short dmc_set_position(WORD CardNo,WORD axis,long current_position)
	//	功 能：设置指令脉冲位置
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	current_position 指令脉冲位置，单位：pulse
	//	返回值：错误代码

	int rtn =dmc_set_position(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,lPos);
	if (rtn!=0)	
	{
		CString str;
		str.Format(L"SetAxisCurPos :: dmc_set_position ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	return MCARD_NO_ERROR;
}
//设置当前轴编码器的位置
ERROR_RESULT CMCard::SetAxisEncPos(UINT uAxsIndex, long lPos)
{

	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetAxisEncPos ::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"SetAxisEncPos ::卡或轴不存在！");
		return MCARD_ERROR;
	}

	//short dmc_set_encoder(WORD CardNo,WORD axis,long encoder_value)
	//DMC5000 系列卡用户使用手册 V1.0
	//— 245 —
	//功 能：设置指定轴编码器脉冲计数值
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//encoder_value 编码器计数值，单位：pulse
	//返回值：错误代码
	int rtn =dmc_set_encoder(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,lPos);
	if (rtn!=0)	
	{
		CString str;
		str.Format(L"SetAxisEncPos :: dmc_set_encoder ,ErrNo=(%d) 调用失败！！",rtn);
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
		DebugInfo2(L"RMove2::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"RMove2::卡或轴不存在！");
		return MCARD_ERROR;
	}


	if (TRUE != A[uAxsIndex].AP.bAxsSvnEnable)
	{
		CString str;
		str.Format(L"RMove2::轴%d使能关闭不能移动！",uAxsIndex);
		AfxMessageBox(str);
		DebugInfo(str);
		return MCARD_ERROR;
	}
	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
	{
		return MCARD_ERROR;
	}

	rtn	= StopAxis(uAxsIndex,1);//先停止
	if (rtn != MCARD_NO_ERROR) 
	{
		DebugInfo(L"RMove2 :: StopAxis ,调用失败！！");
		return MCARD_ERROR;
	}
//dmc_set_pulse_outmode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, 0);//设置 0 号轴脉冲输出方式
	if (1==A[uAxsIndex].AS.bDone)
	{
		//short dmc_set_profile(WORD CardNo,WORD axis,double Min_Vel,double Max_Vel,double
		//Tacc,double Tdec,double Stop_Vel)
		//功 能：设置单轴运动速度曲线
		//参 数：CardNo 控制卡卡号
		//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
		//DMC5400A：0~3
		//Min_Vel 起始速度，单位：pulse/s (最大值为 2M)
		//Max_Vel 最大速度，单位：pulse/s (最大值为 2M)
		//Tacc 加速时间，单位：s (最小值为 0.001s)
		//Tdec 减速时间，单位：s (最小值为 0.001s)
		//Stop_Vel 停止速度，单位：pulse/s (最大值为 2M)
		//返回值：错误代码
		//设置单轴速度S曲线参数
		rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,double(A[uAxsIndex].AP.fStartVel) ,
			double(fVel),
			double(A[uAxsIndex].AP.fTAcc),double(A[uAxsIndex].AP.fTDec),double(A[uAxsIndex].AP.fStartVel));
		if (rtn != 0)
		{
			str.Format(L"RMove2::::dmc_set_profile ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
		//short dmc_set_s_profile(WORD CardNo,WORD axis,WORD s_mode,double s_para)
		//	功 能：设置单轴速度曲线 S 段参数值
		//	参 数：CardNo 控制卡卡号
		//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
		//	DMC5400A：0~3
		//	s_mode 保留参数，固定值为 0
		//	DMC5000 系列卡用户使用手册 V1.0
		//	— 230 —
		//	s_para S 段时间，单位：s；范围：0~0.5 s
		//	返回值：错误代码
		//rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,double(A[uAxsIndex].AP.fTSmooth));
		//if (rtn != 0)
		//{
		//	str.Format(L"RMove2::::dmc_set_s_profile ,ErrNo=(%d) 调用失败！！",rtn);
		//	DebugInfo2(str);
		//	return MCARD_ERROR;
		//}
	}


	//short dmc_pmove(WORD CardNo,WORD axis,long Dist,WORD posi_mode)
	//功 能：指定轴点位运动
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//Dist 目标位置，单位：pulse
	//posi_mode 运动模式，0：相对坐标模式，1：绝对坐标模式
	//返回值：错误代码
	//注 意：当运动模式为相对坐标模式时，目标位置大于 0 时正向运动，小于 0 时反向运动
	
	//初始化两个状态变量，当用户调用完此函数，后调用GetAxisStatus来重新重置这两个状态
	A[uAxsIndex].AS.bMComplet = FALSE;
	A[uAxsIndex].AS.bEComplet = FALSE;


	rtn =dmc_pmove(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,lDist,0);
	if (rtn!=0)	
	{
		str.Format(L"RMove2:: :: dmc_pmove ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//先调用一次更新一下状态
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
		DebugInfo2(L"JogMove::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"JogMove::卡或轴不存在！");
		return MCARD_ERROR;
	}


	if (TRUE != A[uAxsIndex].AP.bAxsSvnEnable)
	{
		CString str;
		str.Format(L"JogMove::轴%d使能关闭不能移动！",uAxsIndex);
		AfxMessageBox(str);
		DebugInfo(str);
		return MCARD_ERROR;
	}

	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
	{
		return MCARD_ERROR;
	}

	//rtn	= StopAxis(uAxsIndex,1);//先停止
	//if (rtn != MCARD_NO_ERROR) 
	//{
	//	DebugInfo(L"JogMove :: StopAxis ,调用失败！！");
	//	return MCARD_ERROR;
	//}

	GetAxisStatus(uAxsIndex);	
	if (A[uAxsIndex].AS.bDone)
	{
		//short dmc_set_profile(WORD CardNo,WORD axis,double Min_Vel,double Max_Vel,double
		//Tacc,double Tdec,double Stop_Vel)
		//功 能：设置单轴运动速度曲线
		//参 数：CardNo 控制卡卡号
		//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
		//DMC5400A：0~3
		//Min_Vel 起始速度，单位：pulse/s (最大值为 2M)
		//Max_Vel 最大速度，单位：pulse/s (最大值为 2M)
		//Tacc 加速时间，单位：s (最小值为 0.001s)
		//Tdec 减速时间，单位：s (最小值为 0.001s)
		//Stop_Vel 停止速度，单位：pulse/s (最大值为 2M)
		//返回值：错误代码
		//设置单轴速度S曲线参数
		rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,double(A[uAxsIndex].AP.fStartVel) ,
			double(fVel),
			double(A[uAxsIndex].AP.fTAcc),double(A[uAxsIndex].AP.fTDec),double(A[uAxsIndex].AP.fStartVel));
		if (rtn != 0)
		{
			str.Format(L"RMove2::::dmc_set_profile ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
		//short dmc_set_s_profile(WORD CardNo,WORD axis,WORD s_mode,double s_para)
		//	功 能：设置单轴速度曲线 S 段参数值
		//	参 数：CardNo 控制卡卡号
		//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
		//	DMC5400A：0~3
		//	s_mode 保留参数，固定值为 0
		//	DMC5000 系列卡用户使用手册 V1.0
		//	— 230 —
		//	s_para S 段时间，单位：s；范围：0~0.5 s
		//	返回值：错误代码
		rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,double(A[uAxsIndex].AP.fTSmooth));
		if (rtn != 0)
		{
			str.Format(L"RMove2::::dmc_set_s_profile ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}

		//short dmc_vmove(WORD CardNo,WORD axis,WORD dir)
		//功 能：指定轴连续运动
		//参 数：CardNo 控制卡卡号
		//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
		//DMC5400A：0~3
		//dir 运动方向，0：负方向，1：正方向
		//返回值：错误代码
		//初始化两个状态变量，当用户调用完此函数，后调用GetAxisStatus来重新重置这两个状态
		A[uAxsIndex].AS.bMComplet = FALSE;
		A[uAxsIndex].AS.bEComplet = FALSE;
		rtn =dmc_vmove(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,bDir);
		if (rtn!=0)	
		{
			str.Format(L"JogMove:: :: dmc_vmove ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}

	}
	else
	{
		//short dmc_change_speed(WORD CardNo,WORD axis,double Curr_Vel,double Taccdec)
		//功 能：在线改变指定轴的当前运动速度
		//DMC5000 系列卡用户使用手册 V1.0
		//— 231 —
		//参 数：CardNo 控制卡卡号
		//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
		//DMC5400A：0~3
		//Curr_Vel 改变后的运动速度，单位：pulse/s
		//Taccdec 保留参数，固定值为 0
		//返回值：错误代码
		//注 意：1）该函数适用于单轴运动中的变速
		//2）变速一旦成立，该轴的默认运行速度将会被改写为 Curr_Vel，也即当调用
		//dmc_get_profile 回读速度参数时会发生与 dmc_set_profile 所设置的不一致的
		//现象
		//3）在连续运动中 Curr_Vel 负值表示往负向变速，正值表示往正向变速。在点位运动
		//中 Curr_Vel 只允许正值

		rtn =dmc_change_speed(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,double(fVel),double(A[uAxsIndex].AP.fTDec));
		if (rtn!=0)	
		{
			str.Format(L"JogMove:: :: dmc_change_speed ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
	}
	//先调用一次更新一下状态
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
//		DebugInfo2(L"JogMove::运动控制卡还没有打开！！");
//		return MCARD_ERROR;
//	}
//	//检查轴是否存在
//	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
//	{
//		DebugInfo2(L"JogMove::卡或轴不存在！");
//		return MCARD_ERROR;
//	}
//
//
//	if (TRUE != A[uAxsIndex].AP.bAxsSvnEnable)
//	{
//		CString str;
//		str.Format(L"JogMove::轴%d使能关闭不能移动！",uAxsIndex);
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
//	rtn	= StopAxis(uAxsIndex,0);//先停止
//	if (rtn != MCARD_NO_ERROR) 
//	{
//		DebugInfo(L"JogMove :: StopAxis ,调用失败！！");
//		return MCARD_ERROR;
//	}
//
//
//	//short dmc_vmove(WORD CardNo,WORD axis,WORD dir)
//	//功 能：指定轴连续运动
//	//参 数：CardNo 控制卡卡号
//	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
//	//DMC5400A：0~3
//	//dir 运动方向，0：负方向，1：正方向
//	//返回值：错误代码
//	//初始化两个状态变量，当用户调用完此函数，后调用GetAxisStatus来重新重置这两个状态
//	A[uAxsIndex].AS.bMComplet = FALSE;
//	A[uAxsIndex].AS.bEComplet = FALSE;
//	rtn =dmc_vmove(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,bDir);
//	if (rtn!=0)	
//	{
//		str.Format(L"JogMove:: :: dmc_vmove ,ErrNo=(%d) 调用失败！！",rtn);
//		DebugInfo2(str);
//		return MCARD_ERROR;
//	}
//	//short dmc_change_speed(WORD CardNo,WORD axis,double Curr_Vel,double Taccdec)
//	//功 能：在线改变指定轴的当前运动速度
//	//DMC5000 系列卡用户使用手册 V1.0
//	//— 231 —
//	//参 数：CardNo 控制卡卡号
//	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
//	//DMC5400A：0~3
//	//Curr_Vel 改变后的运动速度，单位：pulse/s
//	//Taccdec 保留参数，固定值为 0
//	//返回值：错误代码
//	//注 意：1）该函数适用于单轴运动中的变速
//	//2）变速一旦成立，该轴的默认运行速度将会被改写为 Curr_Vel，也即当调用
//	//dmc_get_profile 回读速度参数时会发生与 dmc_set_profile 所设置的不一致的
//	//现象
//	//3）在连续运动中 Curr_Vel 负值表示往负向变速，正值表示往正向变速。在点位运动
//	//中 Curr_Vel 只允许正值
//
//	rtn =dmc_change_speed(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,double(fVel),double(A[uAxsIndex].AP.fTDec));
//	if (rtn!=0)	
//	{
//		str.Format(L"JogMove:: :: dmc_change_speed ,ErrNo=(%d) 调用失败！！",rtn);
//		DebugInfo2(str);
//		return MCARD_ERROR;
//	}
//
//	//先调用一次更新一下状态
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
		DebugInfo2(L"AMove2::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"AMove2::卡或轴不存在！");
		return MCARD_ERROR;
	}


	if (TRUE != A[uAxsIndex].AP.bAxsSvnEnable)
	{
		CString str;
		str.Format(L"AMove2::轴%d使能关闭不能移动！",uAxsIndex);
		AfxMessageBox(str);
		DebugInfo(str);
		return MCARD_ERROR;
	}
	if(FALSE == DebugSupperAxsSafeFilter(uAxsIndex))
	{
		return MCARD_ERROR;
	}

	rtn	= StopAxis(uAxsIndex,0);//先停止
	if (rtn != MCARD_NO_ERROR) 
	{
		DebugInfo(L"RMove2 :: StopAxis ,调用失败！！");
		return MCARD_ERROR;
	}


	//short dmc_set_profile(WORD CardNo,WORD axis,double Min_Vel,double Max_Vel,double
	//Tacc,double Tdec,double Stop_Vel)
	//功 能：设置单轴运动速度曲线
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//Min_Vel 起始速度，单位：pulse/s (最大值为 2M)
	//Max_Vel 最大速度，单位：pulse/s (最大值为 2M)
	//Tacc 加速时间，单位：s (最小值为 0.001s)
	//Tdec 减速时间，单位：s (最小值为 0.001s)
	//Stop_Vel 停止速度，单位：pulse/s (最大值为 2M)
	//返回值：错误代码
	//设置单轴速度S曲线参数
	rtn = dmc_set_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,double(A[uAxsIndex].AP.fStartVel) ,
		double(fVel),
		double(A[uAxsIndex].AP.fTAcc),double(A[uAxsIndex].AP.fTDec),double(A[uAxsIndex].AP.fStartVel));
	if (rtn != 0)
	{
		str.Format(L"AMove2::::dmc_set_profile ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//short dmc_set_s_profile(WORD CardNo,WORD axis,WORD s_mode,double s_para)
	//	功 能：设置单轴速度曲线 S 段参数值
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	s_mode 保留参数，固定值为 0
	//	DMC5000 系列卡用户使用手册 V1.0
	//	— 230 —
	//	s_para S 段时间，单位：s；范围：0~0.5 s
	//	返回值：错误代码
	rtn = dmc_set_s_profile(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,double(A[uAxsIndex].AP.fTSmooth));
	if (rtn != 0)
	{
		str.Format(L"AMove2::::dmc_set_s_profile ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//short dmc_pmove(WORD CardNo,WORD axis,long Dist,WORD posi_mode)
	//功 能：指定轴点位运动
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//Dist 目标位置，单位：pulse
	//posi_mode 运动模式，0：相对坐标模式，1：绝对坐标模式
	//返回值：错误代码
	//注 意：当运动模式为相对坐标模式时，目标位置大于 0 时正向运动，小于 0 时反向运动

	//初始化两个状态变量，当用户调用完此函数，后调用GetAxisStatus来重新重置这两个状态
	A[uAxsIndex].AS.bMComplet = FALSE;
	A[uAxsIndex].AS.bEComplet = FALSE;
	rtn =dmc_pmove(A[uAxsIndex].sCardNo,A[uAxsIndex].sAxisNo,lPos,1);
	if (rtn!=0)	
	{
		str.Format(L"AMove2:: :: dmc_pmove ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//先调用一次更新一下状态
	GetAxisStatus(uAxsIndex);	

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::AMove(UINT uAxsIndex, long lPos)
{
	return AMove2(uAxsIndex, float(A[uAxsIndex].AP.fVel), lPos);
}
////将大轴号分解为具体的卡号，轴号
//ERROR_RESULT CMCard::BeAxisExist(UINT uAxsIndex ,short* sCardNo,short* sAxisNo)
//{
//	int rtn=0;
//	CString str;
//	short sTmpCardNo = short( uAxsIndex / AXIS_COUNT_MAX );
//	short sTmpAxisNo = short( uAxsIndex % AXIS_COUNT_MAX );
//	if (sTmpCardNo < m_nActualCardCount )
//	{//卡存在
//		DWORD dwAxisCount=0;
//		rtn = dmc_get_total_axes(WORD(sTmpCardNo),&dwAxisCount);
//		if (rtn != 0)
//		{
//			str.Format(L"BeAxisExist::dmc_get_total_axes ,ErrNo=(%d) 调用失败！！",rtn);
//			DebugInfo2(str);
//			return MCARD_ERROR;
//		}
//		if (sTmpAxisNo < dwAxisCount)
//		{//轴存在
//			*sCardNo = sTmpCardNo;
//			*sAxisNo = sTmpAxisNo;
//				
//			return MCARD_NO_ERROR;
//		}
//		else
//		{
//			//此轴不存在，跳过此轴设置
//			DebugInfo2(L"BeAxisExist::此轴不存在！！");
//			return MCARD_ERROR;
//		}
//	}
//	else
//	{
//		DebugInfo2(L"BeAxisExist:: 卡号不存在！！");
//		return MCARD_ERROR;
//	}
//}

//指定轴停止运动stop_mode 制动方式，0：减速停止，1：紧急停止
ERROR_RESULT CMCard::StopAxis(UINT uAxsIndex, UINT uMode)
{	
	int rtn=0;
	CString str;
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"StopAxis::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"StopAxis::卡或轴不存在！");
		return MCARD_ERROR;
	}

	rtn = dmc_stop(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,  uMode);
	if (rtn != 0)
	{
		str.Format(L"StopAxis::dmc_stop ,ErrNo=(%d) 调用失败！！",rtn);
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
		DebugInfo2(L"EnableAxis::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"EnableAxis::卡或轴不存在！");
		return MCARD_ERROR;
	}
	//short dmc_write_sevon_pin(WORD CardNo,WORD axis,WORD on_off)
	//功 能：控制指定轴的伺服使能端口的输出
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//DMC5400A：0~3
	//on_off 设置伺服使能端口电平，0：低电平，1：高电平
	//返回值：错误代码

	///****这里可能写反了
	//if ( bYes==1)
	//{
	//	rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsSvnEnablePinLogic?0:1);//GT_AxisOn(uAxisNo+1);
	//	if (rtn != 0) 
	//	{
	//		str.Format(L"EnableAxis::dmc_write_sevon_pin ,ErrNo=(%d) 调用失败！！",rtn);
	//		DebugInfo2(str);
	//		return MCARD_ERROR;
	//	}
	//}
	//else
	//{
	//	rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsSvnEnablePinLogic?1:0);//GT_AxisOn(uAxisNo+1);
	//	if (rtn != 0) 
	//	{
	//		str.Format(L"EnableAxis::dmc_write_sevon_pin ,ErrNo=(%d) 调用失败！！",rtn);
	//		DebugInfo2(str);
	//		return MCARD_ERROR;
	//	}
	//}
	if ( bYes==1)
	{
		rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, A[uAxsIndex].AP.nAxsSvnEnablePinLogic);//GT_AxisOn(uAxisNo+1);
		if (rtn != 0) 
		{
			str.Format(L"EnableAxis::dmc_write_sevon_pin ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
	}
	else
	{
		rtn=dmc_write_sevon_pin(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, !A[uAxsIndex].AP.nAxsSvnEnablePinLogic);//GT_AxisOn(uAxisNo+1);
		if (rtn != 0) 
		{
			str.Format(L"EnableAxis::dmc_write_sevon_pin ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
	}



	A[uAxsIndex].AP.bAxsSvnEnable=bYes;
	
	//不管怎么，操作使能，过后复位状态都重置为0
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
		DebugInfo2(L"SetDoBit::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}


	if(O[uIOIndex].sNodeNo == 0)//运动控制卡本体上的输出
	{

		//short dmc_write_outbit(WORD CardNo, WORD bitno,WORD on_off)
		//	功 能：设置指定控制卡的某个输出端口的电平
		//	参 数：CardNo 控制卡卡号
		//	bitno 输出端口号，取值范围：DMC5410：0~31；DMC5800/5600/5400A：0~15
		//	on_off 输出电平，0：低电平，1：高电平
		//	返回值：错误代码
		rtn	 = dmc_write_outbit((WORD)O[uIOIndex].sCardNo,(WORD)O[uIOIndex].sPortNo,(WORD)(uSetValue?0:1));
		if (rtn!=0)	
		{
			str.Format(L"SetDoBit:: :: dmc_write_outbit ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
		//Sleep(1);
		////short dmc_read_outbit(WORD CardNo, WORD bitno)
		////	功 能：读取指定控制卡的某个输出端口的电平
		////	参 数：CardNo 控制卡卡号
		////	bitno 输入端口号，取值范围：DMC5410：0~31；DMC5800/5600/5400A：0~15
		////	返回值：指定输出端口的电平，0：低电平，1：高电平
		//rtn = dmc_read_outbit((WORD)O[uIOIndex].sCardNo,(WORD)O[uIOIndex].sPortNo);
		//if (rtn != (int)(BSetValue?0:1) )
		//{
		//	str.Format(L"SetDoBit:: :: dmc_write_outbit  调用失败 或者 dmc_read_outbit调用失败 ！！");
		//	DebugInfo2(str);
		//	return MCARD_ERROR;
		//}
	}
	else
	{
		//short dmc_get_can_state(WORD CardNo,WORD* NodeNum,WORD* state)
		//功 能：读取 CAN-IO 通讯状态；DMC5800/5600/5400A 卡专用
		//参 数：CardNo 控制卡卡号
		//NodeNum 返回 CAN 节点数
		//state 返回 CAN-IO 通讯状态，0：断开，1：连接，2：异常
		//返回值：错误代码
			
		//short dmc_write_can_outbit(WORD CardNo,WORD Node,WORD bitno,WORD on_off)
		//功 能：设置指定 CAN-IO 扩展模块的某个输出端口的电平；DMC5800/5600/5400A 卡专用
		//参 数：CardNo 控制卡卡号
		//Node CAN 节点号，取值范围：1~8
		//bitno 输出端口号，取值范围：0~15
		//on_off 输出电平，0：低电平，1：高电平
		//返回值：错误代码
		rtn	 = dmc_write_can_outbit((WORD) O[uIOIndex].sCardNo , (WORD)O[uIOIndex].sNodeNo,(WORD)O[uIOIndex].sPortNo,(WORD)(uSetValue?0:1));
		if (rtn!=0)	
		{
			str.Format(L"SetDoBit:: :: dmc_write_outbit ,ErrNo=(%d) 调用失败！！",rtn);
			DebugInfo2(str);
			return MCARD_ERROR;
		}
		//Sleep(1);
		////short dmc_read_can_outbit(WORD CardNo,WORD Node,WORD bitno)
		////功 能：读取指定 CAN-IO 扩展模块的某个输出端口的电平；DMC5800/5600/5400A 卡专用
		////参 数：CardNo 控制卡卡号
		////Node CAN 节点号，取值范围：1~8
		////bitno 输出端口号，取值范围：0~15
		////返回值：指定输出端口的电平，0：低电平，1：高电平

		//rtn = dmc_read_can_outbit((WORD)O[uIOIndex].sCardNo, (WORD)O[uIOIndex].sNodeNo, (WORD)O[uIOIndex].sPortNo);
		//if (rtn != (int)( BSetValue ? 0 : 1 ) )
		//{
		//	str.Format(L"SetDoBit:: :: dmc_write_can_outbit  调用失败 或者 dmc_read_can_outbit调用失败 ！！");
		//	DebugInfo2(str);
		//	return MCARD_ERROR;
		//}

	}
	O[uIOIndex].sValue	= uSetValue;

	return MCARD_NO_ERROR;
}
//BIndex输出端口号 ， BSetValue  要保持的输出状态 0、1 ， sTime  保持时间 ms
ERROR_RESULT CMCard::SetDoBitReverse(UINT uIOIndex, UINT uSetValue,short sTime)
{
	int rtn=0;
	CString str;
	if (FALSE ==m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetDoBit::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}
	if(FALSE == DebugSupperDoSafeFilter(uIOIndex, uSetValue))
	{
		return MCARD_ERROR;
	}

	if (O[uIOIndex].sValue == (short)uSetValue)
	{
		//保持的状态与输出一致，直接返回
		return MCARD_NO_ERROR;
	}
	else 
	{
		if (sTime<0)
		{
			return MCARD_NO_ERROR;
		}
		if(O[uIOIndex].sNodeNo == 0)//运动控制卡本体上的输出
		{
			//short dmc_write_outbit(WORD CardNo, WORD bitno,WORD on_off)
			//	功 能：设置指定控制卡的某个输出端口的电平
			//	参 数：CardNo 控制卡卡号
			//	bitno 输出端口号，取值范围：DMC5410：0~31；DMC5800/5600/5400A：0~15
			//	on_off 输出电平，0：低电平，1：高电平
			//	返回值：错误代码
			rtn	 = dmc_write_outbit((WORD)O[uIOIndex].sCardNo,(WORD)O[uIOIndex].sPortNo,(WORD)(0?0:1));
			if (rtn!=0)	
			{
				str.Format(L"SetDoBit:: :: dmc_write_outbit ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}

			//short dmc_reverse_outbit(WORD CardNo,WORD bitno,double reverse_time)
			//	功 能：IO 输出延时翻转
			//	参 数：CardNo 控制卡卡号
			//	bitno 输出端口号，取值范围：0~15
			//	reverse_time 延时翻转时间，单位：s
			//	返回值：错误代码
			//	注 意：1）该函数只能对 OUT0~15 端口进行操作
			//	2）当延时翻转时间参数设置为 0 时，此时延时翻转时间将为无限大
			rtn	 = dmc_reverse_outbit((WORD)O[uIOIndex].sCardNo,(WORD)O[uIOIndex].sPortNo,(double)(double(sTime)/1000.00));
			if (rtn!=0)	
			{
				str.Format(L"SetDoBitReverse:: :: dmc_reverse_outbit ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
		}
		else
		{
			rtn	 = dmc_write_can_outbit((WORD) O[uIOIndex].sCardNo , (WORD)O[uIOIndex].sNodeNo,(WORD)O[uIOIndex].sPortNo,(WORD)(uSetValue?0:1));
			if (rtn!=0)	
			{
				str.Format(L"SetDoBit:: :: dmc_write_outbit ,ErrNo=(%d) 调用失败！！",rtn);
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
				str.Format(L"SetDoBit:: :: dmc_write_outbit ,ErrNo=(%d) 调用失败！！",rtn);
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
		DebugInfo2(L"SetDoBit::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}


	if(I[uIOIndex].sNodeNo == 0)//运动控制卡本体上的输出
	{

		//short dmc_read_inbit(WORD CardNo, WORD bitno)
		//	功 能：读取指定控制卡的某个输入端口的电平
		//	参 数：CardNo 控制卡卡号
		//	bitno 输入端口号，取值范围：DMC5410：0~31；DMC5800/5600/5400A：0~15
		//	返回值：指定的输入端口电平：0：低电平，1：高电平
		//	注意：DMC5400A 中 OUT12、0UT13 输出口无效
		rtn	 = dmc_read_inbit((WORD)I[uIOIndex].sCardNo,(WORD)I[uIOIndex].sPortNo);

	}
	else
	{
		//short dmc_read_can_inbit(WORD CardNo,WORD Node,WORD bitno)
		//	功 能：读取指定 CAN-IO 扩展模块的某个输入端口的电平；DMC5800/5600/5400A 卡专用
		//	参 数：CardNo 控制卡卡号
		//	Node CAN 节点号，取值范围：1~8
		//	bitno 输入端口号，取值范围：0~15
		//	返回值：指定输入端口的电平，0：低电平，1：高电平
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
		DebugInfo2(L"SetDoBit::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	for (WORD c = 0; c < MCARD_COUNT_MAX ; c++ )
	{
		for (WORD n = 0; n < (1+EX_IONODE_COUNT_MAX) ; n++ )
		{
			if (n == 0)
			{
				//DWORD dmc_read_inport(WORD CardNo,WORD portno)
				//	功 能：读取指定控制卡的全部输入端口的电平
				//	参 数：CardNo 控制卡卡号
				//	portno IO 组号，取值范围：DMC5410：0；DMC5800/5600/5400A：0、1
				//	返回值：DMC5410：bit0~bit31 值分别代表第 0~31 号输入端口的电平
				//	DMC5800/5600/5400A：bit0~bit31 的定义见表 9.3
				//	表 9.3 DMC5800/5600/5400A 卡 dmc_read_inport 函数返回值各位的定义表
				//	第 0 组（portno 参数） 第 1 组（portno 参数）
				//	函数返回值的 bit 输入口号 输入口名称 函数返回值的 bit 输入口号 输入口名称
				rtn	 = dmc_read_inport(c,0);
			}
			else
			{
				//DWORD dmc_read_can_inport(WORD CardNo,WORD Node,WORD PortNo)
				//	功 能：读取指定 CAN-IO 扩展模块的全部输入端口的电平；DMC5800/5600/5400A 卡专用
				//	参 数：CardNo 控制卡卡号
				//	Node CAN 节点号，取值范围：1~8
				//	PortNo 保留参数，固定值为 0
				//	返回值：bit0~bit15 的值分别代表第 0~15 号输入端口的电平
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

//获取输出端口状态     Add in 2019.3.3 By Gogol
ERROR_RESULT CMCard::GetAllDo()
{
	DWORD rtn = 0;
	CString str;
	if (FALSE == m_bMotionCardOpenning)
	{
		DebugInfo2(L"SetDoBit::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	for (WORD c = 0; c < MCARD_COUNT_MAX; c++)
	{
		for (WORD n = 0; n < (1 + EX_IONODE_COUNT_MAX); n++)
		{
			if (n == 0)
			{
				//DWORD dmc_read_outport(WORD CardNo,WORD portno)
				//	功 能：读取指定控制卡的全部输出端口的电平
				//	参 数：CardNo 控制卡卡号
				//	portno IO 组号，保留参数，固定值位0
				//	返回值：DMC5410：bit0~bit31 值分别代表第 0~31 号输入端口的电平
				//	DMC5C00/5800/5600/5400A：bit0~bit31 的定义见表 9.4
				//	函数返回值的 bit 输入口号 输入口名称 函数返回值的 bit 输入口号 输入口名称
				rtn = dmc_read_outport(c, 0);
			}
			else
			{
				//DWORD dmc_read_can_outport(WORD CardNo,WORD Node,WORD PortNo)
				//	功 能：读取指定 CAN-IO 扩展模块的全部输入端口的电平；DMC5800/5600/5400A 卡专用
				//	参 数：CardNo 控制卡卡号
				//	Node CAN 节点号，取值范围：1~8
				//	PortNo 保留参数，固定值为 0
				//	返回值：bit0~bit15 的值分别代表第 0~15 号输入端口的电平
				rtn = dmc_read_can_outport(c, n, 0);
			}
			for (WORD p = 0; p < 16; p++)
			{
				int n1MainCardPorts = ((1 + EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX);
				int n1NodePorts = OUTPUT_PORT_COUNT_MAX;
				int i = (c * n1MainCardPorts) + (n * n1NodePorts) + p;
				O[i].sValue = ((~rtn) >> O[i].sPortNo) & DWORD(1);    //位非，右移
			}
		}
	}
	return MCARD_NO_ERROR;
}

//设置轴安全回调函数
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


//设置IO安全回调函数
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
	
	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"HomeAxis1::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"HomeAxis1::卡或轴不存在！");
		return MCARD_ERROR;
	}

	//short dmc_set_homelatch_mode(WORD CardNo,WORD axis,WORD enable,WORD logic,
	//WORD source)
	//功 能：设置原点锁存模式；DMC5800/5600/5400A 卡专用
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5800：0~7，DMC5600：0~5,DMC5400A：0~3
	//enable 原点锁存使能，0：禁止，1：允许
	//logic 触发方式，0：下降沿，1：上升沿
	//source 位置源选择，0：指令位置计数器，1：编码器计数器
	//返回值：错误代码
	short rtn = dmc_set_homelatch_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,1,0 ,0);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"StartHomeLatch::::dmc_set_homelatch_mode ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	//short dmc_reset_homelatch_flag(WORD CardNo,WORD axis)
	//功 能：清除原点锁存标志；DMC5800/5600/5400A 卡专用
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5800：0~7，DMC5600：0~5,DMC5400A：0~3
	//返回值：错误代码
	rtn = dmc_reset_homelatch_flag(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"StartHomeLatch::::dmc_reset_homelatch_flag ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::GetHomeLatchSts(UINT uAxsIndex,BOOL &bSucceed, long &lPos)
{
	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"GetHomeLatchSts::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"GetHomeLatchSts::卡或轴不存在！");
		return MCARD_ERROR;
	}
	//long dmc_get_homelatch_flag(WORD CardNo,WORD axis)
	//功 能：读取原点锁存标志；DMC5800/5600/5400A 卡专用
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5800：0~7，DMC5600：0~5,DMC5400A：0~3
	//返回值：原点锁存标志，0：未锁存，1：锁存
	bSucceed=dmc_get_homelatch_flag(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if(bSucceed)
	{
		//long dmc_get_homelatch_value(WORD CardNo,WORD axis)
		//功 能：读取原点锁存值；DMC5800/5600/5400A 卡专用
		//参 数：CardNo 控制卡卡号
		//axis 指定轴号，取值范围：DMC5800：0~7，DMC5600：0~5,DMC5400A：0~3
		//返回值：锁存值，单位：pulse
		lPos=dmc_get_homelatch_flag(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	}

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::ResetHomeLatchSts(UINT uAxsIndex)
{
	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"ResetHomeLatchSts::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"ResetHomeLatchSts::卡或轴不存在！");
		return MCARD_ERROR;
	}

	//short dmc_reset_homelatch_flag(WORD CardNo,WORD axis)
	//功 能：清除原点锁存标志；DMC5800/5600/5400A 卡专用
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5800：0~7，DMC5600：0~5,DMC5400A：0~3
	//返回值：错误代码
	short rtn = dmc_reset_homelatch_flag(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"ResetHomeLatchSts::::dmc_reset_homelatch_flag ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}
ERROR_RESULT CMCard::EndHomeLatch(UINT uAxsIndex)
{
	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"EndHomeLatch::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"EndHomeLatch::卡或轴不存在！");
		return MCARD_ERROR;
	}
	
	//short dmc_reset_homelatch_flag(WORD CardNo,WORD axis)
	//功 能：清除原点锁存标志；DMC5800/5600/5400A 卡专用
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5800：0~7，DMC5600：0~5,DMC5400A：0~3
	//返回值：错误代码
	short rtn = dmc_reset_homelatch_flag(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"EndHomeLatch::::dmc_reset_homelatch_flag ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	//short dmc_set_homelatch_mode(WORD CardNo,WORD axis,WORD enable,WORD logic,
	//WORD source)
	//功 能：设置原点锁存模式；DMC5800/5600/5400A 卡专用
	//参 数：CardNo 控制卡卡号
	//axis 指定轴号，取值范围：DMC5800：0~7，DMC5600：0~5,DMC5400A：0~3
	//enable 原点锁存使能，0：禁止，1：允许
	//logic 触发方式，0：下降沿，1：上升沿
	//source 位置源选择，0：指令位置计数器，1：编码器计数器
	//返回值：错误代码
	rtn = dmc_set_homelatch_mode(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,0,0,0);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"EndHomeLatch::::dmc_set_homelatch_mode ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}


//一维低速位置比较10KHz//*************************************************************************************
//开启指定轴的一维位置比较	
ERROR_RESULT CMCard::StartCompare(UINT uAxsIndex,UINT uCmpSrc)
{
	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"StartCompare::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"StartCompare::卡或轴不存在！");
		return MCARD_ERROR;
	}
	//short dmc_compare_set_config(WORD CardNo, WORD axis,WORD enable, WORD cmp_source)
	//	功 能：设置一维位置比较器
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	enable 比较功能状态，0：禁止，1：使能
	//	cmp_source 比较源，0：指令位置计数器，1：编码器计数器
	//	返回值：错误代码
	int rtn = dmc_compare_set_config(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, WORD(1), WORD(uCmpSrc));
	if(rtn != 0)
	{
		CString str;
		str.Format(L"StartCompare::::dmc_compare_set_config ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	CS[uAxsIndex].bEnable =1;
	CS[uAxsIndex].uCmpSource = uCmpSrc;

	return ClearAllComparePoints(uAxsIndex);
}

//清空指定轴的所有一维位置比较点
ERROR_RESULT CMCard::ClearAllComparePoints(UINT uAxsIndex)
{
	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"ClearAllComparePoints::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"ClearAllComparePoints::卡或轴不存在！");
		return MCARD_ERROR;
	}
	//short dmc_compare_clear_points(WORD CardNo,WORD axis)
	//	功 能：清除已添加的所有一维位置比较点
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	返回值：错误代码
	int rtn = dmc_compare_clear_points(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"ClearAllComparePoints::::dmc_compare_clear_points ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	CS[uAxsIndex].lCmpNum=0;

	return MCARD_NO_ERROR;
}	

//添加一个新的一维位置比较点
//uCmpDir  =0 设定位置小于等于轴当前位置时输出（正向运转时）, =1设定位置大于等于轴当前位置时输出（负向运转时）
//uFunNo;	//uOutPortNo;
//功能码	//输出口号                 （主要是指主卡上的吧？待确定中...）
//1			IO 号	IO 置为高电平
//2			IO 号	IO 置为低电平
//3			IO 号	取反 IO
//5			IO 号	输出 500us 脉冲
//6			IO 号	输出 1ms 脉冲
//7			IO 号	输出 10ms 脉冲
//8			IO 号	输出 100ms 脉冲
//13		轴号	停止指定轴
ERROR_RESULT CMCard::AddNewComparePoint(UINT uAxsIndex,UINT uCmpDir,LONG lNewPointPos,UINT	uFunNo,UINT	uOutPortNo)
{
	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"AddNewComparePoint::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"AddNewComparePoint::卡或轴不存在！");
		return MCARD_ERROR;
	}
	
	if(MCARD_ERROR == GetCompareSts(uAxsIndex,GETNO_CAN_ADD_NUM))
	{	
		DebugInfo2(L"AddNewComparePoint::GetCompareSts 调用失败！");
		return MCARD_ERROR;
	}
	else if( CS[uAxsIndex].lCanAddNum<=0 )
	{	
		DebugInfo2(L"AddNewComparePoint::比较缓冲区已满，不能增加新的比较点！");
		return MCARD_ERROR;
	}

	//short dmc_compare_add_point(WORD CardNo,WORD axis, long pos, WORD dir, WORD action,
	//	DWORD actpara)
	//	功 能：添加一维位置比较点
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5000 系列卡用户使用手册 V1.0
	//	— 251 —
	//	DMC5400A：0~3
	//	pos 比较位置，单位：pulse
	//	dir 比较模式，0：小于等于，1：大于等于
	//	action 比较点触发功能编号，见表 9.5
	//	actpara 比较点触发功能参数，见表 9.5
	//	返回值：错误代码
	int rtn = dmc_compare_add_point(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,lNewPointPos,WORD(uCmpDir),WORD(uFunNo),DWORD(uOutPortNo));
	if(rtn != 0)
	{
		CString str;
		str.Format(L"AddNewComparePoint::dmc_compare_add_point ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}

	return MCARD_NO_ERROR;
}	

//查询更新当前轴比较设置状态。
//uGetNo	
//GETNO_ABLE_SRC	=0	;//比较开启状态及比较源 
//GETNO_CUR_POS		=1	;//当前比较点位置 
//GETNO_CMP_NUM		=2	;//已经比较过的点数
//GETNO_CAN_ADD_NUM	=3	;//可以加入的比较点数
ERROR_RESULT CMCard::GetCompareSts(UINT uAxsIndex,UINT uGetNo)
{
	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"GetCompareSts::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"GetCompareSts::卡或轴不存在！");
		return MCARD_ERROR;
	}
	switch(uGetNo)
	{
	case 0:
		{
			//short dmc_compare_get_config(WORD CardNo, WORD axis,WORD* enable,WORD* cmp_source)
			//	功 能：读取一维位置比较器设置
			//	参 数：CardNo 控制卡卡号
			//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
			//	DMC5400A：0~3
			//	enable 返回比较功能状态
			//	cmp_source 返回比较源
			//	返回值：错误代码
			WORD enable=0;
			WORD cmp_source=0;
			int rtn = dmc_compare_get_config(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,&enable,&cmp_source);
			if(rtn != 0)
			{
				CString str;
				str.Format(L"GetCompareSts::dmc_compare_get_config ,ErrNo=(%d) 调用失败！！",rtn);
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
			//	功 能：读取当前一维比较点位置
			//	参 数：CardNo 控制卡卡号
			//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
			//	DMC5400A：0~3
			//	pos 返回当前比较点位置，单位：pulse
			//	返回值：错误代码
			long pos=0;
			int rtn = dmc_compare_get_current_point(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,&pos);
			if(rtn != 0)
			{
				CString str;
				str.Format(L"GetCompareSts::dmc_compare_get_current_point ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			CS[uAxsIndex].lCurPos=pos;
		}
		break;
	case 2:
		{
			//short dmc_compare_get_points_runned(WORD CardNo,WORD axis,long* PointNum)
			//	功 能：查询已经比较过的一维比较点个数
			//	参 数：CardNo 控制卡卡号
			//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
			//	DMC5400A：0~3
			//	PointNum 返回已经比较过的点数
			//	返回值：错误代码
			long PointNum=0;
			int rtn = dmc_compare_get_points_runned(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,&PointNum);
			if(rtn != 0)
			{
				CString str;
				str.Format(L"GetCompareSts::dmc_compare_get_points_runned ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			CS[uAxsIndex].lCmpNum=PointNum;
		}
		break;
	case 3:
		{
			//short dmc_compare_get_points_remained(WORD CardNo,WORD axis,long* PointNum)
			//	功 能：查询可以加入的一维比较点个数
			//	DMC5000 系列卡用户使用手册 V1.0
			//	— 252 —
			//	参 数：CardNo 控制卡卡号
			//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
			//	DMC5400A：0~3
			//	PointNum 返回可以加入的比较点数
			//	返回值：s错误代码
			long PointNum=0;
			int rtn = dmc_compare_get_points_remained(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo,&PointNum);
			if(rtn != 0)
			{
				CString str;
				str.Format(L"GetCompareSts::dmc_compare_get_points_remained ,ErrNo=(%d) 调用失败！！",rtn);
				DebugInfo2(str);
				return MCARD_ERROR;
			}
			CS[uAxsIndex].lCanAddNum=PointNum;		
		}
		break;
	}
	return MCARD_NO_ERROR;
}
//关闭指定轴的一维位置比较
ERROR_RESULT CMCard::EndCompare(UINT uAxsIndex)
{
	//运动控制卡打开检查
	if(m_bMotionCardOpenning ==FALSE) 
	{
		DebugInfo2(L"EndCompare::运动控制卡还没有打开！！");
		return MCARD_ERROR;
	}

	//检查轴是否存在
	if (A[uAxsIndex].sCardNo == -1 || A[uAxsIndex].sAxisNo == -1)//-1表示不存在
	{
		DebugInfo2(L"EndCompare::卡或轴不存在！");
		return MCARD_ERROR;
	}
	//short dmc_compare_set_config(WORD CardNo, WORD axis,WORD enable, WORD cmp_source)
	//	功 能：设置一维位置比较器
	//	参 数：CardNo 控制卡卡号
	//	axis 指定轴号，取值范围：DMC5410：0~3，DMC5800：0~7，DMC5600：0~5
	//	DMC5400A：0~3
	//	enable 比较功能状态，0：禁止，1：使能
	//	cmp_source 比较源，0：指令位置计数器，1：编码器计数器
	//	返回值：错误代码
	int rtn = dmc_compare_set_config(A[uAxsIndex].sCardNo, A[uAxsIndex].sAxisNo, WORD(0), CS[uAxsIndex].uCmpSource);
	if(rtn != 0)
	{
		CString str;
		str.Format(L"StartCompare::::dmc_compare_set_config ,ErrNo=(%d) 调用失败！！",rtn);
		DebugInfo2(str);
		return MCARD_ERROR;
	}
	CS[uAxsIndex].bEnable =0;

	return ClearAllComparePoints(uAxsIndex);
}	

//************************************************************************************************************
