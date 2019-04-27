#include "stdafx.h"
#include "FileRW.h"

//��ע
//////�����ļ�·��
//ʱ�����
//CString  excelpath;                       //д���ļ�·����
//CString  filepath1 = L"D\\�������������\\�ߴ�����";
//CTime  time = CTime::GetCurrentTime();
//CString strRptTime = time.Format("%Y%m%d");
//excelpath = filepath1 + strRptTime + L".xlsx";


CFileRW::CFileRW()
{
}


CFileRW::~CFileRW()
{
}

//�б�ؼ�д��EXCEL,���ԭ���
//�������ؼ��������洢·������ͷ���飬�ؼ��������ؼ�����
void CFileRW::ListToExcel(CListCtrl *datalist, CString filepath, CString str[], int rowcount, int colcount)   //�洢�ļ�·�����ؼ��������ؼ�����
{
	//�����ͷ���鳤��
	int  length = sizeof(str) / sizeof(str[0]);
	//list�ؼ�����д��Excel
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//����Excel ������(����Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("����Excel������ʧ��!"));
		return;
	}
	////�ڸ���ʱ�Ƿ���ʾ������ΪFALSE
	//app.put_AlertBeforeOverwriting(FALSE);
	//��ʾ���档����ΪFALSE
	//app.put_DisplayAlerts(FALSE);
	//// ����Ϊ�ɼ�
	//m_app.put_Visible(FALSE);
	//���ø����ļ�·��
	/*CString strRptTime = time.Format("%Y%m%d%H%M");
	CString strBookPath = ("D:\\�������\\check data" + strRptTime + ".xlsx");*/
	//ʱ����ص��ļ�����
	//CFile file(L"D:/data insert/checkdevice.xls", CFile::modeCreate|CFile::modeWrite);
	//CString fileName = "d:\\" + strRptTime + ".xls";
	/*CTime time = CTime::GetCurrentTime();
	CString strRptTime = time.Format("%Y%m%d%H%M%S");*/
	//�ļ�·��
	//CString strBookPath = _T("D:\\�������.xlsx");
	/*�õ�����������*/
	books.AttachDispatch(app.get_Workbooks());	
	/*��һ�����������粻���ڣ�������һ��������*/
	try
	{
		/*��һ��������*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		/*����һ���µĹ�����*/
		lpdisp = books.Add(covOptional);
		book.AttachDispatch(lpdisp);
	}
	/*�õ��������е�Sheet������*/
	sheets.AttachDispatch(book.get_Sheets());
	sheet = sheets.get_Item(COleVariant((short)1));   //�õ���һ��������
	CHeaderCtrl   *pmyHeaderCtrl = datalist->GetHeaderCtrl(); //��ȡ��ͷ
	usedrange.AttachDispatch(sheet.get_UsedRange());
	///*��һ��Sheet���粻���ڣ�������һ��Sheet*/
	//CString strSheetName = _T("listdata");
	//try
	//{
	//	/*��һ�����е�Sheet*/
	//	m_lpdisp = m_sheets.get_Item(_variant_t(strSheetName));
	//	m_sheet.AttachDispatch(m_lpdisp);
	//}
	//catch (...)
	//{
	//	/*����һ���µ�Sheet*/
	//	m_lpdisp = m_sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
	//	m_sheet.AttachDispatch(m_lpdisp);
	//	m_sheet.put_Name(strSheetName);
	//}
	/*m_range = m_sheet.get_Range(_variant_t(_T("A")), _variant_t(_T("A")));
	m_range.put_Value2(_variant_t("��Եֱ��"));
	m_range = m_sheet.get_Range(_variant_t("B"), _variant_t("B"));
	m_range.put_Value2(_variant_t("��Բ��"));
	m_range = m_sheet.get_Range(_variant_t("C"), _variant_t("C"));
	m_range.put_Value2(_variant_t("��Բ��"));*/
	try
	{
		//��ȡ��ǰ��ȡ��EXCEL�л��sheet
		usedrange.AttachDispatch(sheet.get_Cells());
		//���Excel�е�����
		usedrange.ClearContents();
		//�Զ���Ӧ
		range = sheet.get_Columns();
		range.AutoFit();
		range = sheet.get_Rows();
		range.AutoFit();
		//��������
		for (int i = 1; i <= colcount; i++)
		{
			range.AttachDispatch(usedrange.get_Item((_variant_t((long)1)),
				(_variant_t)(long)i).pdispVal);
			range.put_Value2((_variant_t (str[i-1])));
		}
		
		/*range.AttachDispatch(usedrange.get_Item((_variant_t((long)1)),
			(_variant_t)(long)2).pdispVal);
		range.put_Value2((_variant_t(L"��Բ��")));
		range.AttachDispatch(usedrange.get_Item((_variant_t((long)1)),
			(_variant_t)(long)3).pdispVal);
		range.put_Value2((_variant_t(L"��Բ��")));*/
		//���������
		CString	 str1;
		for (int row = 0; row < rowcount; row++)
		{
			for (int col = 0; col < colcount; col++)
			{
				str1 = datalist->GetItemText(row, col);
				range.AttachDispatch(usedrange.get_Item(_variant_t((long)row + 2),
					_variant_t((long)col + 1)).pdispVal);
				range.put_Value2(_variant_t(str1));
			}
		}
		//���߸�����Ϊ����ģʽ��1��Ĭ��ֵ�������ķ���ģʽ��,2�������б�,3����ռģʽ��
		book.SaveAs(COleVariant(filepath), covOptional, covOptional, covOptional,
			covOptional, covOptional, 1, covOptional, covOptional, covOptional,
			covOptional, covOptional);
		release();
		AfxMessageBox(L"����ɹ�");
		//m_used_range.AttachDispatch(m_used_range.get_Item(COleVariant((long)1), (COleVariant)(long)1).pdispVal);
	}
	catch (CException *e)
	{
		release();
		AfxMessageBox(L"����ʧ��");
	}
}

