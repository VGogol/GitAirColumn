#pragma once
#include "LTDMC.h"
#include "MCardSettings.h"
#pragma comment(lib,"LTDMC.lib")

//UNICODE
//Ϊ�˸������ʹ�ã�����ͳһ�������£�
//**********************************************************************************************
//�����˶����ƿ� 5000ϵ��  �汾Ver1.32��֧��IO��չ	                                     
//																	
// V1.0 2015 
// V1.2 2016.5.27.18.38 <1.����MessageBox�����д��ڵ��趨.>
// V1.3 2016.6.7.10.23  <1.���� HomeAxis2 > <2.����ԭ�����湦��>
// V1.31 2016.6.15.18.23  <1. > <2.����ԭ�����湦��>
// V1.32 2016.7.23.11.23  <1.���JOGMove��Jog���߱���ͳһ����> <2.����һάλ�ñȽϹ���>
// V1.33 2016.7.23.11.23  <1.�޸�EnableAxs �߼�BUG> <2.>

//**********************************************************************************************
//			�������� �йؾ����ʱ�䣬�Ƕ� �ĵط�ͳһ��λΪ ������(p) �� ��(s)				
//						4/8�������ͳһΪ��0��ʼ [0,1,2,3,...]						       
//			IO�Ŀ��Ŵӱ����ƿ���ʼΪ���0����һ����չ��Ϊ1.....[0,1,2,,,,,,]			   
//

//**********************************************************************************************
//�˶����ƿ����ñ� LTDMC_CARD.cfg �������� �������������˶�������� Motion3XXX.exe �鿴

//**********************************************************************************************
//<�忨��ƽ������˼ά�߼�>����
//
//һ����� �忨 Ĭ�ϵ�ƽ		�����0��ʾ�͵�ƽ    ��1��ʾ�ߵ�ƽ
//								���룺0��ʾ�͵�ƽ	 ��1��ʾ�ߵ�ƽ

//�����ʵ�ʵ���ʱ������˼ά��  �����1: ��ʾ�����  ��0��ʾ�����
//								���룺1����ʾ���ź�  ��0��ʾ���ź�
//
//<��������ڲ�����ʱ�Զ���һ�η�ת�����Ե���ʱֻ����������˼ά�߼��ͺ��ˣ����ؿ��ǵ��������>
//***********************************************************************************************


//��������� �ṹ����
typedef struct _OUTPUT_
{
	CStringW	lpwstrPortName; //�˿ڹ���˵��
	short		sCardNo;
	short		sNodeNo;		//��ʾ��ǰ���ܿ������ſ��ϣ�0����ʾ���˶����ƿ��ϣ�������ʾ�ڣ�BCardNo���ŵ�CAN��չ���ϣ���չ�����ñ���ַҪ���ô�1��ʼ ��8����
	short		sPortNo;		//��ʾ�˹��ܿڰ���BNodeNo���ĸ��˿ڣ�
	short		sValue;			//��ʾ�˹��ܿڵĵ�ǰֵ //1: ��ʾ�����  ��0��ʾ�����

	CStringW	lpwstr0;		//0λ״̬�ַ��� //�����ʾ���ַ����İ�ť�󣬻����0λ״̬
	CStringW	lpwstr1;		//1λ״̬�ַ��� //�����ʾ���ַ����İ�ť�󣬻����1λ״̬
}OUTPUT_PORT;

//��������� �ṹ����
typedef struct _INPUT_
{
	CStringW	lpwstrPortName;	//�˿ڹ���˵��
	short		sCardNo;		//��ʾ��ǰ���ܿ������ſ���
	short		sNodeNo;		//��ʾ��ǰ���ܿ�������IO���ϣ�0����ʾ���˶����ƿ���
	short		sPortNo;		//��ʾ�˹��ܿڰ���BNodeNo���ĸ��˿ڣ�
	short		sValue;			//��ʾ�˹��ܿڵĵ�ǰֵ//1����ʾ���ź�  ��0��ʾ���ź�
}INPUT_PORT;



