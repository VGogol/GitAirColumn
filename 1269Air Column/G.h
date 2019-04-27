#pragma once
#include"MCard.h"
#include"HalconCpp.h"
#include"FileRW.h"
#include"afxdb.h"
#include "afxcmn.h"
#include<string>


using   namespace     HalconCpp;
using   namespace     ADOCG;
using   namespace     std;                            //string类位于此名称空间中

//参数结构体
typedef  struct   _PARAM_
{
	CString        type;                                //参数类型
	CString        paraname;                       //参数名
	double         paravalue;                       //参数值
	double         paraMaxValue;                 //参数最大值
	double         paraMinValue;                 //参数最小值
	CString        remark;                             //备注
}PARAM;

//数据库智能指针结构体
typedef   struct    _DB_
{
	//添加一个指向Connection的指针  
	_ConnectionPtr      m_pConnection;
	//添加一个指向Recordset对象的指针  
	_RecordsetPtr        m_pRecordset;
	_variant_t               RecordsAffected;
	_CatalogPtr            m_pCatalog;
	CString                   _conn_;
}DB;

//产品信息结构体，包括几个尺寸，外观和最终判定结果
//某个时刻可能某项结果为空（不用）
typedef    struct    _Product_
{
	double     Length;                         //产品总长
	bool         AppearanceOK1;           //外观OK1-总长视角毛刺
	double     LeftDiameter;               //产品左边外径
	double     RightDiameter;            //产品右边外径
	bool         AppearanceOK2;         //外观OK2-左侧毛刺，堵孔
	bool         AppearanceOK3;        //外观OK3-右侧毛刺，堵孔
	bool         OK;                            //产品最终结果，1为OK
}Product;


//产品正方向特征（总长，外观）
typedef    struct    _ProductPositive_
{
	double      TotalLength;                      //产品总长
	bool          ApperanceOK;                   //外观OK（正方向毛刺），为1表示外观OK
}ProductPositive;

//产品侧方向特征（外径，外观）
typedef     struct    _ProductLateral_
{
	double      Diameter;                           //产品直径
	BOOL        AppearanceOK;        //产品OK（侧方向毛刺、堵孔、孔不圆），为1表示外观OK
}ProductLateral;

//处理结果链表
typedef    struct    _DisposeResult_
{
	bool       OK;                                    //是否是良品（1代表OK，0位NG）
}DisposeResult;

//存图列表结构体
typedef   struct    _Image_Linked
{
	HObject   image;
}ImageLinked;

static   const      short       AXIS_LEFT = 0;                        //左边轴
static   const      short       AXIS_RIGHT = 1;                     //右边轴

//参数总个数
static   const      int             All_Para_Counts=160;             //参数个数，默认100个
static   const      int             MotionParaCounts = 30;          //运动参数个数,算上备用
static   const      int             CameraParaCounts = 60;          //相机参数个数，算上 备用
static   const      int             VisionParaCounts = 60;             //视觉参数个数，算上备用

static   const      int              RunningTimeOut = 5000;             //动作超时报警时间
static   const      CString       ParaTableName = L"Para";           //生产记录表格名
//static   const      CString       LeftUPModelPath = L"./LeftUP.shm";
//static   const      CString       RightUPModelPath = L"./RightUP.shm";
static   const      string       LeftUPModelPath = "C:/Users/Administrator/Desktop/1269左相机上模板.shm";
static   const      string       RightUPModelPath = "C:/Users/Administrator/Desktop/1269右相机上模板.shm";
//static   const         int       SaveResultMin = 50;                   //将尺寸链表写入文件的最小链表项数

static  const      int            AppearanceNG = 0;                     //外观不良
static  const      int            AppearanceOK = 1;                     //外观OK