//EXCEL���ݶ�ȡ���б�ؼ�
void CFileRW::ExcelToList(CListCtrl *datalist)
{
	CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"excel�ļ�(*.xls)|*.xlsx|ALL Files(*.*)||");         //TRUEΪ�򿪣�FALSΪ����
	if (IDOK != filedlg.DoModal())
	{
		return;
	}
	//��ȡ�ļ�·��
	readxlspath = filedlg.GetPathName();
	if (!PathFileExists(readxlspath))
	{
		AfxMessageBox(readxlspath + "������");
		return;
	}
	if (!app.CreateDispatch(L"Excel.Application"))    //�����ӿڶ���
	{
		AfxMessageBox(L"�޷�����Excel������");
		return;
	}
	COleVariant	covOption((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//�õ�������
	books.AttachDispatch(app.get_Workbooks());
	lpdisp = books.Open(readxlspath, covOption, covOption, covOption, covOption,
		covOption, covOption, covOption, covOption, covOption, covOption,
		covOption, covOption, covOption, covOption);
	//�õ�Workbook
	book.AttachDispatch(lpdisp);
	//sheets
	sheets.AttachDispatch(book.get_Worksheets());
	////�õ�WorkSheet
	//sheet.AttachDispatch(sheets.get_sheet());
	//�õ���ǰ��Ծsheet
	//����е�Ԫ�������ڱ༭״̬���˲������᷵�أ���һֱ�ȴ�
	lpdisp = book.get_ActiveSheet();
	sheet.AttachDispatch(lpdisp);
	usedrange.AttachDispatch(sheet.get_UsedRange());
	//��ȡ��һ����Ԫ���ֵ
	//range.AttachDispatch(sheet.get_UsedRange());
	//m_range.AttachDispatch(m_range.get_Item(COleVariant((long)1), COleVariant((long)1)).pdispVal);
	COleVariant vResult,vResult1;
	//��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
	range.AttachDispatch(sheet.get_UsedRange());
	//ȡ���Ѿ�ʹ�õ�����
	range.AttachDispatch(range.get_Rows());
	long rownum = range.get_Count();
	//ȡ���Ѿ�ʹ�õ�����
	range.AttachDispatch(range.get_Columns());
	long colnum = range.get_Count();
	//ȡ����ʹ���������ʼ�У���1��ʼ
	long startrow = range.get_Row();
	//ȡ����ʹ���������ʼ�У���1��ʼ
	long startcol = range.get_Column();
	////������ͷ
	//CString str1;
	//for ( long col0 = 1;  col0 <= colnum;  col0++)
	//{
	//	range.AttachDispatch(usedrange.get_Item(COleVariant((long)1),
	//		COleVariant(col0)).pdispVal);
	//	str1 = range.get_Value2()/*.bstrVal*/;
	//	datalist->InsertColumn(col0, str1, LVCFMT_CENTER, 70);
	//}
	datalist->DeleteAllItems();
	for (int i = startrow; i <= rownum; i++)
	{
		//�Ȳ������ף�������Ԫ��
		range.AttachDispatch(sheet.get_Cells());
		range.AttachDispatch(range.get_Item(_variant_t((long)i+1),
			_variant_t((long)startcol)).pdispVal);
		vResult = range.get_Value2();
		CString strread, stry0, strm0, strd0;
		SYSTEMTIME st0;
		if (vResult.vt == VT_BSTR)     //�����ַ���
		{
			strread = vResult.bstrVal;
		}
		else if (vResult.vt == VT_R8) //8�ֽڵ�����
		{
			strread.Format(L"%f", vResult.dblVal);
		}
		else if (vResult.vt == VT_DATE) //ʱ���ʽ
		{
			VariantTimeToSystemTime(vResult.date, &st0);
			stry0.Format(L"%d", st0.wYear);
			strm0.Format(L"%d", st0.wMonth);
			strd0.Format(L"%d", st0.wDay);
			strread = stry0 + L"-" + strm0 + L"-" + strd0;
		}
		else if (vResult.vt == VT_EMPTY) //��ԪΪ��
		{
			strread = L"";
		}
		else if (vResult.vt == VT_I4)
		{
			strread.Format(_T("%ld"), (int)vResult.lVal);
		};
		datalist->InsertItem(i , strread);

      //�������Ԫ��
		for (int j = startcol+1; j <= colnum; j++)
		{
			//��ȡ��Ԫ���ֵ
			range.AttachDispatch(sheet.get_Cells());
			range.AttachDispatch(range.get_Item(_variant_t((long)i + 1),
				_variant_t((long)j)).pdispVal);
			vResult1 = range.get_Value2();
			CString str, stry, strm, strd;
			SYSTEMTIME st;
			if (vResult1.vt == VT_BSTR)     //�����ַ���
			{
				str = vResult1.bstrVal;
			}
			else if (vResult1.vt == VT_R8) //8�ֽڵ�����
			{
				str.Format(L"%f", vResult1.dblVal);
			}
			else if (vResult1.vt == VT_DATE) //ʱ���ʽ
			{
				VariantTimeToSystemTime(vResult1.date, &st);
				stry.Format(L"%d", st.wYear);
				strm.Format(L"%d", st.wMonth);
				strd.Format(L"%d", st.wDay);
				str = stry + L"-" + strm + L"-" + strd;
			}
			else if (vResult1.vt == VT_EMPTY) //��ԪΪ��
			{
				str = L"";
			}
			else if (vResult1.vt == VT_I4)
			{
				str.Format(_T("%ld"), (int)vResult1.lVal);
			}
			//datalist->SetItemText(static_cast<int>(i), static_cast<int>(j),str);
			datalist->SetItemText(i-1, j-1, str);
		}
	}
	release();
	AfxMessageBox(L"��ȡ�ɹ�");
}

//list�ؼ����ݲ���Excel��ԭ�ļ��������
//�ؼ������������ļ�·�����ؼ��������ؼ�����
void CFileRW::ListInsertExcel(CListCtrl *datalist, CString filepath, int rowcount, int colcount)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//����Excel ������(����Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("����Excel������ʧ��!"));
		return;
	}
	/*�õ�����������*/
	books.AttachDispatch(app.get_Workbooks());
	/*��һ�����������粻���ڣ��򱨴�*/
	try
	{
		/*��һ��������*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		//��ʧ�ܻ����ļ��������򱨴�
		AfxMessageBox(filepath+"�ļ���ʧ��");
		return;
	}
	/*�õ��������е�Sheet������*/
	sheets.AttachDispatch(book.get_Sheets());
	sheet = sheets.get_Item(COleVariant((short)1));   //�õ���һ��������
	CHeaderCtrl   *pmyHeaderCtrl = datalist->GetHeaderCtrl(); //��ȡ��ͷ
	usedrange.AttachDispatch(sheet.get_UsedRange());
	//��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
	range.AttachDispatch(sheet.get_UsedRange());
	//ȡ��Excel�Ѿ�ʹ�õ�����
	range.AttachDispatch(range.get_Rows());
	long usedrownum = range.get_Count();
	//ȡ��Excel�Ѿ�ʹ�õ�����
	range.AttachDispatch(range.get_Columns());
	long usedcolnum = range.get_Count();
	//���������
	try
	{
		//��ȡ��ǰ��ȡ��EXCEL�л��sheet
		usedrange.AttachDispatch(sheet.get_Cells());
		CString	 str1;
		for (int row = 0; row < rowcount ; row++)
		{
			for (int col = 0; col < colcount; col++)
			{
				str1 = datalist->GetItemText(row, col);
				//��ȡ��Excel�еĲ���λ��
				range.AttachDispatch(usedrange.get_Item(_variant_t((long)row + 1 + usedrownum),
					_variant_t((long)col + 1)).pdispVal);
				range.put_Value2(_variant_t(str1));
				//// Ȼ��������߿�  
				//if (TRUE==enableboarderset)
				//{
				//	ExcelBoarderSet();
				//}
				//else
				//{
				//	enableboarderset = 0;
				//}
			}
		}
		//�ڸ���ʱ�Ƿ���ʾ������ΪFALSE
		app.put_AlertBeforeOverwriting(FALSE);
		//��ʾ���档����ΪFALSE
		app.put_DisplayAlerts(FALSE);
		//���߸�����Ϊ����ģʽ��1��Ĭ��ֵ�������ķ���ģʽ��,2�������б�,3����ռģʽ��
		book.SaveAs(COleVariant(filepath), covOptional, covOptional, covOptional,
			covOptional, covOptional, 1, covOptional, covOptional, covOptional,
			covOptional, covOptional);
		release();
		//AfxMessageBox(L"����ɹ�");
		//m_used_range.AttachDispatch(m_used_range.get_Item(COleVariant((long)1), (COleVariant)(long)1).pdispVal);
	}
	catch (CException *e)
	{
		release();
		AfxMessageBox(L"����ʧ��");
	}
}

