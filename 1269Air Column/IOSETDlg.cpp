// IOSETDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "IOSETDlg.h"
#include "afxdialogex.h"


// CIOSETDlg �Ի���

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


// CIOSETDlg ��Ϣ�������


HBRUSH CIOSETDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
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


	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CIOSETDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SkinH_DetachEx(this->m_hWnd);

	//��������ָ����ɫ�Ļ�ˢ
	m_brush_green = CreateSolidBrush(RGB(100, 200, 100));
	m_brush_red = CreateSolidBrush(RGB(250, 100, 100));
	m_brush_yellow = CreateSolidBrush(RGB(250, 250, 100));

	LoadPortName();

	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CIOSETDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//ʹ�������ڿͻ�����Ч, �����и�����ʾ�ĺ���,,,,,���ڵĿͻ�����Ч��ζ����Ҫ�ػ�
	this->Invalidate(0);    

	//RefreshColor();
	CDialogEx::OnTimer(nIDEvent);
}


// ��IO�˿�������ʾ��EDIT�ؼ���
int CIOSETDlg::LoadPortName()
{
	/////////////////////////////////      ����˿�����        ///////////////////////////////////////
	for (int i = 0; i < 16; i++)
	{
		this->SetDlgItemTextW(IDC_EDIT_I1 + i, G::MCard.I[i].lpwstrPortName);
	}

	//////////////////////////////         ����˿�����        /////////////////////////////////////////
	for (int i = 0; i < 16; i++)
	{
		int j = i * 2;
		this->SetDlgItemTextW(IDC_EDIT_O1+j, G::MCard.O[i].lpwstrPortName);
	}
	return 0;
}


BOOL CIOSETDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO:  �ڴ����ר�ô����/����û���

	int  id = LOWORD(wParam);                        //ȡ�������ֽ�
	int  msg = HIWORD(wParam);                     //ȡ�������ֽ�

	if ((id >= IDC_STATIC_O0 && id <= IDC_STATIC_O15) && (msg == STN_CLICKED || msg == STN_DBLCLK))
	{
		int  i = id - IDC_STATIC_O0;
		BYTE    BV = 0;

		//AfxMessageBox(L"ȷ�ϵ��");
		//������ź�ȡ��
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

//��������
void CIOSETDlg::OnBnClickedSaveioButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString	  str;                             //�ؼ����ַ�����
	CString   strno;                         //�˿ں�����
	try
	{
		/////////////////////////////���˿����Ʒ���MCARD�У��˳�ʱ�����ݴ˽��б���
		//����˿�
		for (int i = 0; i < 16; i++)
		{
			GetDlgItemText(IDC_EDIT_I1 + i, str);
			G::MCard.I[i].lpwstrPortName = str;
		}

		//����˿�     
		//����˿ڱ༭�ؼ�ID���2
		for (int i = 0; i < 32; i = i + 2)
		{
			int j = i / 2;
			GetDlgItemText(IDC_EDIT_O1 + i, str);
			G::MCard.O[j].lpwstrPortName = str;
		}
		int i = G::WriteIOPara();
		if (0==i)
			AfxMessageBox(L"����ɹ�!");
		else
			AfxMessageBox(L"����ʧ��!");
	}
	catch (...)
	{
		AfxMessageBox(L"����ʧ��!");
	}

}


// ʹ��Ƥ��ʱ���¿ؼ���ɫ
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
