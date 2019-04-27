#pragma once
#include "LTDMC.h"
#include "MCardSettings.h"
#pragma comment(lib,"LTDMC.lib")

//UNICODE
//为了更方便的使用，现做统一声明如下：
//**********************************************************************************************
//雷赛运动控制卡 5000系列  版本Ver1.32，支持IO扩展	                                     
//																	
// V1.0 2015 
// V1.2 2016.5.27.18.38 <1.增加MessageBox的所有窗口的设定.>
// V1.3 2016.6.7.10.23  <1.增加 HomeAxis2 > <2.增加原点锁存功能>
// V1.31 2016.6.15.18.23  <1. > <2.增加原点锁存功能>
// V1.32 2016.7.23.11.23  <1.完成JOGMove与Jog在线变速统一函数> <2.增加一维位置比较功能>
// V1.33 2016.7.23.11.23  <1.修改EnableAxs 逻辑BUG> <2.>

//**********************************************************************************************
//			本类所有 有关距离和时间，角度 的地方统一单位为 ：脉冲(p) 和 秒(s)				
//						4/8个轴序号统一为从0开始 [0,1,2,3,...]						       
//			IO的卡号从本控制卡开始为序号0，第一张扩展卡为1.....[0,1,2,,,,,,]			   
//

//**********************************************************************************************
//运动控制卡配置表 LTDMC_CARD.cfg 具体内容 请用雷赛智能运动控制软件 Motion3XXX.exe 查看

//**********************************************************************************************
//<板卡电平与正常思维逻辑>问题
//
//一般情况 板卡 默认电平		输出：0表示低电平    ，1表示高电平
//								输入：0表示低电平	 ，1表示高电平

//在软件实际调用时的正常思维用  输出：1: 表示有输出  ，0表示无输出
//								输入：1：表示有信号  ，0表示无信号
//
//<本类会在内部工作时自动做一次反转，所以调用时只管用正常的思维逻辑就好了，不必考虑电气情况。>
//***********************************************************************************************


//功能输出口 结构类型
typedef struct _OUTPUT_
{
	CStringW	lpwstrPortName; //端口功能说明
	short		sCardNo;
	short		sNodeNo;		//表示当前功能口在哪张卡上，0，表示在运动控制卡上，其它表示在（BCardNo）号的CAN扩展卡上（扩展卡配置表卡地址要设置从1开始 ～8）；
	short		sPortNo;		//表示此功能口绑定了BNodeNo的哪个端口；
	short		sValue;			//表示此功能口的当前值 //1: 表示有输出  ，0表示无输出

	CStringW	lpwstr0;		//0位状态字符串 //点击显示此字符串的按钮后，会进入0位状态
	CStringW	lpwstr1;		//1位状态字符串 //点击显示此字符串的按钮后，会进入1位状态
}OUTPUT_PORT;

//功能输入口 结构类型
typedef struct _INPUT_
{
	CStringW	lpwstrPortName;	//端口功能说明
	short		sCardNo;		//表示当前功能口在哪张卡上
	short		sNodeNo;		//表示当前功能口在哪张IO卡上，0，表示在运动控制卡上
	short		sPortNo;		//表示此功能口绑定了BNodeNo的哪个端口；
	short		sValue;			//表示此功能口的当前值//1：表示有信号  ，0表示无信号
}INPUT_PORT;



typedef struct _AXIS_STATUS_
{
	BOOL	bDriverAlarm;	//驱动器报警																	//A[i].AS.bDriverAlarm			=0;						
	BOOL	bPLimtSnr;		//正   极限感应器，最后次更新状态 0表示 没到正极限									//A[i].AS.bEComplet				=1;
	BOOL	bNLimtSnr;		//负   极限感应器，最后次更新状态 0表示 没到负极限									//A[i].AS.bMComplet				=1;
	BOOL	bORGSnr;		//原点 极限感应器，最后次更新状态 0表示 没有到原点									//A[i].AS.bHome					=0;	
	BOOL	bMComplet;		//脉冲发送完成，执行完成状态，最后次更新状态 ，	0表示 正在执行							//A[i].AS.bPLimtSnr				=0;	
	BOOL	bEComplet;		//编码器检测实际完成，执行完成状态，最后次更新状态 ，	0表示 正在执行						//A[i].AS.bNLimtSnr				=0;	
	BOOL	bHome;			//表示当前轴是否已经复过位		//******  由用户程序代码中复位完成后自己设定  ******//	//A[i].AS.bORGSnr				=0;
	BOOL    bINPSnr;		//																			//A[i].AS.bINPSnr				=0;
	BOOL	bEMGSnr;		//																			//A[i].AS.bEMGSnr				=0;
	BOOL	bEZSnr;			//																			//A[i].AS.bEZSnr				=0;	
	BOOL	bRDYSnr;		//																			//A[i].AS.bRDYSnr				=0;
	long	lMCurPos;		//轴马达当前的位置 (脉冲数）整数													//A[i].AS.lMCurPos				=0;
	long	lEncCurPos;		//轴编码器当前位置 (脉冲数）整数													//A[i].AS.lEncCurPos			=0;
	BOOL	bDone	;		//提示轴是否已经停止，它比 bMComplet，bEComplet 先完成

}AXIS_STATUS;