typedef struct _AXIS_STATUS_
{
	BOOL	bDriverAlarm;	//����������																	//A[i].AS.bDriverAlarm			=0;						
	BOOL	bPLimtSnr;		//��   ���޸�Ӧ�������θ���״̬ 0��ʾ û��������									//A[i].AS.bEComplet				=1;
	BOOL	bNLimtSnr;		//��   ���޸�Ӧ�������θ���״̬ 0��ʾ û��������									//A[i].AS.bMComplet				=1;
	BOOL	bORGSnr;		//ԭ�� ���޸�Ӧ�������θ���״̬ 0��ʾ û�е�ԭ��									//A[i].AS.bHome					=0;	
	BOOL	bMComplet;		//���巢����ɣ�ִ�����״̬�����θ���״̬ ��	0��ʾ ����ִ��							//A[i].AS.bPLimtSnr				=0;	
	BOOL	bEComplet;		//���������ʵ����ɣ�ִ�����״̬�����θ���״̬ ��	0��ʾ ����ִ��						//A[i].AS.bNLimtSnr				=0;	
	BOOL	bHome;			//��ʾ��ǰ���Ƿ��Ѿ�����λ		//******  ���û���������и�λ��ɺ��Լ��趨  ******//	//A[i].AS.bORGSnr				=0;
	BOOL    bINPSnr;		//																			//A[i].AS.bINPSnr				=0;
	BOOL	bEMGSnr;		//																			//A[i].AS.bEMGSnr				=0;
	BOOL	bEZSnr;			//																			//A[i].AS.bEZSnr				=0;	
	BOOL	bRDYSnr;		//																			//A[i].AS.bRDYSnr				=0;
	long	lMCurPos;		//����ﵱǰ��λ�� (������������													//A[i].AS.lMCurPos				=0;
	long	lEncCurPos;		//���������ǰλ�� (������������													//A[i].AS.lEncCurPos			=0;
	BOOL	bDone	;		//��ʾ���Ƿ��Ѿ�ֹͣ������ bMComplet��bEComplet �����

}AXIS_STATUS;

typedef struct _AXIS_PARAM_
{	//28
										//Ĭ��=1000pulse/mm
		double	dPPU;					//�������嵱��  n pulse = 1 unit(mm��deg,m...)Equiv			
		int		nHomeMode;				//����ģʽ //0��һ�λ���//1��һ�λ���ӻ���//2�����λ���	//3��һ�λ�����ټ�һ�� EZ ������л���//4����һ�� EZ ������л���		
		int		nHomeDir;				//���㷽��0������1������		
		float	fHomeVel;				//�����ٶ�      pulse/s		
		float	fStartVel;				//��ʼ�ٶ�      pulse/s		
		float	fVel;					//���������ٶ�  pulse/s			
		float	fTAcc;					//����ʱ�� ��λs						
		float	fTDec;					//����ʱ�� ��λs				
		float	fTSmooth;				//S�ٶȶζ�ƽ��ʱ�� ��0��0.5s
		int		nAxsEncoderFactor;		//������ϵ���������Ǳ�����
		int		nAxsBandDist;			//��λ pulse //Ĭ�ϱ���ʱ��0.1s	
		int		nBacklash;				//�����϶ֵ pulse
		int		nPulseMode;				//=0 ,1 ,2 ,3  pulse+dir, //=4 ,5  ˫����ccw/cw 
		int		nSrPinLogic;			//=0 ���ø�Ӧ�ź���Ч��ƽ��0������Ч��1������Ч
		float	fTStop;					//Ĭ��=0.01 ��ͣ��ʱ�� ��λ��s
		BOOL	bPWMEnable;				//PWMʹ�ܣ�1��ʾPWM���ã�0��ʾ������

		BOOL	bAxsSvnEnable;			//��ʹ�ܣ�						1��ʾ�����ã�0��ʾ��ʹ��
		int		nAxsSvnEnablePinLogic;	//�����ŷ�ʹ����Ч��ƽ��		0������Ч��1������Ч	
		BOOL	bAxsInpEnable;			//��INP�ź�ʹ�ܣ�				1��ʾ�����ã�0��ʾ��ʹ��	
		int		nAxsInpPinLogic;		//���� INP �źŵ���Ч��ƽ��		0������Ч��1������Ч
		BOOL	bAxsAlmEnable;			//��ALM�ź�ʹ�ܣ�				1��ʾ�����ã�0��ʾ��ʹ��	
		int		nAxsAlmPinLogic;		//���� ALM �źŵ���Ч��ƽ��		0������Ч��Ĭ��1������Ч
		int		nAxsErcEnable;			//���� ERC �ź������			0���͵�ƽ��1���ߵ�ƽ
		int		nEncoderInMode;			//���ñ������ļ�����ʽ			0���� A/B ��(����/����) 1��1��A/B		2��2��A/B 		3��4��A/B
		int		nAxsEZPinLogic;			//���� EZ �źŵ���Ч��ƽ��		0������Ч��Ĭ��1������Ч
		int		nAxsEMGPinLogic;		//���� ��ͣEMG�źŵ���Ч��ƽ��	0������Ч��Ĭ��1������Ч		
		int		nEMGIOPort;				//ָ����ǰ��ӳ��Ϊ��ͣ�ĵ�ǰ����ͨ��Input�˿�
		int		nMotorType;				//ָ����ǰ���������ͣ�		0��ʾ������Ĭ��1��ʾ�ŷ�

}AXIS_PARAM;


