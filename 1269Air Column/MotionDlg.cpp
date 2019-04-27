// MotionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "1269Air Column.h"
#include "MotionDlg.h"
#include "afxdialogex.h"


// CMotionDlg �Ի���

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


// CMotionDlg ��Ϣ�������


BOOL CMotionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	RSTing=0;                           //��λ��

	CRect   rect;
	m_AxisState_list.GetClientRect(&rect);
	m_AxisState_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES);
	m_AxisState_list.InsertColumn(0, L"���", LVCFMT_CENTER, rect.Width() / 13+0, -1);
	m_AxisState_list.InsertColumn(1, L"������", LVCFMT_CENTER, rect.Width() * 2 / 13 + 0, -1);
	m_AxisState_list.InsertColumn(2, L"�ᱨ��", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(3, L"��ʹ��", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(4, L"����λ", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(5, L"ԭ��", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(6, L"����λ", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(7, L"���˶�", LVCFMT_CENTER, rect.Width() * 1 / 13-0, -1);
	m_AxisState_list.InsertColumn(8, L"��ǰλ��", LVCFMT_CENTER, rect.Width() * 2 / 13+0, -1);
	m_AxisState_list.InsertColumn(9, L"����λ��", LVCFMT_CENTER, rect.Width() * 2 / 13+0, -1);

	m_AxisState_list.InsertItem(0, L"0",-1);
	m_AxisState_list.InsertItem(1, L"1", -1);

	//����ͼ����Դ
	if (NULL == m_imagelist.GetSafeHandle())
	{
		m_imagelist.Create(32, 32, ILC_MASK | ILC_COLOR32, 1, 1);        //2.0
		m_imagelist.Add(AfxGetApp()->LoadIconW(IDI_GREEN));        //��ɫͼ��
		m_imagelist.Add(AfxGetApp()->LoadIconW(IDI_RED));            //��ɫͼ��
		m_imagelist.Add(AfxGetApp()->LoadIconW(IDI_YELLOW));      //��ɫͼ��

		//����ͼ���б���list�ؼ�����
		m_AxisState_list.SetImageList(&m_imagelist, LVSIL_SMALL);
	}
	AxisName[AXIS_LEFT] = L"����ŷ�";
	AxisName[AXIS_RIGHT] = L"�Ҷ��ŷ�";
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CMotionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
			AfxMessageBox(L"��λ��ɣ�");
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

//�ŷ�ʹ��
void CMotionDlg::OnBnClickedAxisenableButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"�Զ������У�����ִ��!	");
		return;
	}
	int  axisno = m_Axis_combo.GetCurSel();
	AxisEnable(axisno);
}

//��λ
void CMotionDlg::OnBnClickedRstButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (1 == G::bRunflag)
	{
		AfxMessageBox(L"�Զ������У����ܸ�λ!	");
		return;
	}
	int  axisno = m_Axis_combo.GetCurSel();
	if (0 == dmc_check_done(0, axisno))
	{
		AfxMessageBox(L"���˶��У�����ֹͣ���˶���");
		return;
	}
	RSTing = 1;
	G::MCard.HomeAxis2(axisno, 0, 1, 2);

}


// ��ʹ��
BOOL CMotionDlg::AxisEnable(UINT  AxisNo)
{
	//��ʹ��
	if (G::MCard.A[AxisNo].AP.bAxsSvnEnable)
	{
		if (IDYES == MessageBox(L"ȷ��Ҫ�ر�" + AxisName[AxisNo] + L"ʹ����", L"��ʹ��", MB_YESNO | MB_ICONWARNING))
		{
			if (MCARD_NO_ERROR != G::MCard.EnableAxis(AxisNo, 0))
			{
				MessageBox(L"��ʹ�ܹر�ʧ�ܣ�", L"��ʾ", MB_ICONERROR);
			}
			G::MCard.A[AxisNo].AS.bHome = 0;
		}
	}
	else
	{
		if (IDYES == MessageBox(L"ȷ��Ҫ��" + AxisName[AxisNo] + L"ʹ����", L"��ʹ��", MB_YESNO | MB_ICONWARNING))
		{
			if (MCARD_NO_ERROR != G::MCard.EnableAxis(AxisNo, 1))
			{
				MessageBox(L"��ʹ�ܴ�ʧ�ܣ�", L"��ʾ", MB_ICONERROR);
			}
			G::MCard.SetAxisCurPos(AxisNo, 0);
			G::MCard.SetAxisEncPos(AxisNo, 0);

		}
	}
	return 0;
}


void CMotionDlg::OnBnClickedStopButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//1����ֹͣ
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"�Զ������У����ܵ���ֹͣ!	");
		return;
	}
	int axisno = m_Axis_combo.GetCurSel();
	G::MCard.StopAxis(axisno, 1);                 //����ֹͣ
}

//λ������
void CMotionDlg::OnBnClickedPostozeroButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (1 == G::bRunflag)
	{
		AfxMessageBox(L"�����Զ������У���������!	");
		return;
	}
	int   axisno = m_Axis_combo.GetCurSel();
	G::MCard.SetAxisCurPos(axisno, 0);
	G::MCard.SetAxisEncPos(axisno, 0);
	G::MCard.A[axisno].AS.bHome = 0;
}