typedef struct _AXIS_PARAM_
{	//28
										//默认=1000pulse/mm
		double	dPPU;					//设置脉冲当量  n pulse = 1 unit(mm，deg,m...)Equiv			
		int		nHomeMode;				//回零模式 //0：一次回零//1：一次回零加回找//2：二次回零	//3：一次回零后再记一个 EZ 脉冲进行回零//4：记一个 EZ 脉冲进行回零		
		int		nHomeDir;				//回零方向，0：负向，1：正向		
		float	fHomeVel;				//回零速度      pulse/s		
		float	fStartVel;				//开始速度      pulse/s		
		float	fVel;					//正常运行速度  pulse/s			
		float	fTAcc;					//加速时间 单位s						
		float	fTDec;					//减速时间 单位s				
		float	fTSmooth;				//S速度段端平滑时间 ，0～0.5s
		int		nAxsEncoderFactor;		//编码器系数（可能是倍数）
		int		nAxsBandDist;			//单位 pulse //默认保持时间0.1s	
		int		nBacklash;				//反向间隙值 pulse
		int		nPulseMode;				//=0 ,1 ,2 ,3  pulse+dir, //=4 ,5  双脉冲ccw/cw 
		int		nSrPinLogic;			//=0 设置感应信号有效电平，0：低有效，1：高有效
		float	fTStop;					//默认=0.01 急停减时间 单位：s
		BOOL	bPWMEnable;				//PWM使能：1表示PWM启用，0表示不启用

		BOOL	bAxsSvnEnable;			//轴使能：						1表示轴起用，0表示不使用
		int		nAxsSvnEnablePinLogic;	//设置伺服使能有效电平，		0：低有效，1：高有效	
		BOOL	bAxsInpEnable;			//轴INP信号使能：				1表示轴起用，0表示不使用	
		int		nAxsInpPinLogic;		//设置 INP 信号的有效电平，		0：低有效，1：高有效
		BOOL	bAxsAlmEnable;			//轴ALM信号使能：				1表示轴起用，0表示不使用	
		int		nAxsAlmPinLogic;		//设置 ALM 信号的有效电平，		0：低有效，默认1：高有效
		int		nAxsErcEnable;			//设置 ERC 信号输出：			0：低电平，1：高电平
		int		nEncoderInMode;			//设置编码器的计数方式			0：非 A/B 相(脉冲/方向) 1：1×A/B		2：2×A/B 		3：4×A/B
		int		nAxsEZPinLogic;			//设置 EZ 信号的有效电平，		0：低有效，默认1：高有效
		int		nAxsEMGPinLogic;		//设置 急停EMG信号的有效电平，	0：低有效，默认1：高有效		
		int		nEMGIOPort;				//指定当前轴映射为急停的当前卡的通用Input端口
		int		nMotorType;				//指定当前轴电机的类型，		0表示步进，默认1表示伺服

}AXIS_PARAM;


//轴 结构类型
typedef struct _AXIS_
{
	LPWSTR			lpwstrAxisName;	//轴说明
	short			sCardNo;		//卡号
	short			sAxisNo;		//轴号
	AXIS_STATUS		AS;				//轴的当前状态
	AXIS_PARAM		AP;				//轴的当前设置
}AXIS;

////轴 结构类型
//typedef struct _CAN_STATE_
//{
//	short			sNodeNo[8];		//Can节点号（1-8）
//	short			sCanSts[8];		//轴的当前状态 CAN-IO 通讯状态，0：断开，1：连接，2：异常
//}CAN_STATE;


//配置表路径
//const wchar_t* M_MOTION_CARD_CFG_PATH	= L"M_CARD.cfg";			//运动控制卡配置表路径


//轴 返回值 定义
typedef int ERROR_RESULT;
const int	MCARD_NO_ERROR	=0; //正确 返回
const int	MCARD_ERROR		=1; //错误 返回

typedef unsigned int UINT;
typedef BOOL (CALLBACK* ASFR )(UINT);
typedef BOOL (CALLBACK* DSFR )(UINT,UINT);