//�� �ṹ����
typedef struct _AXIS_
{
	LPWSTR			lpwstrAxisName;	//��˵��
	short			sCardNo;		//����
	short			sAxisNo;		//���
	AXIS_STATUS		AS;				//��ĵ�ǰ״̬
	AXIS_PARAM		AP;				//��ĵ�ǰ����
}AXIS;

////�� �ṹ����
//typedef struct _CAN_STATE_
//{
//	short			sNodeNo[8];		//Can�ڵ�ţ�1-8��
//	short			sCanSts[8];		//��ĵ�ǰ״̬ CAN-IO ͨѶ״̬��0���Ͽ���1�����ӣ�2���쳣
//}CAN_STATE;


//���ñ�·��
//const wchar_t* M_MOTION_CARD_CFG_PATH	= L"M_CARD.cfg";			//�˶����ƿ����ñ�·��


//�� ����ֵ ����
typedef int ERROR_RESULT;
const int	MCARD_NO_ERROR	=0; //��ȷ ����
const int	MCARD_ERROR		=1; //���� ����

typedef unsigned int UINT;
typedef BOOL (CALLBACK* ASFR )(UINT);
typedef BOOL (CALLBACK* DSFR )(UINT,UINT);

typedef struct _CMP_STS_ //CompareSts
{
	BOOL			bEnable;		//�Ƿ����� һά����λ�ñȽ� 0����ֹ��1��ʹ��
	UINT			uCmpSource;		//�Ƚ�Դ 0��ָ��λ�ü�������1�������������� 
	LONG			lCurPos;		//��ǰ�Ƚϵ�λ��
	LONG			lCmpNum;		//�Ѿ��ȽϹ��ĵ���
	LONG			lCanAddNum;		//���Լ�����µ���

}CMP_STS;
//uGetNo	
const	UINT  GETNO_ABLE_SRC	=0	;//�ȽϿ���״̬���Ƚ�Դ 
const	UINT  GETNO_CUR_POS		=1	;//��ǰ�Ƚϵ�λ�� 
const	UINT  GETNO_CMP_NUM		=2	;//�Ѿ��ȽϹ��ĵ���
const	UINT  GETNO_CAN_ADD_NUM	=3	;//���Լ���ıȽϵ���


class CMCard
{
public:
///������GetXXX������ȡ������ֵ����������
///////////////////////////////////////////////////////////////////
	OUTPUT_PORT		O[MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX];		//��ǰDO���
	INPUT_PORT		I[MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * INPUT_PORT_COUNT_MAX ];		//��ǰDi����
	AXIS			A[MCARD_COUNT_MAX * AXIS_COUNT_MAX];//���
	CMP_STS			CS[MCARD_COUNT_MAX * AXIS_COUNT_MAX];
	int				CANSTS[MCARD_COUNT_MAX][EX_IONODE_COUNT_MAX+1];//���ţ��ڵ��
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	CMCard(void);
	~CMCard(void);
	//int GetLastError();//
	BOOL	m_bMotionCardOpenning;

	//ʵ�ʼ�⵽�Ŀ�����������ֹ���ù���
	int		m_nActualCardCount;

private:
	BOOL	m_bAutoHomming[MCARD_COUNT_MAX * AXIS_COUNT_MAX];		//����GetHomeAxis2Sts�ڲ� ����
	unsigned short	m_EncSnsValue;				//�������ļ������� �ݴ����

	//��������ŷֽ�Ϊ����Ŀ��ţ����
	//ERROR_RESULT BeAxisExist(UINT uAxsIndex ,short* sCardNo, short* sAxisNo);
	////������IO�ŷֽ�Ϊ����Ŀ��ţ��ڵ�ţ��˿ں�
	//ERROR_RESULT BeIOExist  (UINT uIOIndex  ,short* sCardNo, short* sNodeNo, short* sPortNo);
public:
	
	//���ð�ȫ�ص�***************************************************************************************
	ASFR m_pAxsSafeFilter;
	DSFR m_pDoSafeFilter;	

	//�����û�����ķ�ײ����⺯�������������������̣��˶����ƿ�����ÿ�ζ���ʱ����������
	//�����ᰲȫ�ص�����
	BOOL SetAxsSafeFilter(BOOL (CALLBACK* lpfnAxsSafeFilter)(UINT));

	//����IO��ȫ�ص�����
	BOOL SetDoSafeFilter(BOOL (CALLBACK* lpfnDoSafeFilter)(UINT,UINT));
	
	BOOL DebugSupperAxsSafeFilter(UINT uAxsIndex);
	BOOL DebugSupperDoSafeFilter(UINT uIOIndex, UINT uValue);
	//***************************************************************************************************

	////////////////////////////////////////////////////////////////////////////////////////
	//1.//���˶�������
	CWnd* m_pMBOwner;
	ERROR_RESULT OpenMotionCard(CWnd* pMBOwner );
	ERROR_RESULT CloseMotionCard();
	ERROR_RESULT SetMBOwner(CWnd* pMBOwner);
	ERROR_RESULT MB(LPCTSTR lpcsText);

	//��AP[AXIS_COUNT_MAX]���������õ�����
	ERROR_RESULT SetAxisParam(UINT uAxsIndex);
	/////////////////////////////////////////////////////////////////////////////////////////
	//�Զ���ԭ��1
	ERROR_RESULT HomeAxis1(UINT uAxsIndex);
	////��ȡ�Զ���ԭ��״̬�����Զ���ԭ��ִ�н����Ƿ���ɣ�
	//ERROR_RESULT GetHomeAxis1Sts(UINT uAxsIndex);	

	////�Զ���ԭ��2 nHomeDir =1 ����nVelMode =0 ���٣�nHomeMode =4 Z�����
	ERROR_RESULT HomeAxis2(UINT uAxsIndex,int nHomeDir,int nVelMode,int nHomeMode);
	////��ȡ�Զ���ԭ��״̬�����Զ���ԭ��ִ�н����Ƿ���ɣ�
	//ERROR_RESULT GetHomeAxis2Sts(UINT uAxisNo);	


	//����ԭ��
	ERROR_RESULT StartHomeLatch(UINT uAxsIndex);
	ERROR_RESULT GetHomeLatchSts(UINT uAxsIndex,BOOL &bSucceed, long &lPos);
	ERROR_RESULT ResetHomeLatchSts(UINT uAxsIndex);
	ERROR_RESULT EndHomeLatch(UINT uAxsIndex);

	//����ָ������ ---�������

	//һά����λ�ñȽ�10KHz//*************************************************************************************
	ERROR_RESULT StartCompare(UINT uAxsIndex,UINT uCmpSrc);	//����ָ�����һάλ�ñȽϡ� �Ƚ�Դ��0��ָ��λ�ü�������1��������������
	ERROR_RESULT ClearAllComparePoints(UINT uAxsIndex);	//���ָ���������һάλ�ñȽϵ�
	
	//���һ���µ�һάλ�ñȽϵ�
	//uCmpDir  :0 <=, 1>=;
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
	ERROR_RESULT AddNewComparePoint(UINT uAxsIndex,UINT uCmpDir,LONG lNewPointPos,UINT	uFunNo,UINT	uOutPortNo);
	
	//��ѯ���µ�ǰ��Ƚ�����״̬��
	//uGetNo	
	//GETNO_ABLE_SRC	=0	�ȽϿ���״̬���Ƚ�Դ 
	//GETNO_CUR_POS		=1	��ǰ�Ƚϵ�λ�� 
	//GETNO_CMP_NUM		=2	�Ѿ��ȽϹ��ĵ���
	//GETNO_CAN_ADD_NUM	=3	���Լ���ıȽϵ���
	ERROR_RESULT GetCompareSts(UINT uAxsIndex,UINT uGetNo);
	ERROR_RESULT EndCompare(UINT uAxsIndex);//�ر�ָ�����һάλ�ñȽ�
	//************************************************************************************************************


	//������λ��
	ERROR_RESULT SetAxisCurPos(UINT uAxsIndex,long lPos);//������ĵ�ǰλ��
	ERROR_RESULT SetAxisEncPos(UINT uAxsIndex, long dPos);//�����������λ��
	//ERROR_RESULT ZeroAxisPos(UINT uAxsIndex);	//������λ�ú���Ӧ������λ��Ϊ0 ��������Ư�ƣ�

	//��ȡ��ĵ�ǰλ��
	ERROR_RESULT GetAxisCurPos(UINT uAxsIndex);
	ERROR_RESULT GetAxisEncPos(UINT uAxsIndex);
	
	//��ȡ��ĵ�ǰ״̬������ ԭ��/��������״̬����/������λ�ã�����ź�״̬��
	ERROR_RESULT GetAxisStatus(UINT uAxsIndex);

	//���λ���ƶ�
	ERROR_RESULT RMove(UINT uAxsIndex, long lDist);
	ERROR_RESULT RMove2(UINT uAxsIndex, float fVel,long lDist);
	
	//����λ���ƶ�
	ERROR_RESULT AMove(UINT uAxsIndex, long lPos);
	ERROR_RESULT AMove2(UINT uAxsIndex, float fVel,long lPos);

	//Jog�˶�//�ı�JOG�ٶ�
	ERROR_RESULT JogMove(UINT uAxsIndex, float fVel, BOOL bDir);
	//�ı�JOG�ٶ�
	//ERROR_RESULT SetJogVel(UINT uAxsIndex, float fVel, BOOL bDir);

	ERROR_RESULT StopAxis(UINT uAxsIndex, UINT uMode=0);//=1��ͣ��=0Ϊ����ͣ //GT_Stop
	ERROR_RESULT EnableAxis(UINT uAxsIndex, BOOL bYes=1);

	//�������
	ERROR_RESULT SetDoBit(UINT uIOIndex, UINT uSetValue);
	ERROR_RESULT GetDiBit(UINT uIOIndex);
	ERROR_RESULT GetAllDi();
	//��ȡ����˿�״̬     Add in 2019.3.3 By Gogol
	ERROR_RESULT GetAllDo();

	//����һ��������
	ERROR_RESULT SetDoBitReverse(UINT uIOIndex, UINT uSetValue,short sTime); // 1�ߵ�ƽ ��0 �͵�ƽ // sTime 

	char* wc2c(CString &value);

	//���������Ϣ
	CString m_strLastErrorInfo;
	long    m_lErrCount;

	//���������Ϣ
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