//�ַ���д��text
void	CFileRW::WriteToTxt(CString filepath, CString insertstr)
{
	CStdioFile file;
	CString strline;
	BOOL flag,flag1;
	/*char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");*/
	//��text�ļ�
	flag = file.Open(filepath, CFile::modeReadWrite);
	if (FALSE == flag)
	{
		flag1 = file.Open(filepath, CFile::modeCreate|CFile::modeReadWrite);
	}
	else
	{
		flag1 = 1;
	}
	/*try
	{
		flag = file.Open(filepath, CFile::modeReadWrite);
		if (FALSE == flag)
		{
			throw flag;
		}
	}
	catch (BOOL flag)
	{
		flag = file.Open(filepath, CFile::modeCreate);
	}*/
	if (FALSE == flag1)
	{
		AfxMessageBox(L"�ļ���ʧ�ܣ�");
	}
	file.SeekToEnd();
	strline = L"\n" + insertstr;
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//�趨
	file.WriteString(strline);
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//��ԭ�����趨
	AfxMessageBox(L"�ɹ�������ĵ�ĩβ");
	file.Close();
}

//�ı�Excel��Ԫ����ɫ,���壬�����С
//�ļ�·������ʼ�У���ʼ�У������У������У����壬������ɫ�����������С
void CFileRW::ExcelChangeFont(CString filepath, long startrowfont, long startcolfont,
	long endrowfont, long endcolfont, CString fonts, long colorindex, long fontsize)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//����Excel ������(����Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("����Excel������ʧ��!"));
		return;
	}
	/*�õ�����������*/
	books.AttachDispatch(app.get_Workbooks());
	/*��һ�����������粻���ڣ�������һ��������*/
	try
	{
		/*��һ��������*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		//��ʧ�ܻ����ļ��������򱨴�
		AfxMessageBox(filepath + "�ļ���ʧ��");
		return;
	}
	/*�õ��������е�Sheet������*/
	sheets.AttachDispatch(book.get_Sheets());
	sheet = sheets.get_Item(COleVariant((short)1));   //�õ���һ��������
	usedrange.AttachDispatch(sheet.get_UsedRange());
	//��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
	range.AttachDispatch(sheet.get_UsedRange());
	font.AttachDispatch(range.get_Font());
	for (long i = startrowfont; i <= endrowfont; i++)
	{
		for (long j = startcolfont; j <= endcolfont; j++)
		{
			range.AttachDispatch(usedrange.get_Item(_variant_t((long)i),
				_variant_t((long)j)).pdispVal);
			font.put_Name(_variant_t("���Ŀ���"));//���� 
			font.put_ColorIndex(_variant_t((long)3));//�ֵ���ɫ 
			font.put_Size(_variant_t((long)20));//�ֺ� 
		}
	}
	usedrange.ReleaseDispatch();
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	font.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.Quit();
	app.ReleaseDispatch();
}


