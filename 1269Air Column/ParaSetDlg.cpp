// ParaSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "ParaSetDlg.h"
#include "afxdialogex.h"


// CParaSetDlg 对话框

IMPLEMENT_DYNAMIC(CParaSetDlg, CDialogEx)

CParaSetDlg::CParaSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParaSetDlg::IDD, pParent)
{

}

CParaSetDlg::~CParaSetDlg()
{
}

void CParaSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Para_TAB, m_paratab);
	DDX_Control(pDX, IDC_PARA_LIST, m_paralist);
	DDX_Control(pDX, IDC_PARACHANGE_EDIT, m_parachangeedit);
}


BEGIN_MESSAGE_MAP(CParaSetDlg, CDialogEx)
	ON_NOTIFY(NM_HOVER, IDC_PARA_LIST, &CParaSetDlg::OnHoverParaList)
	ON_NOTIFY(NM_DBLCLK, IDC_PARA_LIST, &CParaSetDlg::OnDblclkParaList)
	ON_NOTIFY(TCN_SELCHANGE, IDC_Para_TAB, &CParaSetDlg::OnSelchangeParaTab)
	ON_EN_KILLFOCUS(IDC_PARACHANGE_EDIT, &CParaSetDlg::OnKillfocusParachangeEdit)
	ON_EN_SETFOCUS(IDC_PARACHANGE_EDIT, &CParaSetDlg::OnSetfocusParachangeEdit)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SAVEPARA_BUTTON, &CParaSetDlg::OnBnClickedSaveparaButton)
END_MESSAGE_MAP()


// CParaSetDlg 消息处理程序


BOOL CParaSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect    rect;
	GetDlgItem(IDC_PARA_LIST)->GetClientRect(&rect);

	m_paralist.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_FULLROWSELECT);
	m_paralist.InsertColumn(0, L"", LVCFMT_CENTER, rect.Width() / 7-60, 0);
	m_paralist.InsertColumn(1, L"参数类型", LVCFMT_CENTER, rect.Width() / 7+10, 1);
	m_paralist.InsertColumn(2, L"参数名", LVCFMT_CENTER, rect.Width() / 7+60, 2);
	m_paralist.InsertColumn(3, L"参数值", LVCFMT_CENTER, rect.Width() / 7+10, 3);
	m_paralist.InsertColumn(4, L"最大值", LVCFMT_CENTER, rect.Width() / 7, 4);
	m_paralist.InsertColumn(5, L"最小值", LVCFMT_CENTER, rect.Width() / 7, 5);
	m_paralist.InsertColumn(6, L"备注", LVCFMT_CENTER, rect.Width() / 7 - 20, 6);

	m_paratab.InsertItem(0, L"运动参数");
	m_paratab.InsertItem(1, L"相机参数");
	m_paratab.InsertItem(2, L"视觉参数");

	m_paralist.ShowWindow(SW_SHOW);

	m_paratab.SetCurSel(0);
	ListLoadItem(&m_paralist,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


//鼠标在list控件悬停
void CParaSetDlg::OnHoverParaList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码

	//低位字指定起始位置，高位字为结束位置。如果低位为0，高位为 - 1，则编辑控件中的全部文本被选中；如
	//果低位字为 - 1，则任何当前选定内容被去掉选定状态。*/
	
	//选中编辑框全部内容
	m_parachangeedit.SetSel(0, -1);
	*pResult = 0;
}

//鼠标左键点击list控件
void CParaSetDlg::OnDblclkParaList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	CRect rc1, rc2, rect;
	this->GetClientRect(rc1);
	this->ClientToScreen(rc1);            //当前对话框相对屏幕坐标

	m_paralist.GetClientRect(rc2);
	m_paralist.ClientToScreen(rc2);     //列表相对屏幕坐标

	if (pNMItemActivate->iSubItem != -1)
	{
		row = pNMItemActivate->iItem;        //当前被选中的行序号
		col = pNMItemActivate->iSubItem;      //当前被选中的列序号

		m_paralist.GetSubItemRect(row, col, LVIR_LABEL, rect);             //获得子项rect
		int w = rect.Width();
		int h = rect.Height();
		rect.left = rect.left + (rc2.left - rc1.left);
		rect.top = rect.top + (rc2.top - rc1.top);
		rect.right = rect.left + w;
		rect.bottom = rect.top + h;

		//m_Cedit.SetParent(&m_Clist);    //转换坐标为列表框的坐标
		m_parachangeedit.MoveWindow(rect);         //移动Edit到rect所在位置
		m_parachangeedit.SetWindowTextW(m_paralist.GetItemText(row, col));   //将该子项值放入edit控件中
		m_parachangeedit.ShowWindow(SW_SHOW);     //显示edit控件
		m_parachangeedit.SetFocus();            //设置edit焦点
		m_parachangeedit.ShowCaret();       //显示光标
		m_parachangeedit.SetSel(-1);          //将光标移动到最后
	}


	*pResult = 0;
}

