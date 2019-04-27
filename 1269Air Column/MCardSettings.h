
////雷赛运动控制卡 5000系列  版本Ver1.32，

//调用此类的软件工程师 根据实际情况修改以下几个常量：
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//调用者修改
//最大主卡数设定；
const int   MCARD_COUNT_MAX				=1;	//一般只1张卡		//最大=8;			
//主卡最大轴数设定；								  
const int   AXIS_COUNT_MAX				=4;	//一般只4个轴		//最大=8;
//最大IO扩展节点数设定；(表示挂载在同一个主卡上的扩展上卡最多的卡数是多少）						
const int   EX_IONODE_COUNT_MAX			=1;	//一般只1个扩展		//最大=8;  
//节点输入端口最大数设定；							   
const int   INPUT_PORT_COUNT_MAX		=16;//一般16			//最大=16;
//节点输出端口最大数设定；							  
const int   OUTPUT_PORT_COUNT_MAX		=16;//一般16			//最大=16;
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//调用者修改

//IO索引号生成公式//调用者可了解一下
//O[ MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX ];		//当前DO输出
//I[ MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * INPUT_PORT_COUNT_MAX  ];		//当前Di输入

/*

//运动控制卡 安全回调函数模板 //全局的

//轴安全函数
BOOL CALLBACK myAxsSafeFun(UINT uAxs)
{
	//**********************************************
	// 此处添加 轴运动前时 安全控制代码
	//**********************************************
	return TRUE;
}

//IO输出安全函数
BOOL CALLBACK myDoSafeFun (UINT uIOIndex,UINT uValue)
{
	//**********************************************
	// 此处添加 IO 输出时 安全控制代码
	//**********************************************
	return TRUE;
}

*/



//DMC5000 系列运动控制卡的运动函数错误码见表 9.7。
//表 9.7 运动函数错误码
//错误码 名称 含义
//0 ERR_NOERR 成功
//1 ERR_UNKNOWN 未知错误
//2 ERR_PARAERR 参数错误
//3 ERR_TIMEOUT PCI 通讯超时
//4 ERR_CONTROLLERBUSY 轴正在运动
//6 ERR_CONTILINE 连续插补错误
//8 ERR_CANNOT_CONNECTETH 无法连接错误
//9 ERR_HANDLEERR 卡号链接错误
//10 ERR_SENDERR PCI 异常，请检查 PCI 槽位是否松动
//12 ERR_FIRMWAREERR 固件文件错误
//14 ERR_FIRMWAR_MISMATCH 固件不匹配
//20 ERR_FIRMWARE_INVALID_PARA_ERR 固件参数错误
//22 ERR_FIRMWARE_STATE_ERR 固件当前状态不允许操作
//24 ERR_FIRMWARE_CARD_NOT_SUPPORT 固件不支持的功能
//25 ERR_PASSWORD_ERR 密码错误
//26 ERR_PASSWORD_TIMES_OUT 密码错误输入次数受限
//50 ERR_LIST_NUM_ERR LIST 号错误
//51 ERR_LIST_NOT_OEPN LIST 不在打开状态
//52 ERR_PARA_NOT_VALID 参数不在有效范围
//53 ERR_LIST_HAS_OPEN LIST 已经打开
//54 ERR_MAIN_LIST_NOT_OPEN 主要的 LIST 没有初始化
//55 ERR_AXIS_NUM_ERR 轴数不在有效范围
//56 ERR_AXIS_MAP_ARRAY_ERR 轴映射表为空
//57 ERR_MAP_AXIS_ERR 映射轴错误
//58 ERR_MAP_AXIS_BUSY 映射轴忙
//59 ERR_PARA_SET_FORBIT 运动中不运行更改参数
//60 ERR_FIFO_FULL 缓冲区已满
//61 ERR_RADIUS_ERR 半径为 0 或小于两点的距离的一半
//62 ERR_MAINLIST_HAS_START ERR_MAINLIST_HAS_START
//63 ERR_ACC_TIME_ZERO 加减速时间为 0
//64 ERR_MAINLIST_NOT_START 主要 LIST 没有启动