//����Excel��Ԫ����ʽ
//�ļ�·������ʼ�У���ʼ�У������У������У��߿�����ʽ���߿��߿��߿�����ɫ����Ԫ���п��Զ���Ӧ�����ж���
void CFileRW::ExcelBoarderSet(CString filepath,long startrowboard, long startcolboard,
	long endrowboard, long endcolboard, long linestyle, long linewidth, long color)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//����Excel ������(����Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("����Excel������ʧ��!"));
		return;
	}
	/*�õ�����������*/
	books.AttachDispatch(app.get_Workbooks());
	/*��һ�����������粻���ڣ�������һ��������*/
	try
	{
		/*��һ��������*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		//��ʧ�ܻ����ļ��������򱨴�
		AfxMessageBox(filepath + "�ļ���ʧ��");
		return;
	}
	/*�õ��������е�Sheet������*/
	sheets.AttachDispatch(book.get_Sheets());
	sheet = sheets.get_Item(COleVariant((short)1));   //�õ���һ��������
	usedrange.AttachDispatch(sheet.get_UsedRange());
	//��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
	range.AttachDispatch(sheet.get_UsedRange());
	range.put_HorizontalAlignment(COleVariant((long)-4108));
	range.put_VerticalAlignment(COleVariant((long)-4108));    // -4108:���У�-4160:���ϣ�-4107:���� 
	//range.AutoFit();
	range = sheet.get_Columns();
	range.AutoFit();
	range = sheet.get_Rows();
	range.AutoFit();
	for (long i = startrowboard; i <= endrowboard; i++)
	{
		for (long j = startcolboard; j <= endcolboard; j++)
		{
			range.AttachDispatch(usedrange.get_Item(_variant_t((long)i  ),
				_variant_t((long)j )).pdispVal);
			// LineStyle=����(1~13) Weight=�߿� ColorIndex=�ߵ���ɫ(-4105Ϊ�Զ�, 1Ϊ��ɫ)   
			long lColor = RGB(255, 0, 0);
			COleVariant varColor(lColor);
			range.BorderAround(_variant_t(long(linestyle)), linewidth, color, 
				vtMissing, varColor);//���ñ߿� 
			//range.SetColumnWidth(_variant_t((long)12));
		}
	}
	release();
}