//tab控件项目改变
void CParaSetDlg::OnSelchangeParaTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	m_parachangeedit.ShowWindow(SW_HIDE);

	int    nTabItem = m_paratab.GetCurSel();
	switch (nTabItem)
	{
	case 0:
		ListLoadItem(&m_paralist, 0);
		break;
	case 1:
		ListLoadItem(&m_paralist, 1);
		break;
	case 2:
		ListLoadItem(&m_paralist, 2);
		break;
	default:
		break;
	}

	*pResult = 0;
}

//鼠标从编辑控件离开(编辑控件失去焦点)
void CParaSetDlg::OnKillfocusParachangeEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	CString	  strminvalue, strmaxvalue, strvalue;
	m_parachangeedit.GetWindowTextW(str);       //取得编辑框内容
	
	//1参数类型，2参数名，3参数值，4最大值，5最小值，6备注
	//如果修改的是参数类型名
	if (1 == col)                   
	{
		AfxMessageBox(L"不允许修改参数类型名！");
		return;
	}
	//如果权限不够，不得修改最大值和最小值
	if ((4 == col) || (5 == col) && (G::UserAuthority < 8))
	{
		AfxMessageBox(L"最大值最小值不允许修改！");
		return;
	}
	//修改的值大于最大值或小于最小值
	if (3 == col)
	{
		int    value, maxvalue, minvalue;
		strminvalue = m_paralist.GetItemText(row, 5);
		strmaxvalue = m_paralist.GetItemText(row, 4);
		value = _wtoi(str.GetBuffer());
		maxvalue = _wtoi(strmaxvalue.GetBuffer());
		minvalue = _wtoi(strminvalue.GetBuffer());
		if (value > maxvalue || value < minvalue)
		{
			AfxMessageBox(L"修改值超过允许范围!");
			return;
		}
	}

	m_paralist.SetItemText(row, col, str);       //将内容更新到列表控件中
	//ChangedToPara();
	GetChangeNo();
	G::PR[No].paraname = m_paralist.GetItemText(row, 2);
	str = m_paralist.GetItemText(row, 3);
	G::PR[No].paravalue = G::string_double(str);
	str = m_paralist.GetItemText(row, 4);
	G::PR[No].paraMaxValue = G::string_double(str);
	str = m_paralist.GetItemText(row, 5);
	G::PR[No].paraMinValue = G::string_double(str);
	G::PR[No].remark = m_paralist.GetItemText(row, 6);

	m_parachangeedit.ShowWindow(SW_HIDE);        //隐藏编辑框
}

//可有可无
//鼠标焦点在edit控件(编辑控件获得焦点)
void CParaSetDlg::OnSetfocusParachangeEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	m_parachangeedit.SetSel(-1);
}




HBRUSH CParaSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


// 向列表控件中插入内容
//类型0,1,2分别表示运动参数，相机参数，视觉参数
int CParaSetDlg::ListLoadItem(CListCtrl* pListCtrl,int ntype)
{
	m_parachangeedit.SetOwner(FromHandle(pListCtrl->m_hWnd));
	pListCtrl->DeleteAllItems();
	CString  str;
	int j;
	int x = 0;             //相机参数插行时用
	int y = 0;             //视觉参数插行时用
	//参数类型，参数名，参数值，最大值，最小值，备注
	switch (ntype)
	{
		//运动参数
	case 0:
		for (int i = 0; i < 30; i++)
		{
			if (L"备用" != G::PR[i].paraname)
			{
				str.Format(L"%d", i + 1);
				m_paralist.InsertItem(i, str);

				m_paralist.SetItemText(i, 1, G::PR[i].type);
				m_paralist.SetItemText(i, 2, G::PR[i].paraname);
				str.Format(L"%.2lf", G::PR[i].paravalue);
				m_paralist.SetItemText(i, 3, str);
				str.Format(L"%.2lf", G::PR[i].paraMaxValue);
				m_paralist.SetItemText(i, 4, str);
				str.Format(L"%.2lf", G::PR[i].paraMinValue);
				m_paralist.SetItemText(i, 5, str);
				m_paralist.SetItemText(i, 6, G::PR[i].remark);
			}
		}
		break;
		//相机参数
	case 1:
		for (int i = 0; i < 60; i++)
		{
			j = i + 30;
			if (L"备用" != G::PR[j].paraname)
			{
				str.Format(L"%d", x +1);
				m_paralist.InsertItem(x, str);

				m_paralist.SetItemText(x, 1, G::PR[j].type);
				m_paralist.SetItemText(x, 2, G::PR[j].paraname);
				str.Format(L"%.2lf", G::PR[j].paravalue);
				m_paralist.SetItemText(x, 3, str);
				str.Format(L"%.2lf", G::PR[j].paraMaxValue);
				m_paralist.SetItemText(x, 4, str);
				str.Format(L"%.2lf", G::PR[j].paraMinValue);
				m_paralist.SetItemText(x, 5, str);
				m_paralist.SetItemText(x, 6, G::PR[j].remark);
				x++;
			}
		}
		break;
		//视觉参数
	case 2:
		for (int i = 0; i < 60; i++)
		{
			j = i + 90;
			if (L"备用" != G::PR[j].paraname)
			{
				str.Format(L"%d", y + 1);
				pListCtrl->InsertItem(y, str);

				pListCtrl->SetItemText(y, 1, G::PR[j].type);
				pListCtrl->SetItemText(y, 2, G::PR[j].paraname);
				str.Format(L"%.2lf", G::PR[j].paravalue);
				pListCtrl->SetItemText(y, 3, str);
				str.Format(L"%.2lf", G::PR[j].paraMaxValue);
				pListCtrl->SetItemText(y, 4, str);
				str.Format(L"%.2lf", G::PR[j].paraMinValue);
				pListCtrl->SetItemText(y, 5, str);
				pListCtrl->SetItemText(y, 6, G::PR[j].remark);
				y++;
			}
		}
		break;
	/*default:
		break;*/
	}

	return 0;
}