typedef struct _CMP_STS_ //CompareSts
{
	BOOL			bEnable;		//是否启用 一维低速位置比较 0：禁止，1：使能
	UINT			uCmpSource;		//比较源 0：指令位置计数器，1：编码器计数器 
	LONG			lCurPos;		//当前比较的位置
	LONG			lCmpNum;		//已经比较过的点数
	LONG			lCanAddNum;		//可以加入的新点数

}CMP_STS;
//uGetNo	
const	UINT  GETNO_ABLE_SRC	=0	;//比较开启状态及比较源 
const	UINT  GETNO_CUR_POS		=1	;//当前比较点位置 
const	UINT  GETNO_CMP_NUM		=2	;//已经比较过的点数
const	UINT  GETNO_CAN_ADD_NUM	=3	;//可以加入的比较点数


class CMCard
{
public:
///所有用GetXXX函数获取的最新值都放在这里
///////////////////////////////////////////////////////////////////
	OUTPUT_PORT		O[MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX];		//当前DO输出
	INPUT_PORT		I[MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * INPUT_PORT_COUNT_MAX ];		//当前Di输入
	AXIS			A[MCARD_COUNT_MAX * AXIS_COUNT_MAX];//轴号
	CMP_STS			CS[MCARD_COUNT_MAX * AXIS_COUNT_MAX];
	int				CANSTS[MCARD_COUNT_MAX][EX_IONODE_COUNT_MAX+1];//卡号，节点号
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	CMCard(void);
	~CMCard(void);
	//int GetLastError();//
	BOOL	m_bMotionCardOpenning;

	//实际检测到的卡数，用来防止调用过界
	int		m_nActualCardCount;

private:
	BOOL	m_bAutoHomming[MCARD_COUNT_MAX * AXIS_COUNT_MAX];		//仅供GetHomeAxis2Sts内部 调用
	unsigned short	m_EncSnsValue;				//编码器的计数方向 暂存变量

	//将索引轴号分解为具体的卡号，轴号
	//ERROR_RESULT BeAxisExist(UINT uAxsIndex ,short* sCardNo, short* sAxisNo);
	////将索引IO号分解为具体的卡号，节点号，端口号
	//ERROR_RESULT BeIOExist  (UINT uIOIndex  ,short* sCardNo, short* sNodeNo, short* sPortNo);
public:
	
	//调用安全回调***************************************************************************************
	ASFR m_pAxsSafeFilter;
	DSFR m_pDoSafeFilter;	

	//传入用户定义的防撞车检测函数，辅助开发动作流程，运动控制卡会在每次动作时来调用它；
	//设置轴安全回调函数
	BOOL SetAxsSafeFilter(BOOL (CALLBACK* lpfnAxsSafeFilter)(UINT));

	//设置IO安全回调函数
	BOOL SetDoSafeFilter(BOOL (CALLBACK* lpfnDoSafeFilter)(UINT,UINT));
	
	BOOL DebugSupperAxsSafeFilter(UINT uAxsIndex);
	BOOL DebugSupperDoSafeFilter(UINT uIOIndex, UINT uValue);
	//***************************************************************************************************

	////////////////////////////////////////////////////////////////////////////////////////
	//1.//打开运动控制器
	CWnd* m_pMBOwner;
	ERROR_RESULT OpenMotionCard(CWnd* pMBOwner );
	ERROR_RESULT CloseMotionCard();
	ERROR_RESULT SetMBOwner(CWnd* pMBOwner);
	ERROR_RESULT MB(LPCTSTR lpcsText);

	//将AP[AXIS_COUNT_MAX]中数据设置到卡中
	ERROR_RESULT SetAxisParam(UINT uAxsIndex);
	/////////////////////////////////////////////////////////////////////////////////////////
	//自动回原点1
	ERROR_RESULT HomeAxis1(UINT uAxsIndex);
	////获取自动回原点状态（即自动回原点执行进程是否完成）
	//ERROR_RESULT GetHomeAxis1Sts(UINT uAxsIndex);	

	////自动回原点2 nHomeDir =1 正向，nVelMode =0 低速，nHomeMode =4 Z相归零
	ERROR_RESULT HomeAxis2(UINT uAxsIndex,int nHomeDir,int nVelMode,int nHomeMode);
	////获取自动回原点状态（即自动回原点执行进程是否完成）
	//ERROR_RESULT GetHomeAxis2Sts(UINT uAxisNo);	


	//锁存原点
	ERROR_RESULT StartHomeLatch(UINT uAxsIndex);
	ERROR_RESULT GetHomeLatchSts(UINT uAxsIndex,BOOL &bSucceed, long &lPos);
	ERROR_RESULT ResetHomeLatchSts(UINT uAxsIndex);
	ERROR_RESULT EndHomeLatch(UINT uAxsIndex);

	//锁存指定输入 ---》待完成