///////////////////////////////////      输入点位                      /////////////////////////////////
static         const           int              SCRAM_BUTTON = 0;                  //急停按钮
static         const           int              START_BUTTON = 1;                   //启动按钮
static         const           int              LEFTFIBERFULL = 3;             //左满料光纤
static         const           int              RIGHTFIBERFULL = 4;                 //右满料光纤
static         const           int              LEFTFIBERHALF = 5;             //左半料光纤
static         const           int              RIGHTFIBERHALF = 6;              //右半料光纤



///////////////////////////////            输出点位                     ////////////////////////////////////////
static         const           int             RED_LED=0;                         //三色灯红灯
static         const           int             GREEN_LED=1;                    //三色灯绿灯
static         const           int             YELLOW_LED=2;                  //三色灯黄灯
static         const           int             BUZZER=3;                        //蜂鸣器
static         const           int             LEFTRADIOTUBENG=4;        //左NG电磁阀
static         const           int             RIGHTRADIOTUBENG = 5;    //右NG电磁阀
static         const           int             TRIGGER1 = 14;                       //相机1触发
static         const           int             TRIGGER2 = 15;                       //相机2触发

////////////////////////////////          参数下标          ///////////////////////////////////
//运动参数
static      const     short      MP_LeftRstSpeed = 0;                 //运动参数-左产品复位速度
static      const     short      MP_LeftRunSpeed = 1;		           //运动参数-左产品自动运行速度 
static      const     short      MP_LeftCamFirstPos = 2;			    //运动参数-左相机初始拍照位置 
static      const     short      MP_LeftCamIntervalDis = 3;			//运动参数-左相机拍照间隔距离
static      const     short      MP_LeftNGBlowPos = 5;		       //运动参数-左NG吹气位置 
static      const     short      MP_RightRstSpeed = 6;			    //运动参数-右产品复位速度
static      const     short      MP_RightRunSpeed = 7;		    	//运动参数-右产品自动运行速度
static      const     short      MP_RightCamFirstPos = 8;			//运动参数-右产品初始拍照位置
static      const     short      MP_RightCamIntervalDis = 9;		//运动参数-右产品拍照间隔距离
static      const     short      MP_RightNGBlowPos = 11;		    //运动参数-右NG吹气位置 
static      const     short      MP_RstTimeout = 12;                  //运动参数-整机复位超时时间
//相机参数
static      const     short      CP_Cam1Gain = 30;			             //相机参数-相机1增益
static      const     short      CP_Cam1ExposureTime = 31;		//相机参数-相机1曝光时间
static      const     short      CP_Cam1Width = 32;			        //相机参数-相机1视野宽
static      const     short      CP_Cam1Height = 33;			        //相机参数-相机1视野高

static      const     short      CP_Cam2Gain = 40;			             //相机参数-相机2增益
static      const     short      CP_Cam2ExposureTime = 41;		//相机参数-相机2曝光时间
static      const     short      CP_Cam2Width = 42;			        //相机参数-相机2视野宽
static      const     short      CP_Cam2Height = 43;			        //相机参数-相机2视野高

static      const     short      CP_Cam3Gain = 50;			             //相机参数-相机3增益
static      const     short      CP_Cam3ExposureTime = 51;		//相机参数-相机3曝光时间
static      const     short      CP_Cam3Width = 52;			        //相机参数-相机3视野宽
static      const     short      CP_Cam3Height = 53;			        //相机参数-相机3视野高

static      const     short      CP_Cam4Gain = 60;			             //相机参数-相机4增益
static      const     short      CP_Cam4ExposureTime = 61;		//相机参数-相机4曝光时间
static      const     short      CP_Cam4Width = 62;			        //相机参数-相机4视野宽
static      const     short      CP_Cam4Height = 63;			        //相机参数-相机4视野高

static      const     short      CP_Cam5Gain = 70;			             //相机参数-相机5增益
static      const     short      CP_Cam5ExposureTime = 71;		//相机参数-相机5曝光时间
static      const     short      CP_Cam5Width = 72;			        //相机参数-相机5视野宽
static      const     short      CP_Cam5Height = 73;			        //相机参数-相机5视野高

