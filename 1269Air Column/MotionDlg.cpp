// MotionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "MotionDlg.h"
#include "afxdialogex.h"


// CMotionDlg 对话框

IMPLEMENT_DYNAMIC(CMotionDlg, CDialogEx)

CMotionDlg::CMotionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMotionDlg::IDD, pParent)
	, m_speed_edit(0)
	, m_distance_edit(0)
{

}

CMotionDlg::~CMotionDlg()
{
}

void CMotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AXISSTATE_LIST, m_AxisState_list);
	DDX_Control(pDX, IDC_AXIS_COMBO, m_Axis_combo);
	DDX_Text(pDX, IDC_SPEED_EDIT, m_speed_edit);
	DDX_Text(pDX, IDC_DISTANCE_EDIT, m_distance_edit);
}


BEGIN_MESSAGE_MAP(CMotionDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AXISENABLE_BUTTON, &CMotionDlg::OnBnClickedAxisenableButton)
	ON_BN_CLICKED(IDC_RST_BUTTON, &CMotionDlg::OnBnClickedRstButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CMotionDlg::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_POSTOZERO_BUTTON, &CMotionDlg::OnBnClickedPostozeroButton)
	ON_BN_CLICKED(IDC_POSRUN_BUTTON, &CMotionDlg::OnBnClickedPosrunButton)
	ON_BN_CLICKED(IDC_NEGRUN_BUTTON, &CMotionDlg::OnBnClickedNegrunButton)
	ON_BN_CLICKED(IDC_MOVETOPOS_BUTTON, &CMotionDlg::OnBnClickedMovetoposButton)
	ON_BN_CLICKED(IDC_SAVEPOS_BUTTON, &CMotionDlg::OnBnClickedSaveposButton)
END_MESSAGE_MAP()


// CMotionDlg 消息处理程序