//�����˶�
void CMotionDlg::OnBnClickedPosrunButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"�Զ������У����ܵ����ֶ�!	");
		return;
	}
	int  axisno = m_Axis_combo.GetCurSel();
	if (0 == dmc_check_done(0, axisno))
	{
		AfxMessageBox(L"���˶��У�");
		return;
	}
	UpdateData(TRUE);
	G::MCard.RMove2(axisno, float(m_speed_edit/**G::MCard.A[axisno].AP.dPPU*/), long(m_distance_edit/**G::MCard.A[axisno].AP.dPPU*/));

}

//�����˶�
void CMotionDlg::OnBnClickedNegrunButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (1 == G::bRunning)
	{
		AfxMessageBox(L"�Զ������У����ܵ����ֶ�!	");
		return;
	}
	int  axisno = m_Axis_combo.GetCurSel();
	if (0 == dmc_check_done(0, axisno))
	{
		AfxMessageBox(L"���˶��У�");
		return;
	}
	UpdateData(TRUE);
	G::MCard.RMove2(axisno, float(m_speed_edit/**G::MCard.A[axisno].AP.dPPU*/), -long(m_distance_edit/**G::MCard.A[axisno].AP.dPPU*/));
}


// ˢ����״̬�б�
void CMotionDlg::RefreshAxisStateList()
{
	for (int item = 0; item < 2; item++)
	{
		LV_ITEM   lvitem;
		memset((char *)&lvitem, '\0', sizeof(lvitem));
		lvitem.mask = LVIF_TEXT | LVIF_STATE | LVIF_IMAGE;
		lvitem.iItem = item;
		lvitem.stateMask = 0;

		//������
		lvitem.iSubItem = 1;
		m_AxisState_list.SetItemText(item, lvitem.iSubItem, AxisName[item]);

		//�ᱨ��
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

		//��ʹ��
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

		//����λ
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

		//ԭ��
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

		//����λ
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

		//���˶�
		lvitem.iSubItem++;
		if (0 == G::MCard.A[item].AS.bDone)     //�˶��� 
		{
			lvitem.iImage = 1;                        //��ɫͼ��
		}
		else
		{
			lvitem.iImage = 0;
		}
		m_AxisState_list.SetItem(&lvitem);

		//��ǰλ��
		lvitem.iSubItem++;
		CString  str;
		long  pos = dmc_get_position(0, item);
		str.Format(L"%ld", pos);
		m_AxisState_list.SetItemText(item, lvitem.iSubItem, str);

		//����λ��
		lvitem.iSubItem++;
		long  enpos = dmc_get_encoder(0, item);
		str.Format(L"%ld", enpos);
		m_AxisState_list.SetItemText(item, lvitem.iSubItem, str);
	}

}

//�ƶ���λ�õ㰴ť
void CMotionDlg::OnBnClickedMovetoposButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (1 == G::bRunflag)
	{
		MessageBox(L"�Զ����������� �������ֶ��ƶ��ᣡ", L"����", MB_ICONWARNING);
		return;
	}
	int  id = GetCheckedRadioButton(IDC_SCRABFIRST1_RADIO, IDC_SCRABNEXT2_RADIO);
	long  pos, curpos;
	int  axisno;                //��Ӧ�˶����
	int  speed = GetDlgItemInt(IDC_SPEED_EDIT);
	int  j;             ///������
	switch (id)
	{
	case IDC_SCRABFIRST1_RADIO:
		//��߳�ʼ����λ��
		//int  i = curpos / (long)G::PR[3].paravalue;           //����
		axisno = AXIS_LEFT;
		curpos = dmc_get_position(0, axisno);
		//��������
		j = (long)(curpos - G::PR[MP_LeftCamFirstPos].paravalue) % (long)G::PR[MP_LeftCamIntervalDis].paravalue;
		//��������0����ʣ�����
		if (j >= 0)
			pos = G::PR[MP_LeftCamIntervalDis].paravalue - j;
		else
			pos = abs(j);
		//j = curpos % (long)G::PR[3].paravalue;         //����
		//pos = (long)G::PR[2].paravalue + (long)G::PR[3].paravalue - j;
		break;
		//��ǰλ�ü����ռ������
	case IDC_SCRABNEXT1_RADIO:
		axisno = AXIS_LEFT;
		curpos = dmc_get_position(0, axisno);
		//��������
		j = (long)(curpos - G::PR[MP_LeftCamFirstPos].paravalue) % (long)G::PR[MP_LeftCamIntervalDis].paravalue;
		//��������0����ʣ�����
		if (j >= 0)
			pos = G::PR[MP_LeftCamIntervalDis].paravalue - j;
		else
			pos = abs(j);
		break;
	case IDC_SCRABFIRST2_RADIO:                //�Ҷ���λ
		axisno = AXIS_RIGHT;
		curpos = dmc_get_position(0, axisno);
		//��������
		j = (long)(curpos - G::PR[MP_RightCamFirstPos].paravalue) % (long)G::PR[MP_RightCamIntervalDis].paravalue;
		//��������0����ʣ�����
		if (j >= 0)
			pos = G::PR[MP_RightCamIntervalDis].paravalue - j;
		else
			pos = abs(j);
		break;
	case IDC_SCRABNEXT2_RADIO:                    //�Ҷ���һλ
		axisno = AXIS_RIGHT;
		curpos = dmc_get_position(0, axisno);
		//��������
		j = (long)(curpos - G::PR[MP_RightCamFirstPos].paravalue) % (long)G::PR[MP_RightCamIntervalDis].paravalue;
		//��������0����ʣ�����
		if (j >= 0)
			pos = G::PR[MP_RightCamIntervalDis].paravalue - j;
		else
			pos = abs(j);
		break;
	default:
		AfxMessageBox(L"����ָ��Ŀ��λ�ú��ٽ����ƶ�");
		return;
		break;
	}
	if (dmc_check_done(0, axisno) != 1)
	{
		AfxMessageBox(L"���˶��У�����ֹͣ���˶�!");
		return;
	}
	if ((axisno == AXIS_LEFT) && (0 == G::bLeftRstOK))
	{
		AfxMessageBox(L"���ȸ�λ����ŷ���");
		return;
	}
	if ((axisno == AXIS_RIGHT) && (0 == G::bRightRstOK))
	{
		AfxMessageBox(L"���ȸ�λ�Ҷ��ŷ���");
		return;
	}
	UpdateData(TRUE);
	//����˶�
	G::MCard.RMove2(axisno, speed, pos);
	////�����ٶ�
	//dmc_set_profile(0, 0, 500, speed, 0.1, 0.1, 500);
	////���λ��
	//dmc_pmove(0, 0, pos, 0);                     //0�������ģʽ
	////G::MCard.AMove2(0, speed, pos);
}