static      const     short      CP_Cam6Gain = 80;			             //相机参数-相机6增益
static      const     short      CP_Cam6ExposureTime = 81;		//相机参数-相机6曝光时间
static      const     short      CP_Cam6Width = 82;			        //相机参数-相机6视野宽
static      const     short      CP_Cam6Height = 83;			        //相机参数-相机6视野高
//视觉参数
static     const     short      VP_Vision1ThresLow = 90;			 //视觉参数-视觉1阈值低值
static     const     short      VP_Vision1ThresHigh = 91;		    //视觉参数-视觉1阈值高值

static     const     short      VP_LeftOKLengthLow = 92;		   //视觉参数-左产品总长OK最小尺寸
static     const     short      VP_LeftOKLengthHigh = 93;			//视觉参数-左产品总长OK最大尺寸
static     const     short      VP_LeftOKDiameterLow = 94;	  //视觉参数-左产品外径OK最小尺寸
static     const     short      VP_LeftOKDiameterHigh = 95;	  //视觉参数-左产品外径OK最大尺寸

static     const     short      VP_RightOKLengthLow = 122;	     //视觉参数-右产品总长OK最小尺寸
static     const     short      VP_RightOKLengthHigh = 123;		//视觉参数-右产品总长OK最大尺寸
static     const     short      VP_RightOKDiameterLow = 124;   //视觉参数-右产品外径OK最小尺寸
static     const     short      VP_RightOKDiameterHigh = 125;  //视觉参数-右产品外径OK最大尺寸

static     const     short      PI_TotalCounts = 150;              //生产信息-总产量
static     const     short      PI_OKCounts = 151;                 //生产信息-OK数量
static     const     short      PI_NGCounts = 152;                //生产信息-NG数量
static     const     short      PI_Yield = 153;                        //生产信息-良率
static     const     short      PI_CT = 154;                           //生产信息-CT
static     const     short      PI_UPH = 155;                        //生产信息-UPH


class G
{
public:
	G();
	~G();

	//全局变量
public:
	static             CMCard                 MCard;

	///////////////////////////////////////////参数相关 
	//相机和视觉参数已使用个数，用于在参数对话框时显示参数用
	static              int        MotionUsed;                      //运动参数使用个数
	static              int        Camera1Used;                  //相机1参数已使用个数
	static              int        Camera2Used;                  //相机2参数已使用个数
	static              int        Camera3Used;                  //相机3参数已使用个数
	static              int        Camera4Used;                  //相机4参数已使用个数
	static              int        Camera5Used;                   //相机5参数已使用个数
	static              int        Camera6Used;                   //相机6参数已使用个数
	static              int        Vision1Used;                     //视觉1参数已使用个数
	static              int        Vision2Used;                     //视觉2参数已使用个数
	static              int        Vision3Used;                     //视觉3参数已使用个数
	static              int        Vision4Used;                     //视觉4参数已使用个数
	static              int        Vision5Used;                     //视觉5参数已使用个数
	static              int        Vision6Used;                     //视觉6参数已使用个数


public:
	static              PARAM                   PR[All_Para_Counts];          
	static              DB				             db;
	static              CCriticalSection       m_criticalsection_imagelist1;         //图像列表1临界(左相机-尺寸)
	static              CCriticalSection       m_criticalsection_imagelist2;         //图像列表2临界(左相机-左外观)
	static              CCriticalSection       m_criticalsection_imagelist3;        //图像列表3临界(左相机-右外观)
	static              CCriticalSection       m_criticalsection_imagelist4;        //图像列表4临界(右相机-尺寸)
	static              CCriticalSection       m_criticalsection_imagelist5;        //图像列表5临界(右相机-左外观)
	static              CCriticalSection       m_criticalsection_imagelist6;        //图像列表6临界(右相机-右外观)
	//static              CCriticalSection       m_criticalsection_productlist1;         //产品列表1临界
	//static              CCriticalSection       m_criticalsection_productlist2;         //产品列表2临界