// 将变更的参数写入全局参数数组(未用)
int CParaSetDlg::ChangedToPara()
{
	CString    paratype;                                          //记录更改子项的参数类型
	CString    str;
	//将更改的参数写入到全局参数数组PR中
	paratype = m_paralist.GetItemText(row, 1);

	//运动参数
	if (L"运动参数" == paratype)
	{
		G::PR[row].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[row].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[row].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[row].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[row].remark = m_paralist.GetItemText(row, 6);
	}

	//相机1参数
	if (L"相机1参数" == paratype)
	{
		int  i = row + 30;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//相机2参数
	if (L"相机2参数" == paratype)
	{
		int  i = row + 40-G::Camera1Used;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//相机3参数
	if (L"相机3参数" == paratype)
	{
		int  i = row + 50-G::Camera1Used-G::Camera2Used;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//相机4参数
	if (L"相机4参数" == paratype)
	{
		int  i = row + 60-G::Camera1Used-G::Camera2Used-G::Camera3Used;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//相机5参数
	if (L"相机5参数" == paratype)
	{
		int  i = row + 70 - G::Camera1Used - G::Camera2Used - G::Camera3Used-G::Camera4Used;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//相机6参数
	if (L"相机6参数" == paratype)
	{
		int  i = row + 80 - G::Camera1Used - G::Camera2Used - G::Camera3Used-G::Camera4Used-G::Camera5Used;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//视觉1参数
	if (L"视觉1参数" == paratype)
	{
		int  i = row + 90;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//视觉2参数
	if (L"视觉2参数" == paratype)
	{
		int  i = row + 100-G::Vision1Used;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//视觉3参数
	if (L"视觉3参数" == paratype)
	{
		int  i = row + 110-G::Vision1Used-G::Vision2Used;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//视觉4参数
	if (L"视觉4参数" == paratype)
	{
		int  i = row + 120-G::Vision1Used-G::Vision2Used-G::Vision3Used;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//视觉5参数
	if (L"视觉5参数" == paratype)
	{
		int  i = row + 130 - G::Vision1Used - G::Vision2Used - G::Vision3Used-G::Vision4Used;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	//视觉5参数
	if (L"视觉6参数" == paratype)
	{
		int  i = row + 140 - G::Vision1Used - G::Vision2Used - G::Vision3Used - G::Vision4Used-G::Vision5Used;
		G::PR[i].paraname = m_paralist.GetItemText(row, 2);
		str = m_paralist.GetItemText(row, 3);
		G::PR[i].paravalue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 4);
		G::PR[i].paraMaxValue = _wtoi(str.GetBuffer());
		str = m_paralist.GetItemText(row, 5);
		G::PR[i].paraMinValue = _wtoi(str.GetBuffer());
		G::PR[i].remark = m_paralist.GetItemText(row, 6);
	}

	return 0;
}

//保存参数
void CParaSetDlg::OnBnClickedSaveparaButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (-1 == G::WriteSerialPara(m_paratab.GetCurSel()))
	{
		AfxMessageBox(L"参数保存失败！");
	}
	else
		AfxMessageBox(L"参数保存成功！");

}


// 获取改变的参数号
int CParaSetDlg::GetChangeNo()
{
	int nTabitem = m_paratab.GetCurSel();
	switch (nTabitem)
	{
	case 0:
		No = row /*- 1*/;
		break;
	case 1:
		No = row + MotionParaCounts /*- 1*/;
		break;
	case 2:
		No = row + MotionParaCounts+CameraParaCounts /*- 1*/;
		break;
	default:
		AfxMessageBox(L"选择出错！");
		return 0;
		break;
	}
	return 0;
}
