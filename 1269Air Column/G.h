#pragma once
#include"MCard.h"
#include"HalconCpp.h"
#include"FileRW.h"
#include"afxdb.h"
#include "afxcmn.h"
#include<string>


using   namespace     HalconCpp;
using   namespace     ADOCG;
using   namespace     std;                            //string��λ�ڴ����ƿռ���

//�����ṹ��
typedef  struct   _PARAM_
{
	CString        type;                                //��������
	CString        paraname;                       //������
	double         paravalue;                       //����ֵ
	double         paraMaxValue;                 //�������ֵ
	double         paraMinValue;                 //������Сֵ
	CString        remark;                             //��ע
}PARAM;

//���ݿ�����ָ��ṹ��
typedef   struct    _DB_
{
	//���һ��ָ��Connection��ָ��  
	_ConnectionPtr      m_pConnection;
	//���һ��ָ��Recordset�����ָ��  
	_RecordsetPtr        m_pRecordset;
	_variant_t               RecordsAffected;
	_CatalogPtr            m_pCatalog;
	CString                   _conn_;
}DB;

//��Ʒ��Ϣ�ṹ�壬���������ߴ磬��ۺ������ж����
//ĳ��ʱ�̿���ĳ����Ϊ�գ����ã�
typedef    struct    _Product_
{
	double     Length;                         //��Ʒ�ܳ�
	bool         AppearanceOK1;           //���OK1-�ܳ��ӽ�ë��
	double     LeftDiameter;               //��Ʒ����⾶
	double     RightDiameter;            //��Ʒ�ұ��⾶
	bool         AppearanceOK2;         //���OK2-���ë�̣��¿�
	bool         AppearanceOK3;        //���OK3-�Ҳ�ë�̣��¿�
	bool         OK;                            //��Ʒ���ս����1ΪOK
}Product;


//��Ʒ�������������ܳ�����ۣ�
typedef    struct    _ProductPositive_
{
	double      TotalLength;                      //��Ʒ�ܳ�
	bool          ApperanceOK;                   //���OK��������ë�̣���Ϊ1��ʾ���OK
}ProductPositive;

//��Ʒ�෽���������⾶����ۣ�
typedef     struct    _ProductLateral_
{
	double      Diameter;                           //��Ʒֱ��
	BOOL        AppearanceOK;        //��ƷOK���෽��ë�̡��¿ס��ײ�Բ����Ϊ1��ʾ���OK
}ProductLateral;

//����������
typedef    struct    _DisposeResult_
{
	bool       OK;                                    //�Ƿ�����Ʒ��1����OK��0λNG��
}DisposeResult;

//��ͼ�б�ṹ��
typedef   struct    _Image_Linked
{
	HObject   image;
}ImageLinked;

static   const      short       AXIS_LEFT = 0;                        //�����
static   const      short       AXIS_RIGHT = 1;                     //�ұ���

//�����ܸ���
static   const      int             All_Para_Counts=160;             //����������Ĭ��100��
static   const      int             MotionParaCounts = 30;          //�˶���������,���ϱ���
static   const      int             CameraParaCounts = 60;          //����������������� ����
static   const      int             VisionParaCounts = 60;             //�Ӿ��������������ϱ���

static   const      int              RunningTimeOut = 5000;             //������ʱ����ʱ��
static   const      CString       ParaTableName = L"Para";           //������¼�����
//static   const      CString       LeftUPModelPath = L"./LeftUP.shm";
//static   const      CString       RightUPModelPath = L"./RightUP.shm";
static   const      string       LeftUPModelPath = "C:/Users/Administrator/Desktop/1269�������ģ��.shm";
static   const      string       RightUPModelPath = "C:/Users/Administrator/Desktop/1269�������ģ��.shm";
//static   const         int       SaveResultMin = 50;                   //���ߴ�����д���ļ�����С��������

static  const      int            AppearanceNG = 0;                     //��۲���
static  const      int            AppearanceOK = 1;                     //���OK

///////////////////////////////////      �����λ                      /////////////////////////////////
static         const           int              SCRAM_BUTTON = 0;                  //��ͣ��ť
static         const           int              START_BUTTON = 1;                   //������ť
static         const           int              LEFTFIBERFULL = 3;             //�����Ϲ���
static         const           int              RIGHTFIBERFULL = 4;                 //�����Ϲ���
static         const           int              LEFTFIBERHALF = 5;             //����Ϲ���
static         const           int              RIGHTFIBERHALF = 6;              //�Ұ��Ϲ���



///////////////////////////////            �����λ                     ////////////////////////////////////////
static         const           int             RED_LED=0;                         //��ɫ�ƺ��
static         const           int             GREEN_LED=1;                    //��ɫ���̵�
static         const           int             YELLOW_LED=2;                  //��ɫ�ƻƵ�
static         const           int             BUZZER=3;                        //������
static         const           int             LEFTRADIOTUBENG=4;        //��NG��ŷ�
static         const           int             RIGHTRADIOTUBENG = 5;    //��NG��ŷ�
static         const           int             TRIGGER1 = 14;                       //���1����
static         const           int             TRIGGER2 = 15;                       //���2����

////////////////////////////////          �����±�          ///////////////////////////////////
//�˶�����
static      const     short      MP_LeftRstSpeed = 0;                 //�˶�����-���Ʒ��λ�ٶ�
static      const     short      MP_LeftRunSpeed = 1;		           //�˶�����-���Ʒ�Զ������ٶ� 
static      const     short      MP_LeftCamFirstPos = 2;			    //�˶�����-�������ʼ����λ�� 
static      const     short      MP_LeftCamIntervalDis = 3;			//�˶�����-��������ռ������
static      const     short      MP_LeftNGBlowPos = 5;		       //�˶�����-��NG����λ�� 
static      const     short      MP_RightRstSpeed = 6;			    //�˶�����-�Ҳ�Ʒ��λ�ٶ�
static      const     short      MP_RightRunSpeed = 7;		    	//�˶�����-�Ҳ�Ʒ�Զ������ٶ�
static      const     short      MP_RightCamFirstPos = 8;			//�˶�����-�Ҳ�Ʒ��ʼ����λ��
static      const     short      MP_RightCamIntervalDis = 9;		//�˶�����-�Ҳ�Ʒ���ռ������
static      const     short      MP_RightNGBlowPos = 11;		    //�˶�����-��NG����λ�� 
static      const     short      MP_RstTimeout = 12;                  //�˶�����-������λ��ʱʱ��
//�������
static      const     short      CP_Cam1Gain = 30;			             //�������-���1����
static      const     short      CP_Cam1ExposureTime = 31;		//�������-���1�ع�ʱ��
static      const     short      CP_Cam1Width = 32;			        //�������-���1��Ұ��
static      const     short      CP_Cam1Height = 33;			        //�������-���1��Ұ��

static      const     short      CP_Cam2Gain = 40;			             //�������-���2����
static      const     short      CP_Cam2ExposureTime = 41;		//�������-���2�ع�ʱ��
static      const     short      CP_Cam2Width = 42;			        //�������-���2��Ұ��
static      const     short      CP_Cam2Height = 43;			        //�������-���2��Ұ��

static      const     short      CP_Cam3Gain = 50;			             //�������-���3����
static      const     short      CP_Cam3ExposureTime = 51;		//�������-���3�ع�ʱ��
static      const     short      CP_Cam3Width = 52;			        //�������-���3��Ұ��
static      const     short      CP_Cam3Height = 53;			        //�������-���3��Ұ��

static      const     short      CP_Cam4Gain = 60;			             //�������-���4����
static      const     short      CP_Cam4ExposureTime = 61;		//�������-���4�ع�ʱ��
static      const     short      CP_Cam4Width = 62;			        //�������-���4��Ұ��
static      const     short      CP_Cam4Height = 63;			        //�������-���4��Ұ��

static      const     short      CP_Cam5Gain = 70;			             //�������-���5����
static      const     short      CP_Cam5ExposureTime = 71;		//�������-���5�ع�ʱ��
static      const     short      CP_Cam5Width = 72;			        //�������-���5��Ұ��
static      const     short      CP_Cam5Height = 73;			        //�������-���5��Ұ��

static      const     short      CP_Cam6Gain = 80;			             //�������-���6����
static      const     short      CP_Cam6ExposureTime = 81;		//�������-���6�ع�ʱ��
static      const     short      CP_Cam6Width = 82;			        //�������-���6��Ұ��
static      const     short      CP_Cam6Height = 83;			        //�������-���6��Ұ��
//�Ӿ�����
static     const     short      VP_Vision1ThresLow = 90;			 //�Ӿ�����-�Ӿ�1��ֵ��ֵ
static     const     short      VP_Vision1ThresHigh = 91;		    //�Ӿ�����-�Ӿ�1��ֵ��ֵ