	static          CCriticalSection       m_criticalsection_LeftPositive;      //左产品正向临界
	static          CCriticalSection       m_criticalsection_RightPositive;   //右产品正向临界
	static          CCriticalSection       m_criticalsection_LeftLateral1;     //左产品左侧临界
	static          CCriticalSection       m_criticalsection_LeftLateral2;     //左产品右侧临界
	static          CCriticalSection       m_criticalsection_RightLateral1;   //右产品左侧临界
	static          CCriticalSection       m_criticalsection_RightLateral2;   //右产品右侧临界
	static          CCriticalSection       m_criticalsection_LeftResultlist;	  //左产品结果临界
	static          CCriticalSection       m_criticalsection_RightResultlist;//右产品结果临界
	//static          CCriticalSection       m_criticalsection_sizelist1tofile;  //数据到文件列表1临界
	//static          CCriticalSection       m_criticalsection_sizelist2tofile;  //数据到文件列表2临界

public:
    static              HTuple             hv_AcqHandle1;                 //相机1(左相机-总长)
	static              HTuple             hv_AcqHandle2;                //相机2（左相机-左端外径外观）
	static              HTuple             hv_AcqHandle3;                 //相机3（左相机-右端外径外观）
	static              HTuple             hv_AcqHandle4;                //相机4（右相机-总长）
	static              HTuple             hv_AcqHandle5;                //相机5（右相机-左端外径外观）
	static              HTuple             hv_AcqHandle6;                //相机6（右相机-右端外径外观）
				      
	static               HTuple            HW1;                                //窗口1(左窗口-总长)
	static               HTuple            HW2;                                //窗口2（左窗口-左端外径外观）
	static               HTuple            HW3;                               //窗口3（左窗口-右端外径外观）
	static               HTuple            HW4;                               //窗口4（右窗口-总长）
	static               HTuple            HW5;                               //窗口5（右窗口-左端外径外观）
	static               HTuple            HW6;                               //窗口6（右窗口-右端外径外观）
				      
	static               CString            ParaFileName;                        //保存参数文件名
				      
	static               BOOL              bRunning;                              //运行中标志   
	static               BOOL              bResetOK;                              //复位完成
	static               BOOL              bCanGrab;                              //允许拍照
	static               bool                bAlarm;                             //报警标志
	static               bool                bLeftNoShield;                       //左端没有屏蔽，可以执行
	static               bool                bRightNoShield;                     //右端没有屏蔽，可以执行
	static               bool                bRunflag;                             //自动流程中标志，用于限制单轴手动
	static               bool                bLeftRstOK;                     //左边轴复位完成
	static               bool                bRightRstOK;                      //右边轴复位完成

	static              int                  UserAuthority;                             //当前登录用户权限

	static              BOOL               bOpenExcelOk;                     //Excel打开成功

	static              HTuple             modle1;                                 //模板1，左尺寸模板

	static              LONGLONG     GridCounts1;                   //已走的格数1，用于左相机触发计数
	static              LONGLONG     GridCounts2;                  //已走的格数2，用于右相机触发计数

	static             long                 grab1counts;                          //图像1抓图数量
	static             long                 grab2counts;                          //图像2抓图数量
	static             long                 grab3counts;                          //图像3抓图数量
	static             long                 grab4counts;                          //图像4抓图数量
	static             long                 grab5counts;                          //图像5抓图数量
	static             long                 grab6counts;                          //图像6抓图数量
	static             long                 grab1delete;                           //图像1删除数量
	static             long                 grab2delete;                           //图像2删除数量
	static             long                  triggercounts1;                      //触发次数1，左相机用
	static             long                  triggercounts2;                        //触发次数2，右相机用
	static             long                  blow1;                                     //吹产品次数1，左吹气用
	static             long                  blow2;                                    //吹产品次数2，右吹气用
	static             long                  LeftNullCounts;                 //左侧空产品数，用于计算单边屏蔽停止
	static             long                  RightNullCounts;             //右侧空产品数，用于计算单边屏蔽停止

