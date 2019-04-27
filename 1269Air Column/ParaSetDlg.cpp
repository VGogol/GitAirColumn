// ParaSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "ParaSetDlg.h"
#include "afxdialogex.h"


// CParaSetDlg �Ի���

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


// CParaSetDlg ��Ϣ�������


BOOL CParaSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect    rect;
	GetDlgItem(IDC_PARA_LIST)->GetClientRect(&rect);

	m_paralist.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_FULLROWSELECT);
	m_paralist.InsertColumn(0, L"", LVCFMT_CENTER, rect.Width() / 7-60, 0);
	m_paralist.InsertColumn(1, L"��������", LVCFMT_CENTER, rect.Width() / 7+10, 1);
	m_paralist.InsertColumn(2, L"������", LVCFMT_CENTER, rect.Width() / 7+60, 2);
	m_paralist.InsertColumn(3, L"����ֵ", LVCFMT_CENTER, rect.Width() / 7+10, 3);
	m_paralist.InsertColumn(4, L"���ֵ", LVCFMT_CENTER, rect.Width() / 7, 4);
	m_paralist.InsertColumn(5, L"��Сֵ", LVCFMT_CENTER, rect.Width() / 7, 5);
	m_paralist.InsertColumn(6, L"��ע", LVCFMT_CENTER, rect.Width() / 7 - 20, 6);

	m_paratab.InsertItem(0, L"�˶�����");
	m_paratab.InsertItem(1, L"�������");
	m_paratab.InsertItem(2, L"�Ӿ�����");

	m_paralist.ShowWindow(SW_SHOW);

	m_paratab.SetCurSel(0);
	ListLoadItem(&m_paralist,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


//�����list�ؼ���ͣ
void CParaSetDlg::OnHoverParaList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//��λ��ָ����ʼλ�ã���λ��Ϊ����λ�á������λΪ0����λΪ - 1����༭�ؼ��е�ȫ���ı���ѡ�У���
	//����λ��Ϊ - 1�����κε�ǰѡ�����ݱ�ȥ��ѡ��״̬��*/
	
	//ѡ�б༭��ȫ������
	m_parachangeedit.SetSel(0, -1);
	*pResult = 0;
}

//���������list�ؼ�
void CParaSetDlg::OnDblclkParaList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CRect rc1, rc2, rect;
	this->GetClientRect(rc1);
	this->ClientToScreen(rc1);            //��ǰ�Ի��������Ļ����

	m_paralist.GetClientRect(rc2);
	m_paralist.ClientToScreen(rc2);     //�б������Ļ����

	if (pNMItemActivate->iSubItem != -1)
	{
		row = pNMItemActivate->iItem;        //��ǰ��ѡ�е������
		col = pNMItemActivate->iSubItem;      //��ǰ��ѡ�е������

		m_paralist.GetSubItemRect(row, col, LVIR_LABEL, rect);             //�������rect
		int w = rect.Width();
		int h = rect.Height();
		rect.left = rect.left + (rc2.left - rc1.left);
		rect.top = rect.top + (rc2.top - rc1.top);
		rect.right = rect.left + w;
		rect.bottom = rect.top + h;

		//m_Cedit.SetParent(&m_Clist);    //ת������Ϊ�б�������
		m_parachangeedit.MoveWindow(rect);         //�ƶ�Edit��rect����λ��
		m_parachangeedit.SetWindowTextW(m_paralist.GetItemText(row, col));   //��������ֵ����edit�ؼ���
		m_parachangeedit.ShowWindow(SW_SHOW);     //��ʾedit�ؼ�
		m_parachangeedit.SetFocus();            //����edit����
		m_parachangeedit.ShowCaret();       //��ʾ���
		m_parachangeedit.SetSel(-1);          //������ƶ������
	}


	*pResult = 0;
}

//tab�ؼ���Ŀ�ı�
void CParaSetDlg::OnSelchangeParaTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

