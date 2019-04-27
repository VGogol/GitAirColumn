#pragma once
#include "stdafx.h"
#include"CApplication.h"
#include"CRange.h"
#include"CWorkbook.h"
#include"CWorkbooks.h"
#include"CWorksheets.h"
#include"CWorksheet.h"
#include"CFont0.h"
#include"CBorders.h"
#include"Cnterior.h"
#include "afxwin.h"
#include "afxcmn.h"
#include<Shlwapi.h>
#include<locale.h>
#include"G.h"
class CFileRW
{
public:
	CFileRW();
	~CFileRW();

public:
	CApplication app;
	CRange range;
	CRange usedrange;
	CWorkbook book;
	CWorkbooks books;
	CWorksheets sheets;
	CWorksheet sheet;
	LPDISPATCH lpdisp;
	CFont0  font;
public:
	CString readxlspath;

public:
	//�б�ؼ�д��Excel����ԭ�������
	//�������ؼ��������洢·������ͷ���飬�ؼ��������ؼ�����
	void ListToExcel(CListCtrl *datalist, CString filepath, CString str[], int rowcount, int colcount);
	
	//Excel��ȡ��list�ؼ�
	//�������ؼ�����
	void ExcelToList(CListCtrl *datalist);

	//list�ؼ����ݲ���Excel��ԭ�ļ��������
	//�������ؼ������������ļ�·�����ؼ��������ؼ�����
	void ListInsertExcel(CListCtrl *datalist, CString filepath, int rowcount, int colcount);

	//�ַ���д��text
	//�������ļ�·����д���ַ���
	void WriteToTxt(CString filepath,CString insertstr);

	//�ı�Excel���壬��ɫ�������С
	//�ļ�·������ʼ�У���ʼ�У������У�������,���壬������ɫ�����������С
	void ExcelChangeFont(CString filepath, long startrowfont, long startcolfont,
		long endrowfont,long endcolfont,CString fonts,long colorindex,long fontsize);

	//����Excel��Ԫ����ʽ
	//�ļ�·������ʼ�У���ʼ�У������У������У��߿�����ʽ���߿��߿�ȣ��߿�����ɫ����Ԫ���п��Զ���Ӧ����Ԫ�������Զ�����
	void ExcelBoarderSet(CString filepath,long startrowboard, long startcolboard, 
		long endrowboard,long endcolboard,long linestyle,long linewidth,long color);

	//�ͷ���Դ
	void release();

	//������
	// ���������ݵ�N�����ݲ���Excel����Excel�еĵ��У������е����������޷���֪��
	//���������������������ݣ������ļ�·���������кţ������кţ���ǰ�ĵ���ʹ�����һ�е���һ��Ϊ��һ�У�
	int InsertBatchData(int counts, CPtrList *list, VARIANT data, CString filepath, int rowno, int colno);
	
	//��1269������������  2018.12.4
	// �����õ��ĳߴ�������������д��Excel
	//�����������ļ�·������ʼ�У���ʼ��
	int ResultLinkedToExcel(int counts, CString filepath, int rowno, int colno);

	////��1269������������  2018.12.7
	//��Excel�ļ��в���һ������
	int  InsertOneItem1269(CString filepath);

	// ��Excel���򲢽��г�ʼ��,ͬʱ�򿪻��½�һ���ļ�
	int OpenExcel(CString filepath);
};