BOOL CMotionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	RSTing=0;                           //复位中

	CRect   rect;
	m_AxisState_list.GetClientRect(&rect);
	m_AxisState_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES);
	m_AxisState_list.InsertColumn(0, L"轴号", LVCFMT_CENTER, rect.Width() / 13+0, -1);
	m_AxisState_list.InsertColumn(1, L"轴名称", LVCFMT_CENTER, rect.Width() * 2 / 13 + 0, -1);
	m_AxisState_list.InsertColumn(2, L"轴报警", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(3, L"轴使能", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(4, L"正限位", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(5, L"原点", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(6, L"负限位", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(7, L"轴运动", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(8, L"当前位置", LVCFMT_CENTER, rect.Width() * 2 / 13+0, -1);
	m_AxisState_list.InsertColumn(9, L"反馈位置", LVCFMT_CENTER, rect.Width() * 2 / 13+0, -1);

	m_AxisState_list.InsertItem(0, L"0",-1);
	m_AxisState_list.InsertItem(1, L"1", -1);

	//插入图标资源
	if (NULL == m_imagelist.GetSafeHandle())
	{
		m_imagelist.Create(32, 32, ILC_MASK | ILC_COLOR32, 1, 1);        //2.0
		m_imagelist.Add(AfxGetApp()->LoadIconW(IDI_GREEN));        //绿色图标
		m_imagelist.Add(AfxGetApp()->LoadIconW(IDI_RED));            //红色图标
		m_imagelist.Add(AfxGetApp()->LoadIconW(IDI_YELLOW));      //黄色图标

		//设置图像列表与list控件关联
		m_AxisState_list.SetImageList(&m_imagelist, LVSIL_SMALL);
	}
	AxisName[AXIS_LEFT] = L"左端伺服";
	AxisName[AXIS_RIGHT] = L"右端伺服";
	for (int i = 0; i < 2; i++)
	{
		m_Axis_combo.AddString(AxisName[i]);
	}
	m_Axis_combo.SetCurSel(AXIS_LEFT);

	SetDlgItemInt(IDC_SPEED_EDIT, 20000);
	SetDlgItemInt(IDC_DISTANCE_EDIT, 20000);

	RefreshAxisStateList();

	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CMotionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	RefreshAxisStateList();
	if(RSTing==1)
	{
		int   axisno = m_Axis_combo.GetCurSel();
		if(1==G::MCard.A[axisno].AS.bDone)
		{
			G::MCard.SetAxisCurPos(axisno,0);
	        G::MCard.SetAxisEncPos(axisno,0);
	        G::MCard.A[axisno].AS.bHome = 1;
			RSTing=0;
			if (axisno == AXIS_LEFT)
				G::bLeftRstOK = 1;
			if (axisno == AXIS_RIGHT)
				G::bRightRstOK = 1;
			AfxMessageBox(L"复位完成！");
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

//伺服使能
void CMotionDlg::OnBnClickedAxisenableButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"自动运行中，不能执行!	");
		return;
	}
	int  axisno = m_Axis_combo.GetCurSel();
	AxisEnable(axisno);
}

//复位
void CMotionDlg::OnBnClickedRstButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (1 == G::bRunflag)
	{
		AfxMessageBox(L"自动运行中，不能复位!	");
		return;
	}
	int  axisno = m_Axis_combo.GetCurSel();
	if (0 == dmc_check_done(0, axisno))
	{
		AfxMessageBox(L"轴运动中，请先停止轴运动！");
		return;
	}
	RSTing = 1;
	G::MCard.HomeAxis2(axisno, 0, 1, 2);

}


// 轴使能
BOOL CMotionDlg::AxisEnable(UINT  AxisNo)
{
	//轴使能
	if (G::MCard.A[AxisNo].AP.bAxsSvnEnable)
	{
		if (IDYES == MessageBox(L"确定要关闭" + AxisName[AxisNo] + L"使能吗？", L"轴使能", MB_YESNO | MB_ICONWARNING))
		{
			if (MCARD_NO_ERROR != G::MCard.EnableAxis(AxisNo, 0))
			{
				MessageBox(L"轴使能关闭失败！", L"提示", MB_ICONERROR);
			}
			G::MCard.A[AxisNo].AS.bHome = 0;
		}
	}
	else
	{
		if (IDYES == MessageBox(L"确定要打开" + AxisName[AxisNo] + L"使能吗？", L"轴使能", MB_YESNO | MB_ICONWARNING))
		{
			if (MCARD_NO_ERROR != G::MCard.EnableAxis(AxisNo, 1))
			{
				MessageBox(L"轴使能打开失败！", L"提示", MB_ICONERROR);
			}
			G::MCard.SetAxisCurPos(AxisNo, 0);
			G::MCard.SetAxisEncPos(AxisNo, 0);

		}
	}
	return 0;
}


void CMotionDlg::OnBnClickedStopButton()
{
	// TODO:  在此添加控件通知处理程序代码
	//1立即停止
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"自动运行中，不能单轴停止!	");
		return;
	}
	int axisno = m_Axis_combo.GetCurSel();
	G::MCard.StopAxis(axisno, 1);                 //立即停止
}

//位置清零
void CMotionDlg::OnBnClickedPostozeroButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (1 == G::bRunflag)
	{
		AfxMessageBox(L"程序自动运行中，不能清零!	");
		return;
	}
	int   axisno = m_Axis_combo.GetCurSel();
	G::MCard.SetAxisCurPos(axisno, 0);
	G::MCard.SetAxisEncPos(axisno, 0);
	G::MCard.A[axisno].AS.bHome = 0;
}

//正向运动
void CMotionDlg::OnBnClickedPosrunButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"自动运行中，不能单轴手动!	");
		return;
	}
	int  axisno = m_Axis_combo.GetCurSel();
	if (0 == dmc_check_done(0, axisno))
	{
		AfxMessageBox(L"轴运动中！");
		return;
	}
	UpdateData(TRUE);
	G::MCard.RMove2(axisno, float(m_speed_edit/**G::MCard.A[axisno].AP.dPPU*/), long(m_distance_edit/**G::MCard.A[axisno].AP.dPPU*/));

}

//反向运动
void CMotionDlg::OnBnClickedNegrunButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"自动运行中，不能单轴手动!	");
		return;
	}
	int  axisno = m_Axis_combo.GetCurSel();
	if (0 == dmc_check_done(0, axisno))
	{
		AfxMessageBox(L"轴运动中！");
		return;
	}
	UpdateData(TRUE);
	G::MCard.RMove2(axisno, float(m_speed_edit/**G::MCard.A[axisno].AP.dPPU*/), -long(m_distance_edit/**G::MCard.A[axisno].AP.dPPU*/));
}