static     const     short      VP_LeftOKLengthLow = 92;		   //�Ӿ�����-���Ʒ�ܳ�OK��С�ߴ�
static     const     short      VP_LeftOKLengthHigh = 93;			//�Ӿ�����-���Ʒ�ܳ�OK���ߴ�
static     const     short      VP_LeftOKDiameterLow = 94;	  //�Ӿ�����-���Ʒ�⾶OK��С�ߴ�
static     const     short      VP_LeftOKDiameterHigh = 95;	  //�Ӿ�����-���Ʒ�⾶OK���ߴ�

static     const     short      VP_RightOKLengthLow = 122;	     //�Ӿ�����-�Ҳ�Ʒ�ܳ�OK��С�ߴ�
static     const     short      VP_RightOKLengthHigh = 123;		//�Ӿ�����-�Ҳ�Ʒ�ܳ�OK���ߴ�
static     const     short      VP_RightOKDiameterLow = 124;   //�Ӿ�����-�Ҳ�Ʒ�⾶OK��С�ߴ�
static     const     short      VP_RightOKDiameterHigh = 125;  //�Ӿ�����-�Ҳ�Ʒ�⾶OK���ߴ�

static     const     short      PI_TotalCounts = 150;              //������Ϣ-�ܲ���
static     const     short      PI_OKCounts = 151;                 //������Ϣ-OK����
static     const     short      PI_NGCounts = 152;                //������Ϣ-NG����
static     const     short      PI_Yield = 153;                        //������Ϣ-����
static     const     short      PI_CT = 154;                           //������Ϣ-CT
static     const     short      PI_UPH = 155;                        //������Ϣ-UPH


class G
{
public:
	G();
	~G();

	//ȫ�ֱ���
public:
	static             CMCard                 MCard;

	///////////////////////////////////////////������� 
	//������Ӿ�������ʹ�ø����������ڲ����Ի���ʱ��ʾ������
	static              int        MotionUsed;                      //�˶�����ʹ�ø���
	static              int        Camera1Used;                  //���1������ʹ�ø���
	static              int        Camera2Used;                  //���2������ʹ�ø���
	static              int        Camera3Used;                  //���3������ʹ�ø���
	static              int        Camera4Used;                  //���4������ʹ�ø���
	static              int        Camera5Used;                   //���5������ʹ�ø���
	static              int        Camera6Used;                   //���6������ʹ�ø���
	static              int        Vision1Used;                     //�Ӿ�1������ʹ�ø���
	static              int        Vision2Used;                     //�Ӿ�2������ʹ�ø���
	static              int        Vision3Used;                     //�Ӿ�3������ʹ�ø���
	static              int        Vision4Used;                     //�Ӿ�4������ʹ�ø���
	static              int        Vision5Used;                     //�Ӿ�5������ʹ�ø���
	static              int        Vision6Used;                     //�Ӿ�6������ʹ�ø���


public:
	static              PARAM                   PR[All_Para_Counts];          
	static              DB				             db;
	static              CCriticalSection       m_criticalsection_imagelist1;         //ͼ���б�1�ٽ�(�����-�ߴ�)
	static              CCriticalSection       m_criticalsection_imagelist2;         //ͼ���б�2�ٽ�(�����-�����)
	static              CCriticalSection       m_criticalsection_imagelist3;        //ͼ���б�3�ٽ�(�����-�����)
	static              CCriticalSection       m_criticalsection_imagelist4;        //ͼ���б�4�ٽ�(�����-�ߴ�)
	static              CCriticalSection       m_criticalsection_imagelist5;        //ͼ���б�5�ٽ�(�����-�����)
	static              CCriticalSection       m_criticalsection_imagelist6;        //ͼ���б�6�ٽ�(�����-�����)
	//static              CCriticalSection       m_criticalsection_productlist1;         //��Ʒ�б�1�ٽ�
	//static              CCriticalSection       m_criticalsection_productlist2;         //��Ʒ�б�2�ٽ�

	static          CCriticalSection       m_criticalsection_LeftPositive;      //���Ʒ�����ٽ�
	static          CCriticalSection       m_criticalsection_RightPositive;   //�Ҳ�Ʒ�����ٽ�
	static          CCriticalSection       m_criticalsection_LeftLateral1;     //���Ʒ����ٽ�
	static          CCriticalSection       m_criticalsection_LeftLateral2;     //���Ʒ�Ҳ��ٽ�
	static          CCriticalSection       m_criticalsection_RightLateral1;   //�Ҳ�Ʒ����ٽ�
	static          CCriticalSection       m_criticalsection_RightLateral2;   //�Ҳ�Ʒ�Ҳ��ٽ�
	static          CCriticalSection       m_criticalsection_LeftResultlist;	  //���Ʒ����ٽ�
	static          CCriticalSection       m_criticalsection_RightResultlist;//�Ҳ�Ʒ����ٽ�
	//static          CCriticalSection       m_criticalsection_sizelist1tofile;  //���ݵ��ļ��б�1�ٽ�
	//static          CCriticalSection       m_criticalsection_sizelist2tofile;  //���ݵ��ļ��б�2�ٽ�

public:
    static              HTuple             hv_AcqHandle1;                 //���1(�����-�ܳ�)
	static              HTuple             hv_AcqHandle2;                //���2�������-����⾶��ۣ�
	static              HTuple             hv_AcqHandle3;                 //���3�������-�Ҷ��⾶��ۣ�
	static              HTuple             hv_AcqHandle4;                //���4�������-�ܳ���
	static              HTuple             hv_AcqHandle5;                //���5�������-����⾶��ۣ�
	static              HTuple             hv_AcqHandle6;                //���6�������-�Ҷ��⾶��ۣ�
				      
	static               HTuple            HW1;                                //����1(�󴰿�-�ܳ�)
	static               HTuple            HW2;                                //����2���󴰿�-����⾶��ۣ�
	static               HTuple            HW3;                               //����3���󴰿�-�Ҷ��⾶��ۣ�
	static               HTuple            HW4;                               //����4���Ҵ���-�ܳ���
	static               HTuple            HW5;                               //����5���Ҵ���-����⾶��ۣ�
	static               HTuple            HW6;                               //����6���Ҵ���-�Ҷ��⾶��ۣ�
				      
	static               CString            ParaFileName;                        //��������ļ���
				      
	static               BOOL              bRunning;                              //�����б�־   
	static               BOOL              bResetOK;                              //��λ���
	static               BOOL              bCanGrab;                              //��������
	static               bool                bAlarm;                             //������־
	static               bool                bLeftNoShield;                       //���û�����Σ�����ִ��
	static               bool                bRightNoShield;                     //�Ҷ�û�����Σ�����ִ��
	static               bool                bRunflag;                             //�Զ������б�־���������Ƶ����ֶ�
	static               bool                bLeftRstOK;                     //����Ḵλ���
	static               bool                bRightRstOK;                      //�ұ��Ḵλ���

	static              int                  UserAuthority;                             //��ǰ��¼�û�Ȩ��

	static              BOOL               bOpenExcelOk;                     //Excel�򿪳ɹ�

	static              HTuple             modle1;                                 //ģ��1����ߴ�ģ��

	static              LONGLONG     GridCounts1;                   //���ߵĸ���1�������������������
	static              LONGLONG     GridCounts2;                  //���ߵĸ���2�������������������

	static             long                 grab1counts;                          //ͼ��1ץͼ����
	static             long                 grab2counts;                          //ͼ��2ץͼ����
	static             long                 grab3counts;                          //ͼ��3ץͼ����
	static             long                 grab4counts;                          //ͼ��4ץͼ����
	static             long                 grab5counts;                          //ͼ��5ץͼ����
	static             long                 grab6counts;                          //ͼ��6ץͼ����
	static             long                 grab1delete;                           //ͼ��1ɾ������
	static             long                 grab2delete;                           //ͼ��2ɾ������
	static             long                  triggercounts1;                      //��������1���������
	static             long                  triggercounts2;                        //��������2���������
	static             long                  blow1;                                     //����Ʒ����1��������
	static             long                  blow2;                                    //����Ʒ����2���Ҵ�����
	static             long                  LeftNullCounts;                 //���ղ�Ʒ�������ڼ��㵥������ֹͣ
	static             long                  RightNullCounts;             //�Ҳ�ղ�Ʒ�������ڼ��㵥������ֹͣ