//����λ�õ㰴ť
void CMotionDlg::OnBnClickedSaveposButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int  id = GetCheckedRadioButton(IDC_SCRABFIRST1_RADIO, IDC_SCRABNEXT2_RADIO);
	int no;                        //PR�������±�
	int  axisno;                //��Ӧ�˶����
	int pos;
	switch (id)
	{
	case IDC_SCRABFIRST1_RADIO:               //����λ
		axisno = AXIS_LEFT;
		no = MP_LeftCamFirstPos;
		break;
	case IDC_SCRABNEXT1_RADIO:               //����һ��
		axisno = AXIS_LEFT;
		no = MP_LeftCamIntervalDis;
		break;
	case IDC_SCRABFIRST2_RADIO:              //����λ
		axisno = AXIS_RIGHT;
		no = MP_RightCamFirstPos;
		break;
	case IDC_SCRABNEXT2_RADIO:
		axisno = AXIS_RIGHT;
		no = MP_RightCamIntervalDis;
		break;
	default:
		AfxMessageBox(L"��ѡ�񱣴�λ�õ��ٰ����水ť��");
		return;
		break;
	}
	if ((axisno == AXIS_LEFT) && (0 == G::bLeftRstOK))
	{
		AfxMessageBox(L"���ȸ�λ����ŷ���");
		return;
	}
	if ((axisno == AXIS_RIGHT) && (0 == G::bRightRstOK))
	{
		AfxMessageBox(L"���ȸ�λ�Ҷ��ŷ���");
		return;
	}
	if ((id == IDC_SCRABNEXT1_RADIO) || (id == IDC_SCRABNEXT2_RADIO))
	{
		AfxMessageBox(L"���ܸ��ģ�������ѡ��!");
		return;
	}
	if (IDNO == MessageBox(L"ȷ�����浱ǰλ�õ㣿", L"��ʾ", MB_YESNO | MB_ICONQUESTION))
		return;
	//����λ��
	if (axisno==AXIS_LEFT)
		pos = (long)(dmc_get_position(0, axisno) + G::PR[MP_LeftCamIntervalDis].paravalue) % (long)G::PR[MP_LeftCamIntervalDis].paravalue;
	else if (axisno==AXIS_RIGHT)
		pos = (long)(dmc_get_position(0, axisno) + G::PR[MP_RightCamIntervalDis].paravalue) % (long)G::PR[MP_RightCamIntervalDis].paravalue;
	G::PR[no].paravalue = pos;
	if (0 == G::WriteCommonPara())
	AfxMessageBox(L"��������ɹ���");
	else
	AfxMessageBox(L"��������ʧ�ܣ�");
	/*if (i % 2 == 0)
		G::PR[i].paravalue = G::MCard.GetAxisCurPos(0);
	else
		G::PR[i].paravalue = G::MCard.GetAxisCurPos(0) - G::PR[i - 1].paravalue;*/
	/*if (0 == G::WriteCommonPara())
		AfxMessageBox(L"��������ɹ���");
	else
		AfxMessageBox(L"��������ʧ�ܣ�");*/
}