//�ͷ���Դ
void CFileRW::release()
{
	usedrange.ReleaseDispatch();
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.Quit();
	app.ReleaseDispatch();
}

//������
// ���������ݵ�N�����ݲ���Excel����Excel�еĵ��У������е����������޷���֪��
//���������������������ݣ������ļ�·���������кţ������кţ���ǰ�ĵ���ʹ�����һ�е���һ��Ϊ��һ�У�
int CFileRW::InsertBatchData(int counts, CPtrList *list, VARIANT data, CString filepath, int rowno, int colno)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//����Excel ������(����Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("����Excel������ʧ��!"));
		return -1;
	}
	books.AttachDispatch(app.get_Workbooks());
	/*��һ�����������粻���ڣ�������һ��������*/
	try
	{
		/*��һ��������*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		/*����һ���µĹ�����*/
		lpdisp = books.Add(covOptional);
		book.AttachDispatch(lpdisp);
	}
	/*�õ��������е�Sheet������*/
	sheets.AttachDispatch(book.get_Worksheets());
	sheet.AttachDispatch(book.get_ActiveSheet());          //////��ȡ��ǰ��Ծ��sheet
	//sheets.AttachDispatch(book.get_Sheets());
	//sheet = sheets.get_Item(COleVariant((short)1));   //�õ���һ��������

	usedrange.AttachDispatch(sheet.get_UsedRange());
	//��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
	range.AttachDispatch(sheet.get_UsedRange());
	//ȡ��Excel�Ѿ�ʹ�õ�����
	range.AttachDispatch(range.get_Rows());
	long usedrownum = range.get_Count();
	//ȡ��Excel�Ѿ�ʹ�õ�����
	range.AttachDispatch(range.get_Columns());
	long usedcolnum = range.get_Count();
	//���������
	try
	{
		//��ȡ��ǰ��ȡ��EXCEL�л��sheet
		usedrange.AttachDispatch(sheet.get_Cells());
		//��ȡ��Excel�еĲ���λ��
		range.AttachDispatch(usedrange.get_Item(_variant_t((long)rowno  + usedrownum),
			_variant_t((long)colno )).pdispVal);
		range.put_Value2(data);
				//// Ȼ��������߿�  
				//if (TRUE==enableboarderset)
				//{
				//	ExcelBoarderSet();
				//}
				//else
				//{
				//	enableboarderset = 0;
				//}
		/////////////////////////////////////////�����޸��ļ�ʱ������
		//�ڸ���ʱ�Ƿ���ʾ������ΪFALSE
		app.put_AlertBeforeOverwriting(FALSE); 
		//��ʾ���档����ΪFALSE
		app.put_DisplayAlerts(FALSE);
		//���߸�����Ϊ����ģʽ��1��Ĭ��ֵ�������ķ���ģʽ��,2�������б�,3����ռģʽ��
		book.SaveAs(COleVariant(filepath), covOptional, covOptional, covOptional,
			covOptional, covOptional, 3, covOptional, covOptional, covOptional,
			covOptional, covOptional);
		//release();
	}
	catch (CException *e)
	{
		release();
		AfxMessageBox(L"����ʧ��");
		return -1;
	}

	return 0;
}