	//一维低速位置比较10KHz//*************************************************************************************
	ERROR_RESULT StartCompare(UINT uAxsIndex,UINT uCmpSrc);	//开启指定轴的一维位置比较。 比较源，0：指令位置计数器，1：编码器计数器
	ERROR_RESULT ClearAllComparePoints(UINT uAxsIndex);	//清空指定轴的所有一维位置比较点
	
	//添加一个新的一维位置比较点
	//uCmpDir  :0 <=, 1>=;
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
	ERROR_RESULT AddNewComparePoint(UINT uAxsIndex,UINT uCmpDir,LONG lNewPointPos,UINT	uFunNo,UINT	uOutPortNo);
	
	//查询更新当前轴比较设置状态。
	//uGetNo	
	//GETNO_ABLE_SRC	=0	比较开启状态及比较源 
	//GETNO_CUR_POS		=1	当前比较点位置 
	//GETNO_CMP_NUM		=2	已经比较过的点数
	//GETNO_CAN_ADD_NUM	=3	可以加入的比较点数
	ERROR_RESULT GetCompareSts(UINT uAxsIndex,UINT uGetNo);
	ERROR_RESULT EndCompare(UINT uAxsIndex);//关闭指定轴的一维位置比较
	//************************************************************************************************************


	//设置轴位置
	ERROR_RESULT SetAxisCurPos(UINT uAxsIndex,long lPos);//设置轴的当前位置
	ERROR_RESULT SetAxisEncPos(UINT uAxsIndex, long dPos);//设置轴编码器位置
	//ERROR_RESULT ZeroAxisPos(UINT uAxsIndex);	//设置轴位置和相应编码器位置为0 ，并消除漂移；

	//获取轴的当前位置
	ERROR_RESULT GetAxisCurPos(UINT uAxsIndex);
	ERROR_RESULT GetAxisEncPos(UINT uAxsIndex);
	
	//获取轴的当前状态（包括 原点/正负极限状态，轴/编码器位置，完成信号状态）
	ERROR_RESULT GetAxisStatus(UINT uAxsIndex);

	//相对位置移动
	ERROR_RESULT RMove(UINT uAxsIndex, long lDist);
	ERROR_RESULT RMove2(UINT uAxsIndex, float fVel,long lDist);
	
	//绝对位置移动
	ERROR_RESULT AMove(UINT uAxsIndex, long lPos);
	ERROR_RESULT AMove2(UINT uAxsIndex, float fVel,long lPos);

	//Jog运动//改变JOG速度
	ERROR_RESULT JogMove(UINT uAxsIndex, float fVel, BOOL bDir);
	//改变JOG速度
	//ERROR_RESULT SetJogVel(UINT uAxsIndex, float fVel, BOOL bDir);

	ERROR_RESULT StopAxis(UINT uAxsIndex, UINT uMode=0);//=1急停，=0为减速停 //GT_Stop
	ERROR_RESULT EnableAxis(UINT uAxsIndex, BOOL bYes=1);

	//输入输出
	ERROR_RESULT SetDoBit(UINT uIOIndex, UINT uSetValue);
	ERROR_RESULT GetDiBit(UINT uIOIndex);
	ERROR_RESULT GetAllDi();
	//获取输出端口状态     Add in 2019.3.3 By Gogol
	ERROR_RESULT GetAllDo();

	//产生一个长脉冲
	ERROR_RESULT SetDoBitReverse(UINT uIOIndex, UINT uSetValue,short sTime); // 1高电平 ，0 低电平 // sTime 

	char* wc2c(CString &value);

	//输出错误信息
	CString m_strLastErrorInfo;
	long    m_lErrCount;

	//输出错误信息
	CString m_strLastErrorInfo2;
	long    m_lErrCount2;
#if _DEBUG
	inline void DebugInfo(LPCTSTR strInfo)
	{
		//AfxMessageBox(strInfo,0,0);
		CString str;
		char* c=NULL;
		str.Format(L"%s\r\n",strInfo);	
		c=wc2c(str);
		TRACE(c);
		free(c);
		c=NULL;
		m_lErrCount ++;
		m_strLastErrorInfo=strInfo;
	}
	inline void DebugInfo2(LPCTSTR strInfo)
	{
		//AfxMessageBox(strInfo,0,0);
		CString str;
		char* c=NULL;
		str.Format(L"%s\r\n",strInfo);	
		c=wc2c(str);
		TRACE(c);
		free(c);
		c=NULL;
		m_lErrCount2 ++;
		m_strLastErrorInfo2=strInfo;
	}
#else
#define DebugInfo(strInfo) 
#define DebugInfo2(strInfo) 
#endif

	inline void ClsAllErrInfo()
	{
		m_strLastErrorInfo=L"";
		m_lErrCount=0;
		m_strLastErrorInfo2=L"";
		m_lErrCount2=0;
	}

};
