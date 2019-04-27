// IOSETDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "IOSETDlg.h"
#include "afxdialogex.h"


// CIOSETDlg 对话框

IMPLEMENT_DYNAMIC(CIOSETDlg, CDialogEx)

CIOSETDlg::CIOSETDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIOSETDlg::IDD, pParent)
{

}

CIOSETDlg::~CIOSETDlg()
{
}

void CIOSETDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIOSETDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAVEIO_BUTTON, &CIOSETDlg::OnBnClickedSaveioButton)
END_MESSAGE_MAP()


// CIOSETDlg 消息处理程序


HBRUSH CIOSETDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	int  nID = pWnd->GetDlgCtrlID();
	if (nID >= IDC_STATIC_I1 && nID <= IDC_STATIC_I16)
	{
		if (G::MCard.I[nID - IDC_STATIC_I1].sValue == 0)
		{
			pDC->SetBkColor(RGB(100, 200, 100));
			return this->m_brush_green;
		}
		else if (1 == G::MCard.I[nID - IDC_STATIC_I1].sValue)
		{
			pDC->SetBkColor(RGB(250, 100, 100));
			return  this->m_brush_red;
		}
		else
		{
			pDC->SetBkColor(RGB(250, 250, 100));
			return  this->m_brush_yellow;
		}
	}

	if (nID >= IDC_STATIC_O0 && nID <= IDC_STATIC_O15)
	{
		if (G::MCard.O[nID - IDC_STATIC_O0].sValue == 0)
		{
			pDC->SetBkColor(RGB(100, 200, 100));
			return this->m_brush_green;
		}
		else if (1 == G::MCard.O[nID - IDC_STATIC_O0].sValue)
		{
			pDC->SetBkColor(RGB(250, 100, 100));
			return  this->m_brush_red;
		}
		else
		{
			pDC->SetBkColor(RGB(250, 250, 100));
			return  this->m_brush_yellow;
		}
	}


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CIOSETDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SkinH_DetachEx(this->m_hWnd);

	//创建带有指定颜色的画刷
	m_brush_green = CreateSolidBrush(RGB(100, 200, 100));
	m_brush_red = CreateSolidBrush(RGB(250, 100, 100));
	m_brush_yellow = CreateSolidBrush(RGB(250, 250, 100));

	LoadPortName();

	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CIOSETDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	//使整个窗口客户区无效, 并进行更新显示的函数,,,,,窗口的客户区无效意味着需要重绘
	this->Invalidate(0);    

	//RefreshColor();
	CDialogEx::OnTimer(nIDEvent);
}


// 将IO端口名称显示到EDIT控件中
int CIOSETDlg::LoadPortName()
{
	/////////////////////////////////      输入端口名称        ///////////////////////////////////////
	for (int i = 0; i < 16; i++)
	{
		this->SetDlgItemTextW(IDC_EDIT_I1 + i, G::MCard.I[i].lpwstrPortName);
	}

	//////////////////////////////         输出端口名称        /////////////////////////////////////////
	for (int i = 0; i < 16; i++)
	{
		int j = i * 2;
		this->SetDlgItemTextW(IDC_EDIT_O1+j, G::MCard.O[i].lpwstrPortName);
	}
	return 0;
}


BOOL CIOSETDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO:  在此添加专用代码和/或调用基类

	int  id = LOWORD(wParam);                        //取低两个字节
	int  msg = HIWORD(wParam);                     //取高两个字节

	if ((id >= IDC_STATIC_O0 && id <= IDC_STATIC_O15) && (msg == STN_CLICKED || msg == STN_DBLCLK))
	{
		int  i = id - IDC_STATIC_O0;
		BYTE    BV = 0;

		//AfxMessageBox(L"确认点击");
		//点击后信号取反
		if (1 == G::MCard.O[i].sValue)
		{
			BV = 0;
		}
		else
		{
			BV = 1;
		}
		G::MCard.SetDoBit(i, BV);
		Sleep(30);
		this->GetDlgItem(id)->RedrawWindow();
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

//保存设置
void CIOSETDlg::OnBnClickedSaveioButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString	  str;                             //控件内字符内容
	CString   strno;                         //端口号名称
	try
	{
		/////////////////////////////将端口名称放入MCARD中，退出时会依据此进行保存
		//输入端口
		for (int i = 0; i < 16; i++)
		{
			GetDlgItemText(IDC_EDIT_I1 + i, str);
			G::MCard.I[i].lpwstrPortName = str;
		}

		//输出端口     
		//输出端口编辑控件ID相隔2
		for (int i = 0; i < 32; i = i + 2)
		{
			int j = i / 2;
			GetDlgItemText(IDC_EDIT_O1 + i, str);
			G::MCard.O[j].lpwstrPortName = str;
		}
		int i = G::WriteIOPara();
		if (0==i)
			AfxMessageBox(L"保存成功!");
		else
			AfxMessageBox(L"保存失败!");
	}
	catch (...)
	{
		AfxMessageBox(L"保存失败!");
	}

}


// 使用皮肤时更新控件颜色
void CIOSETDlg::RefreshColor()
{
	int  nID = GetDlgCtrlID();
	if (nID >= IDC_STATIC_I1 && nID <= IDC_STATIC_I16)
	{
		if (G::MCard.I[nID - IDC_STATIC_I1].sValue == 0)
		{
			SkinH_SetBackColor(GetDlgItem(nID)->m_hWnd, 100, 200, 100);
			//pDC->SetBkColor(RGB(100, 200, 100));
			//return; /*this->m_brush_green;*/
		}
		else if (1 == G::MCard.I[nID - IDC_STATIC_I1].sValue)
		{
			SkinH_SetBackColor(GetDlgItem(nID)->m_hWnd, 250, 100, 100);
			//pDC->SetBkColor(RGB(250, 100, 100));
			//return;  /*this->m_brush_red;*/
		}
		else
		{
			SkinH_SetBackColor(GetDlgItem(nID)->m_hWnd, 250, 100, 100);
			//pDC->SetBkColor(RGB(250, 250, 100));
			//return;  /*this->m_brush_yellow;*/
		}
	}

	if (nID >= IDC_STATIC_O0 && nID <= IDC_STATIC_O15)
	{
		if (G::MCard.O[nID - IDC_STATIC_O0].sValue == 0)
		{
			SkinH_SetBackColor(GetDlgItem(nID)->m_hWnd, 100, 200, 100);
			//pDC->SetBkColor(RGB(100, 200, 100));
			/*this->m_brush_green;*/
		}
		else if (1 == G::MCard.O[nID - IDC_STATIC_O0].sValue)
		{
			SkinH_SetBackColor(GetDlgItem(nID)->m_hWnd, 250, 100, 100);
			//pDC->SetBkColor(RGB(250, 100, 100));
			//return;  /*this->m_brush_red;*/
		}
		else
		{
			SkinH_SetBackColor(GetDlgItem(nID)->m_hWnd, 250, 250, 100);
			//pDC->SetBkColor(RGB(250, 250, 100));
			//return;  /*this->m_brush_yellow;*/
		}
	}
	return;
}