////��1269������������
//// �����õ��ĳߴ�������������д��Excel
//int CFileRW::ResultLinkedToExcel(int counts, CString filepath, int rowno, int colno)
//{
//	COleVariant
//		covTrue((short)TRUE),
//		covFalse((short)FALSE),
//		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
//	//����Excel ������(����Excel)
//	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
//	{
//		AfxMessageBox(_T("����Excel������ʧ��!"));
//		return -1;
//	}
//	////�ڸ���ʱ�Ƿ���ʾ������ΪFALSE
//	app.put_AlertBeforeOverwriting(FALSE);
//	//��ʾ���档����ΪFALSE
//	app.put_DisplayAlerts(FALSE);
//
//	books.AttachDispatch(app.get_Workbooks());
//	/*��һ�����������粻���ڣ�������һ��������*/
//	try
//	{
//		/*��һ��������*/
//		lpdisp = books.Open(filepath,
//			covOptional, covOptional, covOptional, covOptional, covOptional,
//			covOptional, covOptional, covOptional, covOptional, covOptional,
//			covOptional, covOptional, covOptional, covOptional);
//		book.AttachDispatch(lpdisp);
//	}
//	catch (...)
//	{
//		/*����һ���µĹ�����*/
//		lpdisp = books.Add(covOptional);
//		book.AttachDispatch(lpdisp);
//	}
//	/*�õ��������е�Sheet������*/
//	sheets.AttachDispatch(book.get_Sheets());
//	//sheet = sheets.get_Item(COleVariant((short)1));   //�õ���һ��������
//	sheet.AttachDispatch(book.get_ActiveSheet());          //////��ȡ��ǰ��Ծ��sheet
//	usedrange.AttachDispatch(sheet.get_UsedRange());
//	//��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
//	range.AttachDispatch(sheet.get_UsedRange());
//	//ȡ��Excel�Ѿ�ʹ�õ�����
//	range.AttachDispatch(range.get_Rows());
//	long  rowno1 = range.get_Row();               //��ȡʹ�������һ���к�
//	long usedrownum = range.get_Count();         //ʹ������
//	long   beginrow = rowno1 + usedrownum - 1;        //��ʼ������ʼ��
//	//ȡ��Excel�Ѿ�ʹ�õ�����
//	range.AttachDispatch(range.get_Columns());
//	long usedcolnum = range.get_Count();
//	//���������
//	CheckSize   sizelink1;
//	CheckSize   sizelink2;
//	int              row = rowno;                 //��¼��ʼ�к�
//	int              col = colno;                    //��¼��ʼ�к�
//	CString        str,str1[4];
//	try
//	{
//		/*int i = G::SizelistToFile1.GetCount();
//		int j = G::SizelistToFile2.GetCount();*/
//		if (G::SizelistToFile1.GetCount() < counts || G::SizelistToFile2.GetCount() < counts)
//			return -1;
//		//��ȡ��ǰ��ȡ��EXCEL�л��sheet
//		usedrange.AttachDispatch(sheet.get_Cells());
//
//		////�ڵ�һ��д�����
//		//str1[0] = L"���Ʒ�ܳ�";
//		//str1[1] = L"���Ʒ�⾶";
//		//str1[2] = L"�Ҳ�Ʒ�ܳ�";
//		//str1[3] = L"�Ҳ�Ʒ�⾶";
//		//for (int j = 0; j < 4; j++)
//		//{
//		//	range.AttachDispatch(usedrange.get_Item(_variant_t((long)1),
//		//		_variant_t((long)j+1)).pdispVal);
//		//	range.put_Value2(_variant_t(str[j]));
//		//	Sleep(30);
//		//}
//		//
//		///////////////////////////////////////////////////��������
//		for (int i = 0; i < counts; i++)
//		{
//			sizelink1 = G::SizelistToFile1.GetHead();
//			sizelink2 = G::SizelistToFile2.GetHead();
//			//���Ʒ�ܳ����⾶���Ҳ�Ʒ�ܳ����⾶
//			range.AttachDispatch(usedrange.get_Item(_variant_t((long)rowno + beginrow),
//				_variant_t((long)colno)).pdispVal);
//			//data = sizelink1.TotalLength[0].D();
//			str.Format(L"%.3f", sizelink1.TotalLength);
//			range.put_Value2(_variant_t(str));
//			colno++;                                     //��һ�У����Ʒ�⾶
//			range.AttachDispatch(usedrange.get_Item(_variant_t((long)rowno + beginrow),
//				_variant_t((long)colno)).pdispVal);
//			str.Format(L"%.3f", sizelink1.Diameter);
//			range.put_Value2(_variant_t(str));
//			colno++;                                  //��һ�У��Ҳ�Ʒ�ܳ�
//			range.AttachDispatch(usedrange.get_Item(_variant_t((long)rowno + beginrow),
//				_variant_t((long)colno)).pdispVal);
//			//sizelink2.TotalLength = 44;
//			str.Format(L"%.3f", sizelink2.TotalLength);
//			range.put_Value2(_variant_t(str));
//			colno++;                               //��һ�У��Ҳ�Ʒ�⾶
//			range.AttachDispatch(usedrange.get_Item(_variant_t((long)rowno + beginrow), 
//				_variant_t((long)colno)).pdispVal);
//			str.Format(L"%.3f", sizelink2.Diameter);
//			range.put_Value2(_variant_t(str));
//			//ɾ��ͷ�ڵ�
//			G::SizelistToFile1.RemoveHead();
//			G::SizelistToFile2.RemoveHead();
//
//			//���»�ȡ����
//			rowno++;
//			colno = col;
//
//			Sleep(300);
//		}
//		/////////////////////////////////////////�����޸��ļ�ʱ������
//		//�ڸ���ʱ�Ƿ���ʾ������ΪFALSE
//		//app.put_AlertBeforeOverwriting(FALSE);
//		//��ʾ���档����ΪFALSE
//		//app.put_DisplayAlerts(FALSE);
//		//���߸�����Ϊ����ģʽ��1��Ĭ��ֵ�������ķ���ģʽ��,2�������б�,3����ռģʽ��
//		book.SaveAs(COleVariant(filepath), covOptional, covOptional, covOptional,
//			covOptional, covOptional, 2, covOptional, covOptional, covOptional,
//			covOptional, covOptional);
//		//AfxMessageBox(L"����ɹ�");
//		release();
//	}
//	catch (CException *e)
//	{
//		release();
//		AfxMessageBox(L"����ʧ��");
//		return -1;
//	}
//
//	return 0;
//}