//���ӱ༭�ؼ��뿪(�༭�ؼ�ʧȥ����)
void CParaSetDlg::OnKillfocusParachangeEdit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CString	  strminvalue, strmaxvalue, strvalue;
	m_parachangeedit.GetWindowTextW(str);       //ȡ�ñ༭������
	
	//1�������ͣ�2��������3����ֵ��4���ֵ��5��Сֵ��6��ע
	//����޸ĵ��ǲ���������
	if (1 == col)                   
	{
		AfxMessageBox(L"�������޸Ĳ�����������");
		return;
	}
	//���Ȩ�޲����������޸����ֵ����Сֵ
	if ((4 == col) || (5 == col) && (G::UserAuthority < 8))
	{
		AfxMessageBox(L"���ֵ��Сֵ�������޸ģ�");
		return;
	}
	//�޸ĵ�ֵ�������ֵ��С����Сֵ
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
			AfxMessageBox(L"�޸�ֵ��������Χ!");
			return;
		}
	}

	m_paralist.SetItemText(row, col, str);       //�����ݸ��µ��б�ؼ���
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

	m_parachangeedit.ShowWindow(SW_HIDE);        //���ر༭��
}

//���п���
//��꽹����edit�ؼ�(�༭�ؼ���ý���)
void CParaSetDlg::OnSetfocusParachangeEdit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_parachangeedit.SetSel(-1);
}




HBRUSH CParaSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


// ���б�ؼ��в�������
//����0,1,2�ֱ��ʾ�˶�����������������Ӿ�����
int CParaSetDlg::ListLoadItem(CListCtrl* pListCtrl,int ntype)
{
	m_parachangeedit.SetOwner(FromHandle(pListCtrl->m_hWnd));
	pListCtrl->DeleteAllItems();
	CString  str;
	int j;
	int x = 0;             //�����������ʱ��
	int y = 0;             //�Ӿ���������ʱ��
	//�������ͣ�������������ֵ�����ֵ����Сֵ����ע
	switch (ntype)
	{
		//�˶�����
	case 0:
		for (int i = 0; i < 30; i++)
		{
			if (L"����" != G::PR[i].paraname)
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
		//�������
	case 1:
		for (int i = 0; i < 60; i++)
		{
			j = i + 30;
			if (L"����" != G::PR[j].paraname)
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
		//�Ӿ�����
	case 2:
		for (int i = 0; i < 60; i++)
		{
			j = i + 90;
			if (L"����" != G::PR[j].paraname)
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


// ������Ĳ���д��ȫ�ֲ�������(δ��)
int CParaSetDlg::ChangedToPara()
{
	CString    paratype;                                          //��¼��������Ĳ�������
	CString    str;
	//�����ĵĲ���д�뵽ȫ�ֲ�������PR��
	paratype = m_paralist.GetItemText(row, 1);

	//�˶�����
	if (L"�˶�����" == paratype)
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

	//���1����
	if (L"���1����" == paratype)
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

	//���2����
	if (L"���2����" == paratype)
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

	//���3����
	if (L"���3����" == paratype)
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

	//���4����
	if (L"���4����" == paratype)
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

	//���5����
	if (L"���5����" == paratype)
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

	//���6����
	if (L"���6����" == paratype)
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

	//�Ӿ�1����
	if (L"�Ӿ�1����" == paratype)
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

	//�Ӿ�2����
	if (L"�Ӿ�2����" == paratype)
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

	//�Ӿ�3����
	if (L"�Ӿ�3����" == paratype)
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

	//�Ӿ�4����
	if (L"�Ӿ�4����" == paratype)
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

	//�Ӿ�5����
	if (L"�Ӿ�5����" == paratype)
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

	//�Ӿ�5����
	if (L"�Ӿ�6����" == paratype)
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

//�������
void CParaSetDlg::OnBnClickedSaveparaButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (-1 == G::WriteSerialPara(m_paratab.GetCurSel()))
	{
		AfxMessageBox(L"��������ʧ�ܣ�");
	}
	else
		AfxMessageBox(L"��������ɹ���");

}


// ��ȡ�ı�Ĳ�����
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
		AfxMessageBox(L"ѡ�����");
		return 0;
		break;
	}
	return 0;
}