	///////////////����ʵʱ��ʾ��Ʒ�ߴ�
	static             double              LeftLength;                            //��ǰ���Ʒ�ܳ�
	static             double              LeftDiameter;                        //��ǰ���Ʒ�⾶
	static             double              RightLength;                          //��ǰ�Ҳ�Ʒ�ܳ�
	static             double              RightDiameter;                       //��ǰ�Ҳ�Ʒ�⾶

	///////////////////////               �б�                               ////////////////////////////////////
	static         CList<ImageLinked, ImageLinked&>Imagelist1;          //ͼ���б�1 ���Ʒ�ߴ�
	static         CList<ImageLinked, ImageLinked&>Imagelist2;          //ͼ���б�2 ���Ʒ���
	static         CList<ImageLinked, ImageLinked&>Imagelist3;          //ͼ���б�3 ���Ʒ�Ҷ�
	static         CList<ImageLinked, ImageLinked&>Imagelist4;          //ͼ���б�4 �Ҳ�Ʒ�ߴ�
	static         CList<ImageLinked, ImageLinked&>Imagelist5;          //ͼ���б�5 �Ҳ�Ʒ���
	static         CList<ImageLinked, ImageLinked&>Imagelist6;          //ͼ���б�6 �Ҳ�Ʒ�Ҷ�
	//static         CList<Product, Product&>ProductList1;                   //��Ʒ�б�1-����Ʒ
	//static         CList<Product, Product&>ProductList2;                   //��Ʒ�б�2-�Ҳ��Ʒ

	static   CList<ProductPositive, ProductPositive&>LeftPositiveList;      //���Ʒ�����б�
	static   CList<ProductPositive, ProductPositive&>RightPositiveList;    //�Ҳ�Ʒ���������б�
	static   CList<ProductLateral, ProductLateral&>LeftLateralList1;      //���Ʒ����б�
	static   CList<ProductLateral, ProductLateral&>LeftLateralList2;     //���Ʒ�Ҳ��б�
	static   CList<ProductLateral, ProductLateral&>RightLateralList1;   //�Ҳ�Ʒ����б�
	static   CList<ProductLateral, ProductLateral&>RightLateralList2;  //�Ҳ�Ʒ�Ҳ��б�
	static   CList<DisposeResult, DisposeResult&>LeftResultlist;        //���Ʒ������б�
	static   CList<DisposeResult, DisposeResult&>RightResultlist;        //�Ҳ�Ʒ������б�
	//static         CList<CheckSize, CheckSize&>SizelistToFile1;         //���ߴ������ļ�����1
	//static         CList<CheckSize, CheckSize&>SizelistToFile2;         //���ߴ������ļ�����2


	// ���ԭ�㺯��
	//��ţ���λ�ٶ�
static	         int         GoHome(int AxisNum, long  RstSpeed);
// �򿪲���ʼ�����
 static            int         OpenCamera();
 // ��ʼ������
static             BOOL     InitParam();
// �������
static            int           WriteCommonPara();
// ����IO�˿���
static            int            WriteIOPara();
//�������
static            int            CameraTrigger(int CameraNo);
// д��������Ϣ
static            int             WriteProduction();
// ɾ����������
static            int             ClearLinked();
//������ɫ
static             int            UpdateColor(CDialog* pdlg, UINT uid, UINT uvalue); 
//���ÿؼ���ɫ
static             int            SetCtlColor(CDialog* pdlg, UINT uid, COLORREF color);     
// doubleתstring��
static             CString     double_string(double xx);
// stringתdouble��
static            double       string_double(CString xx);

// ͼ����1�����Ʒ�ߴ磩
static            int              ImageDispose1(HObject  ho_image, HTuple hv_modle);
// ͼ����2���Ҳ�Ʒ�ߴ磩
static            int              ImageDispose2(HObject  ho_image, HTuple hv_modle);
// �������ݿ�
static            int              CreateDatabase(CString dbname);
// �����ݿ��д������
//����
static            int              CreateTable(CString TableName);

static            void          disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);

static           void           dev_display_shape_matching_results(HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row, HTuple hv_Column, HTuple hv_Angle, HTuple hv_ScaleR, HTuple hv_ScaleC, HTuple hv_Model,HTuple  WindowHandle);

// д��һ�����͵Ĳ���
static            int           WriteSerialPara(short SerialNo);
// �ٴζ�ȡ����ʱʹ��
static           int            ReadPara2();
};