// 刷新轴状态列表
void CMotionDlg::RefreshAxisStateList()
{
	for (int item = 0; item < 2; item++)
	{
		LV_ITEM   lvitem;
		memset((char *)&lvitem, '\0', sizeof(lvitem));
		lvitem.mask = LVIF_TEXT | LVIF_STATE | LVIF_IMAGE;
		lvitem.iItem = item;
		lvitem.stateMask = 0;

		//轴名称
		lvitem.iSubItem = 1;
		m_AxisState_list.SetItemText(item, lvitem.iSubItem, AxisName[item]);

		//轴报警
		lvitem.iSubItem++;
		if (0 == G::MCard.A[item].AS.bDriverAlarm)	
		{
			lvitem.iImage = 0;
		}
		else
		{
			lvitem.iImage = 1;
		}
		m_AxisState_list.SetItem(&lvitem);

		//轴使能
		lvitem.iSubItem++;
		if (0 == G::MCard.A[item].AP.bAxsSvnEnable)
		{
			lvitem.iImage = 0;
		}
		else
		{
			lvitem.iImage = 1;
		}
		m_AxisState_list.SetItem(&lvitem);

		//正限位
		lvitem.iSubItem++;
		if (0 == G::MCard.A[item].AS.bPLimtSnr)
		{
			lvitem.iImage = 0;
		}
		else
		{
			lvitem.iImage = 1;
		}
		m_AxisState_list.SetItem(&lvitem);

		//原点
		lvitem.iSubItem++;
		if (0 == G::MCard.A[item].AS.bORGSnr)
		{
			lvitem.iImage = 0;
		}
		else
		{
			lvitem.iImage = 1;
		}
		m_AxisState_list.SetItem(&lvitem);

		//负限位
		lvitem.iSubItem++;
		if (0 == G::MCard.A[item].AS.bNLimtSnr)
		{
			lvitem.iImage = 0;
		}
		else
		{
			lvitem.iImage = 1;
		}
		m_AxisState_list.SetItem(&lvitem);

		//轴运动
		lvitem.iSubItem++;
		if (0 == G::MCard.A[item].AS.bDone)     //运动中 
		{
			lvitem.iImage = 1;                        //红色图标
		}
		else
		{
			lvitem.iImage = 0;
		}
		m_AxisState_list.SetItem(&lvitem);

		//当前位置
		lvitem.iSubItem++;
		CString  str;
		long  pos = dmc_get_position(0, item);
		str.Format(L"%ld", pos);
		m_AxisState_list.SetItemText(item, lvitem.iSubItem, str);

		//反馈位置
		lvitem.iSubItem++;
		long  enpos = dmc_get_encoder(0, item);
		str.Format(L"%ld", enpos);
		m_AxisState_list.SetItemText(item, lvitem.iSubItem, str);
	}

}

//移动到位置点按钮
void CMotionDlg::OnBnClickedMovetoposButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (1 == G::bRunflag)
	{
		MessageBox(L"自动运行流程中 ，不能手动移动轴！", L"警告", MB_ICONWARNING);
		return;
	}
	int  id = GetCheckedRadioButton(IDC_SCRABFIRST1_RADIO, IDC_SCRABNEXT2_RADIO);
	long  pos, curpos;
	int  axisno;                //对应运动轴号
	int  speed = GetDlgItemInt(IDC_SPEED_EDIT);
	int  j;             ///余数，
	switch (id)
	{
	case IDC_SCRABFIRST1_RADIO:
		//左边初始拍照位置
		//int  i = curpos / (long)G::PR[3].paravalue;           //倍数
		axisno = AXIS_LEFT;
		curpos = dmc_get_position(0, axisno);
		//计算余数
		j = (long)(curpos - G::PR[MP_LeftCamFirstPos].paravalue) % (long)G::PR[MP_LeftCamIntervalDis].paravalue;
		//余数大于0，走剩余距离
		if (j >= 0)
			pos = G::PR[MP_LeftCamIntervalDis].paravalue - j;
		else
			pos = abs(j);
		//j = curpos % (long)G::PR[3].paravalue;         //余数
		//pos = (long)G::PR[2].paravalue + (long)G::PR[3].paravalue - j;
		break;
		//当前位置加拍照间隔距离
	case IDC_SCRABNEXT1_RADIO:
		axisno = AXIS_LEFT;
		curpos = dmc_get_position(0, axisno);
		//计算余数
		j = (long)(curpos - G::PR[MP_LeftCamFirstPos].paravalue) % (long)G::PR[MP_LeftCamIntervalDis].paravalue;
		//余数大于0，走剩余距离
		if (j >= 0)
			pos = G::PR[MP_LeftCamIntervalDis].paravalue - j;
		else
			pos = abs(j);
		break;
	case IDC_SCRABFIRST2_RADIO:                //右端首位
		axisno = AXIS_RIGHT;
		curpos = dmc_get_position(0, axisno);
		//计算余数
		j = (long)(curpos - G::PR[MP_RightCamFirstPos].paravalue) % (long)G::PR[MP_RightCamIntervalDis].paravalue;
		//余数大于0，走剩余距离
		if (j >= 0)
			pos = G::PR[MP_RightCamIntervalDis].paravalue - j;
		else
			pos = abs(j);
		break;
	case IDC_SCRABNEXT2_RADIO:                    //右端下一位
		axisno = AXIS_RIGHT;
		curpos = dmc_get_position(0, axisno);
		//计算余数
		j = (long)(curpos - G::PR[MP_RightCamFirstPos].paravalue) % (long)G::PR[MP_RightCamIntervalDis].paravalue;
		//余数大于0，走剩余距离
		if (j >= 0)
			pos = G::PR[MP_RightCamIntervalDis].paravalue - j;
		else
			pos = abs(j);
		break;
	default:
		AfxMessageBox(L"请先指定目标位置后，再进行移动");
		return;
		break;
	}
	if (dmc_check_done(0, axisno) != 1)
	{
		AfxMessageBox(L"轴运动中，请先停止轴运动!");
		return;
	}
	if ((axisno == AXIS_LEFT) && (0 == G::bLeftRstOK))
	{
		AfxMessageBox(L"请先复位左端伺服！");
		return;
	}
	if ((axisno == AXIS_RIGHT) && (0 == G::bRightRstOK))
	{
		AfxMessageBox(L"请先复位右端伺服！");
		return;
	}
	UpdateData(TRUE);
	//相对运动
	G::MCard.RMove2(axisno, speed, pos);
	////设置速度
	//dmc_set_profile(0, 0, 500, speed, 0.1, 0.1, 500);
	////相对位置
	//dmc_pmove(0, 0, pos, 0);                     //0相对坐标模式
	////G::MCard.AMove2(0, speed, pos);
}