	///////////////用于实时显示产品尺寸
	static             double              LeftLength;                            //当前左产品总长
	static             double              LeftDiameter;                        //当前左产品外径
	static             double              RightLength;                          //当前右产品总长
	static             double              RightDiameter;                       //当前右产品外径

	///////////////////////               列表                               ////////////////////////////////////
	static         CList<ImageLinked, ImageLinked&>Imagelist1;          //图像列表1 左产品尺寸
	static         CList<ImageLinked, ImageLinked&>Imagelist2;          //图像列表2 左产品左端
	static         CList<ImageLinked, ImageLinked&>Imagelist3;          //图像列表3 左产品右端
	static         CList<ImageLinked, ImageLinked&>Imagelist4;          //图像列表4 右产品尺寸
	static         CList<ImageLinked, ImageLinked&>Imagelist5;          //图像列表5 右产品左端
	static         CList<ImageLinked, ImageLinked&>Imagelist6;          //图像列表6 右产品右端
	//static         CList<Product, Product&>ProductList1;                   //产品列表1-左侧产品
	//static         CList<Product, Product&>ProductList2;                   //产品列表2-右侧产品

	static   CList<ProductPositive, ProductPositive&>LeftPositiveList;      //左产品正向列表
	static   CList<ProductPositive, ProductPositive&>RightPositiveList;    //右产品正向特征列表
	static   CList<ProductLateral, ProductLateral&>LeftLateralList1;      //左产品左侧列表
	static   CList<ProductLateral, ProductLateral&>LeftLateralList2;     //左产品右侧列表
	static   CList<ProductLateral, ProductLateral&>RightLateralList1;   //右产品左侧列表
	static   CList<ProductLateral, ProductLateral&>RightLateralList2;  //右产品右侧列表
	static   CList<DisposeResult, DisposeResult&>LeftResultlist;        //左产品检测结果列表
	static   CList<DisposeResult, DisposeResult&>RightResultlist;        //右产品检测结果列表
	//static         CList<CheckSize, CheckSize&>SizelistToFile1;         //检测尺寸结果存文件链表1
	//static         CList<CheckSize, CheckSize&>SizelistToFile2;         //检测尺寸结果存文件链表2


	// 轴回原点函数
	//轴号，复位速度
static	         int         GoHome(int AxisNum, long  RstSpeed);
// 打开并初始化相机
 static            int         OpenCamera();
 // 初始化参数
static             BOOL     InitParam();
// 保存参数
static            int           WriteCommonPara();
// 保存IO端口名
static            int            WriteIOPara();
//相机触发
static            int            CameraTrigger(int CameraNo);
// 写入生产信息
static            int             WriteProduction();
// 删除链表内容
static            int             ClearLinked();
//更新颜色
static             int            UpdateColor(CDialog* pdlg, UINT uid, UINT uvalue); 
//设置控件颜色
static             int            SetCtlColor(CDialog* pdlg, UINT uid, COLORREF color);     
// double转string型
static             CString     double_string(double xx);
// string转double型
static            double       string_double(CString xx);

// 图像处理1（左产品尺寸）
static            int              ImageDispose1(HObject  ho_image, HTuple hv_modle);
// 图像处理2（右产品尺寸）
static            int              ImageDispose2(HObject  ho_image, HTuple hv_modle);
// 创建数据库
static            int              CreateDatabase(CString dbname);
// 在数据库中创建表格
//表名
static            int              CreateTable(CString TableName);

static            void          disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);

static           void           dev_display_shape_matching_results(HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row, HTuple hv_Column, HTuple hv_Angle, HTuple hv_ScaleR, HTuple hv_ScaleC, HTuple hv_Model,HTuple  WindowHandle);

// 写入一个类型的参数
static            int           WriteSerialPara(short SerialNo);
// 再次读取参数时使用
static           int            ReadPara2();
};