////��1269������������  2018.12.7
//��Excel�ļ��в���һ������
//int  CFileRW::InsertOneItem1269(CString filepath)
//{
//	COleVariant
//		covTrue((short)TRUE),
//		covFalse((short)FALSE),
//		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
//	sheet.AttachDispatch(book.get_ActiveSheet());          //////��ȡ��ǰ��Ծ��sheet
//	usedrange.AttachDispatch(sheet.get_UsedRange());
//	//��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
//	range.AttachDispatch(sheet.get_UsedRange());
//	//ȡ��Excel�Ѿ�ʹ�õ�����
//	range.AttachDispatch(range.get_Rows());
//	long  rowno1 = range.get_Row();               //��ȡʹ�������һ���к�
//	long usedrownum = range.get_Count();         //ʹ������
//	long   beginrow = rowno1 + usedrownum;        //��ʼ������ʼ��
//	//ȡ��Excel�Ѿ�ʹ�õ�����
//	range.AttachDispatch(range.get_Columns());
//	long usedcolnum = range.get_Count();
//
//	int   colno = 1;
//	//���������
//	CheckSize   sizelink1;
//	CheckSize   sizelink2;
//	CString        str;
//	try
//	{
//		if (G::SizelistToFile1.GetCount() < 1 || G::SizelistToFile2.GetCount() < 1)
//			return -1;
//		//��ȡ��ǰ��ȡ��EXCEL�л��sheet
//		usedrange.AttachDispatch(sheet.get_Cells());
//
//		///////////////////////////////////////////////////��������
//		sizelink1 = G::SizelistToFile1.GetHead();
//		sizelink2 = G::SizelistToFile2.GetHead();
//		
//		Sleep(20);
//		//���Ʒ�ܳ����⾶���Ҳ�Ʒ�ܳ����⾶
//		range.AttachDispatch(usedrange.get_Item(_variant_t((long)beginrow),
//			_variant_t((long)colno)).pdispVal);
//		//data = sizelink1.TotalLength[0].D();
//		str.Format(L"%.3f", sizelink1.TotalLength);
//		range.put_Value2(_variant_t(str));
//		colno++;                                     //��һ�У����Ʒ�⾶
//		range.AttachDispatch(usedrange.get_Item(_variant_t((long)beginrow),
//			_variant_t((long)colno)).pdispVal);
//		str.Format(L"%.3f", sizelink1.Diameter);
//		range.put_Value2(_variant_t(str));
//		colno++;                                  //��һ�У��Ҳ�Ʒ�ܳ�
//		Sleep(20);
//
//		range.AttachDispatch(usedrange.get_Item(_variant_t((long)beginrow),
//			_variant_t((long)colno)).pdispVal);
//		//sizelink2.TotalLength = 44;
//		str.Format(L"%.3f", sizelink2.TotalLength);
//		range.put_Value2(_variant_t(str));
//		colno++;                               //��һ�У��Ҳ�Ʒ�⾶
//		range.AttachDispatch(usedrange.get_Item(_variant_t((long)beginrow),
//			_variant_t((long)colno)).pdispVal);
//		str.Format(L"%.3f", sizelink2.Diameter);
//		range.put_Value2(_variant_t(str));
//		//ɾ��ͷ�ڵ�
//		G::SizelistToFile1.RemoveHead();
//		G::SizelistToFile2.RemoveHead();
//		
//		Sleep(20);
//		/////////////////////////////////////////�����޸��ļ�ʱ������
//		//�ڸ���ʱ�Ƿ���ʾ������ΪFALSE
//		//app.put_AlertBeforeOverwriting(FALSE);
//		//��ʾ���档����ΪFALSE
//		//app.put_DisplayAlerts(FALSE);
//		//���߸�����Ϊ����ģʽ��1��Ĭ��ֵ�������ķ���ģʽ��,2�������б�,3����ռģʽ��
//		book.SaveAs(COleVariant(filepath), covOptional, covOptional, covOptional,
//			covOptional, covOptional, 2, covOptional, covOptional, covOptional,
//			covOptional, covOptional);
//	}
//	catch (CException *e)
//	{
//		return -1;
//	}
//
//	return 0;
//}


// ��Excel���򲢽��г�ʼ��
int CFileRW::OpenExcel(CString filepath)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//����Excel ������(����Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("����Excel������ʧ��!"));
		return -1;
	}

	////�ڸ���ʱ�Ƿ���ʾ������ΪFALSE
	app.put_AlertBeforeOverwriting(FALSE);
	//��ʾ���档����ΪFALSE
	app.put_DisplayAlerts(FALSE);

	books.AttachDispatch(app.get_Workbooks());
	/*��һ�����������粻���ڣ�������һ��������*/
	try
	{
		/*��һ��������*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		/*����һ���µĹ�����*/
		lpdisp = books.Add(covOptional);
		book.AttachDispatch(lpdisp);
	}
	/*�õ��������е�Sheet������*/
	sheets.AttachDispatch(book.get_Sheets());

	return 0;
}