//保存位置点按钮
void CMotionDlg::OnBnClickedSaveposButton()
{
	// TODO:  在此添加控件通知处理程序代码
	int  id = GetCheckedRadioButton(IDC_SCRABFIRST1_RADIO, IDC_SCRABNEXT2_RADIO);
	int no;                        //PR数组中下标
	int  axisno;                //对应运动轴号
	int pos;
	switch (id)
	{
	case IDC_SCRABFIRST1_RADIO:               //左首位
		axisno = AXIS_LEFT;
		no = MP_LeftCamFirstPos;
		break;
	case IDC_SCRABNEXT1_RADIO:               //左下一个
		axisno = AXIS_LEFT;
		no = MP_LeftCamIntervalDis;
		break;
	case IDC_SCRABFIRST2_RADIO:              //右首位
		axisno = AXIS_RIGHT;
		no = MP_RightCamFirstPos;
		break;
	case IDC_SCRABNEXT2_RADIO:
		axisno = AXIS_RIGHT;
		no = MP_RightCamIntervalDis;
		break;
	default:
		AfxMessageBox(L"请选择保存位置点再按保存按钮！");
		return;
		break;
	}
	if ((axisno == AXIS_LEFT) && (0 == G::bLeftRstOK))
	{
		AfxMessageBox(L"请先复位左端伺服！");
		return;
	}
	if ((axisno == AXIS_RIGHT) && (0 == G::bRightRstOK))
	{
		AfxMessageBox(L"请先复位右端伺服！");
		return;
	}
	if ((id == IDC_SCRABNEXT1_RADIO) || (id == IDC_SCRABNEXT2_RADIO))
	{
		AfxMessageBox(L"不能更改，请重新选择!");
		return;
	}
	if (IDNO == MessageBox(L"确定保存当前位置点？", L"提示", MB_YESNO | MB_ICONQUESTION))
		return;
	//计算位置
	if (axisno==AXIS_LEFT)
		pos = (long)(dmc_get_position(0, axisno) + G::PR[MP_LeftCamIntervalDis].paravalue) % (long)G::PR[MP_LeftCamIntervalDis].paravalue;
	else if (axisno==AXIS_RIGHT)
		pos = (long)(dmc_get_position(0, axisno) + G::PR[MP_RightCamIntervalDis].paravalue) % (long)G::PR[MP_RightCamIntervalDis].paravalue;
	G::PR[no].paravalue = pos;
	if (0 == G::WriteCommonPara())
	AfxMessageBox(L"参数保存成功！");
	else
	AfxMessageBox(L"参数保存失败！");
	/*if (i % 2 == 0)
		G::PR[i].paravalue = G::MCard.GetAxisCurPos(0);
	else
		G::PR[i].paravalue = G::MCard.GetAxisCurPos(0) - G::PR[i - 1].paravalue;*/
	/*if (0 == G::WriteCommonPara())
		AfxMessageBox(L"参数保存成功！");
	else
		AfxMessageBox(L"参数保存失败！");*/
}
