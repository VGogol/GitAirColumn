#include "stdafx.h"
#include "FileRW.h"

//备注
//////复杂文件路径
//时间相关
//CString  excelpath;                       //写入文件路径名
//CString  filepath1 = L"D\\空气柱检测数据\\尺寸数据";
//CTime  time = CTime::GetCurrentTime();
//CString strRptTime = time.Format("%Y%m%d");
//excelpath = filepath1 + strRptTime + L".xlsx";


CFileRW::CFileRW()
{
}


CFileRW::~CFileRW()
{
}

//列表控件写入EXCEL,清空原表格
//参数：控件变量，存储路径，表头数组，控件行数，控件列数
void CFileRW::ListToExcel(CListCtrl *datalist, CString filepath, CString str[], int rowcount, int colcount)   //存储文件路径，控件行数，控件列数
{
	//求出表头数组长度
	int  length = sizeof(str) / sizeof(str[0]);
	//list控件数据写入Excel
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//创建Excel 服务器(启动Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("启动Excel服务器失败!"));
		return;
	}
	////在覆盖时是否提示。设置为FALSE
	//app.put_AlertBeforeOverwriting(FALSE);
	//显示警告。设置为FALSE
	//app.put_DisplayAlerts(FALSE);
	//// 设置为可见
	//m_app.put_Visible(FALSE);
	//设置复杂文件路径
	/*CString strRptTime = time.Format("%Y%m%d%H%M");
	CString strBookPath = ("D:\\检测数据\\check data" + strRptTime + ".xlsx");*/
	//时间相关的文件操作
	//CFile file(L"D:/data insert/checkdevice.xls", CFile::modeCreate|CFile::modeWrite);
	//CString fileName = "d:\\" + strRptTime + ".xls";
	/*CTime time = CTime::GetCurrentTime();
	CString strRptTime = time.Format("%Y%m%d%H%M%S");*/
	//文件路径
	//CString strBookPath = _T("D:\\检测数据.xlsx");
	/*得到工作簿容器*/
	books.AttachDispatch(app.get_Workbooks());	
	/*打开一个工作簿，如不存在，则新增一个工作簿*/
	try
	{
		/*打开一个工作簿*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		/*增加一个新的工作簿*/
		lpdisp = books.Add(covOptional);
		book.AttachDispatch(lpdisp);
	}
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());
	sheet = sheets.get_Item(COleVariant((short)1));   //得到第一个工作表
	CHeaderCtrl   *pmyHeaderCtrl = datalist->GetHeaderCtrl(); //获取表头
	usedrange.AttachDispatch(sheet.get_UsedRange());
	///*打开一个Sheet，如不存在，就新增一个Sheet*/
	//CString strSheetName = _T("listdata");
	//try
	//{
	//	/*打开一个已有的Sheet*/
	//	m_lpdisp = m_sheets.get_Item(_variant_t(strSheetName));
	//	m_sheet.AttachDispatch(m_lpdisp);
	//}
	//catch (...)
	//{
	//	/*创建一个新的Sheet*/
	//	m_lpdisp = m_sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
	//	m_sheet.AttachDispatch(m_lpdisp);
	//	m_sheet.put_Name(strSheetName);
	//}
	/*m_range = m_sheet.get_Range(_variant_t(_T("A")), _variant_t(_T("A")));
	m_range.put_Value2(_variant_t("边缘直线"));
	m_range = m_sheet.get_Range(_variant_t("B"), _variant_t("B"));
	m_range.put_Value2(_variant_t("左圆心"));
	m_range = m_sheet.get_Range(_variant_t("C"), _variant_t("C"));
	m_range.put_Value2(_variant_t("右圆心"));*/
	try
	{
		//获取当前读取的EXCEL中活动的sheet
		usedrange.AttachDispatch(sheet.get_Cells());
		//清空Excel中的内容
		usedrange.ClearContents();
		//自动适应
		range = sheet.get_Columns();
		range.AutoFit();
		range = sheet.get_Rows();
		range.AutoFit();
		//填入列名
		for (int i = 1; i <= colcount; i++)
		{
			range.AttachDispatch(usedrange.get_Item((_variant_t((long)1)),
				(_variant_t)(long)i).pdispVal);
			range.put_Value2((_variant_t (str[i-1])));
		}
		
		/*range.AttachDispatch(usedrange.get_Item((_variant_t((long)1)),
			(_variant_t)(long)2).pdispVal);
		range.put_Value2((_variant_t(L"左圆心")));
		range.AttachDispatch(usedrange.get_Item((_variant_t((long)1)),
			(_variant_t)(long)3).pdispVal);
		range.put_Value2((_variant_t(L"右圆心")));*/
		//填充新内容
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
		//第七个参数为访问模式，1（默认值，不更改访问模式）,2（共享列表）,3（独占模式）
		book.SaveAs(COleVariant(filepath), covOptional, covOptional, covOptional,
			covOptional, covOptional, 1, covOptional, covOptional, covOptional,
			covOptional, covOptional);
		release();
		AfxMessageBox(L"保存成功");
		//m_used_range.AttachDispatch(m_used_range.get_Item(COleVariant((long)1), (COleVariant)(long)1).pdispVal);
	}
	catch (CException *e)
	{
		release();
		AfxMessageBox(L"保存失败");
	}
}

//EXCEL数据读取到列表控件
void CFileRW::ExcelToList(CListCtrl *datalist)
{
	CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"excel文件(*.xls)|*.xlsx|ALL Files(*.*)||");         //TRUE为打开，FALS为保存
	if (IDOK != filedlg.DoModal())
	{
		return;
	}
	//获取文件路径
	readxlspath = filedlg.GetPathName();
	if (!PathFileExists(readxlspath))
	{
		AfxMessageBox(readxlspath + "不存在");
		return;
	}
	if (!app.CreateDispatch(L"Excel.Application"))    //创建接口对象
	{
		AfxMessageBox(L"无法启动Excel服务器");
		return;
	}
	COleVariant	covOption((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//得到工作簿
	books.AttachDispatch(app.get_Workbooks());
	lpdisp = books.Open(readxlspath, covOption, covOption, covOption, covOption,
		covOption, covOption, covOption, covOption, covOption, covOption,
		covOption, covOption, covOption, covOption);
	//得到Workbook
	book.AttachDispatch(lpdisp);
	//sheets
	sheets.AttachDispatch(book.get_Worksheets());
	////得到WorkSheet
	//sheet.AttachDispatch(sheets.get_sheet());
	//得到当前活跃sheet
	//如果有单元格正处于编辑状态，此操作不会返回，会一直等待
	lpdisp = book.get_ActiveSheet();
	sheet.AttachDispatch(lpdisp);
	usedrange.AttachDispatch(sheet.get_UsedRange());
	//读取第一个单元格的值
	//range.AttachDispatch(sheet.get_UsedRange());
	//m_range.AttachDispatch(m_range.get_Item(COleVariant((long)1), COleVariant((long)1)).pdispVal);
	COleVariant vResult,vResult1;
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	range.AttachDispatch(sheet.get_UsedRange());
	//取得已经使用的行数
	range.AttachDispatch(range.get_Rows());
	long rownum = range.get_Count();
	//取得已经使用的列数
	range.AttachDispatch(range.get_Columns());
	long colnum = range.get_Count();
	//取得已使用区域的起始行，从1开始
	long startrow = range.get_Row();
	//取得已使用区域的起始列，从1开始
	long startcol = range.get_Column();
	////建立列头
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
		//先插入行首，即首列元素
		range.AttachDispatch(sheet.get_Cells());
		range.AttachDispatch(range.get_Item(_variant_t((long)i+1),
			_variant_t((long)startcol)).pdispVal);
		vResult = range.get_Value2();
		CString strread, stry0, strm0, strd0;
		SYSTEMTIME st0;
		if (vResult.vt == VT_BSTR)     //若是字符串
		{
			strread = vResult.bstrVal;
		}
		else if (vResult.vt == VT_R8) //8字节的数字
		{
			strread.Format(L"%f", vResult.dblVal);
		}
		else if (vResult.vt == VT_DATE) //时间格式
		{
			VariantTimeToSystemTime(vResult.date, &st0);
			stry0.Format(L"%d", st0.wYear);
			strm0.Format(L"%d", st0.wMonth);
			strd0.Format(L"%d", st0.wDay);
			strread = stry0 + L"-" + strm0 + L"-" + strd0;
		}
		else if (vResult.vt == VT_EMPTY) //单元为空
		{
			strread = L"";
		}
		else if (vResult.vt == VT_I4)
		{
			strread.Format(_T("%ld"), (int)vResult.lVal);
		};
		datalist->InsertItem(i , strread);

      //插入后面元素
		for (int j = startcol+1; j <= colnum; j++)
		{
			//读取单元格的值
			range.AttachDispatch(sheet.get_Cells());
			range.AttachDispatch(range.get_Item(_variant_t((long)i + 1),
				_variant_t((long)j)).pdispVal);
			vResult1 = range.get_Value2();
			CString str, stry, strm, strd;
			SYSTEMTIME st;
			if (vResult1.vt == VT_BSTR)     //若是字符串
			{
				str = vResult1.bstrVal;
			}
			else if (vResult1.vt == VT_R8) //8字节的数字
			{
				str.Format(L"%f", vResult1.dblVal);
			}
			else if (vResult1.vt == VT_DATE) //时间格式
			{
				VariantTimeToSystemTime(vResult1.date, &st);
				stry.Format(L"%d", st.wYear);
				strm.Format(L"%d", st.wMonth);
				strd.Format(L"%d", st.wDay);
				str = stry + L"-" + strm + L"-" + strd;
			}
			else if (vResult1.vt == VT_EMPTY) //单元为空
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
	AfxMessageBox(L"读取成功");
}

//list控件内容插入Excel，原文件必须存在
//控件变量，插入文件路径，控件行数，控件列数
void CFileRW::ListInsertExcel(CListCtrl *datalist, CString filepath, int rowcount, int colcount)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//创建Excel 服务器(启动Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("启动Excel服务器失败!"));
		return;
	}
	/*得到工作簿容器*/
	books.AttachDispatch(app.get_Workbooks());
	/*打开一个工作簿，如不存在，则报错簿*/
	try
	{
		/*打开一个工作簿*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		//打开失败或者文件不存在则报错
		AfxMessageBox(filepath+"文件打开失败");
		return;
	}
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());
	sheet = sheets.get_Item(COleVariant((short)1));   //得到第一个工作表
	CHeaderCtrl   *pmyHeaderCtrl = datalist->GetHeaderCtrl(); //获取表头
	usedrange.AttachDispatch(sheet.get_UsedRange());
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	range.AttachDispatch(sheet.get_UsedRange());
	//取得Excel已经使用的行数
	range.AttachDispatch(range.get_Rows());
	long usedrownum = range.get_Count();
	//取得Excel已经使用的列数
	range.AttachDispatch(range.get_Columns());
	long usedcolnum = range.get_Count();
	//填充新内容
	try
	{
		//获取当前读取的EXCEL中活动的sheet
		usedrange.AttachDispatch(sheet.get_Cells());
		CString	 str1;
		for (int row = 0; row < rowcount ; row++)
		{
			for (int col = 0; col < colcount; col++)
			{
				str1 = datalist->GetItemText(row, col);
				//获取在Excel中的插入位置
				range.AttachDispatch(usedrange.get_Item(_variant_t((long)row + 1 + usedrownum),
					_variant_t((long)col + 1)).pdispVal);
				range.put_Value2(_variant_t(str1));
				//// 然后设置外边框  
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
		//在覆盖时是否提示。设置为FALSE
		app.put_AlertBeforeOverwriting(FALSE);
		//显示警告。设置为FALSE
		app.put_DisplayAlerts(FALSE);
		//第七个参数为访问模式，1（默认值，不更改访问模式）,2（共享列表）,3（独占模式）
		book.SaveAs(COleVariant(filepath), covOptional, covOptional, covOptional,
			covOptional, covOptional, 1, covOptional, covOptional, covOptional,
			covOptional, covOptional);
		release();
		//AfxMessageBox(L"保存成功");
		//m_used_range.AttachDispatch(m_used_range.get_Item(COleVariant((long)1), (COleVariant)(long)1).pdispVal);
	}
	catch (CException *e)
	{
		release();
		AfxMessageBox(L"保存失败");
	}
}

//字符串写入text
void	CFileRW::WriteToTxt(CString filepath, CString insertstr)
{
	CStdioFile file;
	CString strline;
	BOOL flag,flag1;
	/*char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");*/
	//打开text文件
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
		AfxMessageBox(L"文件打开失败！");
	}
	file.SeekToEnd();
	strline = L"\n" + insertstr;
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//设定
	file.WriteString(strline);
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//还原区域设定
	AfxMessageBox(L"成功添加至文档末尾");
	file.Close();
}

//改变Excel单元格颜色,字体，字体大小
//文件路径，起始行，起始列，结束行，结束列，字体，字体颜色索引，字体大小
void CFileRW::ExcelChangeFont(CString filepath, long startrowfont, long startcolfont,
	long endrowfont, long endcolfont, CString fonts, long colorindex, long fontsize)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//创建Excel 服务器(启动Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("启动Excel服务器失败!"));
		return;
	}
	/*得到工作簿容器*/
	books.AttachDispatch(app.get_Workbooks());
	/*打开一个工作簿，如不存在，则新增一个工作簿*/
	try
	{
		/*打开一个工作簿*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		//打开失败或者文件不存在则报错
		AfxMessageBox(filepath + "文件打开失败");
		return;
	}
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());
	sheet = sheets.get_Item(COleVariant((short)1));   //得到第一个工作表
	usedrange.AttachDispatch(sheet.get_UsedRange());
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	range.AttachDispatch(sheet.get_UsedRange());
	font.AttachDispatch(range.get_Font());
	for (long i = startrowfont; i <= endrowfont; i++)
	{
		for (long j = startcolfont; j <= endcolfont; j++)
		{
			range.AttachDispatch(usedrange.get_Item(_variant_t((long)i),
				_variant_t((long)j)).pdispVal);
			font.put_Name(_variant_t("华文楷体"));//字体 
			font.put_ColorIndex(_variant_t((long)3));//字的颜色 
			font.put_Size(_variant_t((long)20));//字号 
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


//设置Excel单元格样式
//文件路径，起始行，起始列，结束行，结束列，边框线样式，边框线宽，边框线颜色，单元格列宽自动适应，居中对齐
void CFileRW::ExcelBoarderSet(CString filepath,long startrowboard, long startcolboard,
	long endrowboard, long endcolboard, long linestyle, long linewidth, long color)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//创建Excel 服务器(启动Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("启动Excel服务器失败!"));
		return;
	}
	/*得到工作簿容器*/
	books.AttachDispatch(app.get_Workbooks());
	/*打开一个工作簿，如不存在，则新增一个工作簿*/
	try
	{
		/*打开一个工作簿*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		//打开失败或者文件不存在则报错
		AfxMessageBox(filepath + "文件打开失败");
		return;
	}
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());
	sheet = sheets.get_Item(COleVariant((short)1));   //得到第一个工作表
	usedrange.AttachDispatch(sheet.get_UsedRange());
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	range.AttachDispatch(sheet.get_UsedRange());
	range.put_HorizontalAlignment(COleVariant((long)-4108));
	range.put_VerticalAlignment(COleVariant((long)-4108));    // -4108:居中，-4160:靠上，-4107:靠下 
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
			// LineStyle=线型(1~13) Weight=线宽 ColorIndex=线的颜色(-4105为自动, 1为黑色)   
			long lColor = RGB(255, 0, 0);
			COleVariant varColor(lColor);
			range.BorderAround(_variant_t(long(linestyle)), linewidth, color, 
				vtMissing, varColor);//设置边框 
			//range.SetColumnWidth(_variant_t((long)12));
		}
	}
	release();
}

//释放资源
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

//不可用
// 将链表内容的N组数据插入Excel（插Excel中的单列，链表中的数据属性无法得知）
//插入组数，链表，插入数据，插入文件路径，插入行号，插入列号（当前文档已使用最后一行的下一行为第一行）
int CFileRW::InsertBatchData(int counts, CPtrList *list, VARIANT data, CString filepath, int rowno, int colno)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//创建Excel 服务器(启动Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("启动Excel服务器失败!"));
		return -1;
	}
	books.AttachDispatch(app.get_Workbooks());
	/*打开一个工作簿，如不存在，则新增一个工作簿*/
	try
	{
		/*打开一个工作簿*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		/*增加一个新的工作簿*/
		lpdisp = books.Add(covOptional);
		book.AttachDispatch(lpdisp);
	}
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Worksheets());
	sheet.AttachDispatch(book.get_ActiveSheet());          //////获取当前活跃的sheet
	//sheets.AttachDispatch(book.get_Sheets());
	//sheet = sheets.get_Item(COleVariant((short)1));   //得到第一个工作表

	usedrange.AttachDispatch(sheet.get_UsedRange());
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	range.AttachDispatch(sheet.get_UsedRange());
	//取得Excel已经使用的行数
	range.AttachDispatch(range.get_Rows());
	long usedrownum = range.get_Count();
	//取得Excel已经使用的列数
	range.AttachDispatch(range.get_Columns());
	long usedcolnum = range.get_Count();
	//填充新内容
	try
	{
		//获取当前读取的EXCEL中活动的sheet
		usedrange.AttachDispatch(sheet.get_Cells());
		//获取在Excel中的插入位置
		range.AttachDispatch(usedrange.get_Item(_variant_t((long)rowno  + usedrownum),
			_variant_t((long)colno )).pdispVal);
		range.put_Value2(data);
				//// 然后设置外边框  
				//if (TRUE==enableboarderset)
				//{
				//	ExcelBoarderSet();
				//}
				//else
				//{
				//	enableboarderset = 0;
				//}
		/////////////////////////////////////////屏蔽修改文件时弹出的
		//在覆盖时是否提示。设置为FALSE
		app.put_AlertBeforeOverwriting(FALSE); 
		//显示警告。设置为FALSE
		app.put_DisplayAlerts(FALSE);
		//第七个参数为访问模式，1（默认值，不更改访问模式）,2（共享列表）,3（独占模式）
		book.SaveAs(COleVariant(filepath), covOptional, covOptional, covOptional,
			covOptional, covOptional, 3, covOptional, covOptional, covOptional,
			covOptional, covOptional);
		//release();
	}
	catch (CException *e)
	{
		release();
		AfxMessageBox(L"保存失败");
		return -1;
	}

	return 0;
}

////仅1269空气柱检测可用
//// 将检测得到的尺寸数据链表内容写入Excel
//int CFileRW::ResultLinkedToExcel(int counts, CString filepath, int rowno, int colno)
//{
//	COleVariant
//		covTrue((short)TRUE),
//		covFalse((short)FALSE),
//		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
//	//创建Excel 服务器(启动Excel)
//	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
//	{
//		AfxMessageBox(_T("启动Excel服务器失败!"));
//		return -1;
//	}
//	////在覆盖时是否提示。设置为FALSE
//	app.put_AlertBeforeOverwriting(FALSE);
//	//显示警告。设置为FALSE
//	app.put_DisplayAlerts(FALSE);
//
//	books.AttachDispatch(app.get_Workbooks());
//	/*打开一个工作簿，如不存在，则新增一个工作簿*/
//	try
//	{
//		/*打开一个工作簿*/
//		lpdisp = books.Open(filepath,
//			covOptional, covOptional, covOptional, covOptional, covOptional,
//			covOptional, covOptional, covOptional, covOptional, covOptional,
//			covOptional, covOptional, covOptional, covOptional);
//		book.AttachDispatch(lpdisp);
//	}
//	catch (...)
//	{
//		/*增加一个新的工作簿*/
//		lpdisp = books.Add(covOptional);
//		book.AttachDispatch(lpdisp);
//	}
//	/*得到工作簿中的Sheet的容器*/
//	sheets.AttachDispatch(book.get_Sheets());
//	//sheet = sheets.get_Item(COleVariant((short)1));   //得到第一个工作表
//	sheet.AttachDispatch(book.get_ActiveSheet());          //////获取当前活跃的sheet
//	usedrange.AttachDispatch(sheet.get_UsedRange());
//	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
//	range.AttachDispatch(sheet.get_UsedRange());
//	//取得Excel已经使用的行数
//	range.AttachDispatch(range.get_Rows());
//	long  rowno1 = range.get_Row();               //获取使用区域第一行行号
//	long usedrownum = range.get_Count();         //使用行数
//	long   beginrow = rowno1 + usedrownum - 1;        //开始插入起始行
//	//取得Excel已经使用的列数
//	range.AttachDispatch(range.get_Columns());
//	long usedcolnum = range.get_Count();
//	//填充新内容
//	CheckSize   sizelink1;
//	CheckSize   sizelink2;
//	int              row = rowno;                 //记录初始行号
//	int              col = colno;                    //记录初始列号
//	CString        str,str1[4];
//	try
//	{
//		/*int i = G::SizelistToFile1.GetCount();
//		int j = G::SizelistToFile2.GetCount();*/
//		if (G::SizelistToFile1.GetCount() < counts || G::SizelistToFile2.GetCount() < counts)
//			return -1;
//		//获取当前读取的EXCEL中活动的sheet
//		usedrange.AttachDispatch(sheet.get_Cells());
//
//		////在第一行写入标题
//		//str1[0] = L"左产品总长";
//		//str1[1] = L"左产品外径";
//		//str1[2] = L"右产品总长";
//		//str1[3] = L"右产品外径";
//		//for (int j = 0; j < 4; j++)
//		//{
//		//	range.AttachDispatch(usedrange.get_Item(_variant_t((long)1),
//		//		_variant_t((long)j+1)).pdispVal);
//		//	range.put_Value2(_variant_t(str[j]));
//		//	Sleep(30);
//		//}
//		//
//		///////////////////////////////////////////////////插入检测结果
//		for (int i = 0; i < counts; i++)
//		{
//			sizelink1 = G::SizelistToFile1.GetHead();
//			sizelink2 = G::SizelistToFile2.GetHead();
//			//左产品总长、外径；右产品总长，外径
//			range.AttachDispatch(usedrange.get_Item(_variant_t((long)rowno + beginrow),
//				_variant_t((long)colno)).pdispVal);
//			//data = sizelink1.TotalLength[0].D();
//			str.Format(L"%.3f", sizelink1.TotalLength);
//			range.put_Value2(_variant_t(str));
//			colno++;                                     //下一列，左产品外径
//			range.AttachDispatch(usedrange.get_Item(_variant_t((long)rowno + beginrow),
//				_variant_t((long)colno)).pdispVal);
//			str.Format(L"%.3f", sizelink1.Diameter);
//			range.put_Value2(_variant_t(str));
//			colno++;                                  //下一列，右产品总长
//			range.AttachDispatch(usedrange.get_Item(_variant_t((long)rowno + beginrow),
//				_variant_t((long)colno)).pdispVal);
//			//sizelink2.TotalLength = 44;
//			str.Format(L"%.3f", sizelink2.TotalLength);
//			range.put_Value2(_variant_t(str));
//			colno++;                               //下一列，右产品外径
//			range.AttachDispatch(usedrange.get_Item(_variant_t((long)rowno + beginrow), 
//				_variant_t((long)colno)).pdispVal);
//			str.Format(L"%.3f", sizelink2.Diameter);
//			range.put_Value2(_variant_t(str));
//			//删除头节点
//			G::SizelistToFile1.RemoveHead();
//			G::SizelistToFile2.RemoveHead();
//
//			//重新获取行列
//			rowno++;
//			colno = col;
//
//			Sleep(300);
//		}
//		/////////////////////////////////////////屏蔽修改文件时弹出的
//		//在覆盖时是否提示。设置为FALSE
//		//app.put_AlertBeforeOverwriting(FALSE);
//		//显示警告。设置为FALSE
//		//app.put_DisplayAlerts(FALSE);
//		//第七个参数为访问模式，1（默认值，不更改访问模式）,2（共享列表）,3（独占模式）
//		book.SaveAs(COleVariant(filepath), covOptional, covOptional, covOptional,
//			covOptional, covOptional, 2, covOptional, covOptional, covOptional,
//			covOptional, covOptional);
//		//AfxMessageBox(L"保存成功");
//		release();
//	}
//	catch (CException *e)
//	{
//		release();
//		AfxMessageBox(L"保存失败");
//		return -1;
//	}
//
//	return 0;
//}

////仅1269空气柱检测可用  2018.12.7
//向Excel文件中插入一行数据
//int  CFileRW::InsertOneItem1269(CString filepath)
//{
//	COleVariant
//		covTrue((short)TRUE),
//		covFalse((short)FALSE),
//		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
//	sheet.AttachDispatch(book.get_ActiveSheet());          //////获取当前活跃的sheet
//	usedrange.AttachDispatch(sheet.get_UsedRange());
//	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
//	range.AttachDispatch(sheet.get_UsedRange());
//	//取得Excel已经使用的行数
//	range.AttachDispatch(range.get_Rows());
//	long  rowno1 = range.get_Row();               //获取使用区域第一行行号
//	long usedrownum = range.get_Count();         //使用行数
//	long   beginrow = rowno1 + usedrownum;        //开始插入起始行
//	//取得Excel已经使用的列数
//	range.AttachDispatch(range.get_Columns());
//	long usedcolnum = range.get_Count();
//
//	int   colno = 1;
//	//填充新内容
//	CheckSize   sizelink1;
//	CheckSize   sizelink2;
//	CString        str;
//	try
//	{
//		if (G::SizelistToFile1.GetCount() < 1 || G::SizelistToFile2.GetCount() < 1)
//			return -1;
//		//获取当前读取的EXCEL中活动的sheet
//		usedrange.AttachDispatch(sheet.get_Cells());
//
//		///////////////////////////////////////////////////插入检测结果
//		sizelink1 = G::SizelistToFile1.GetHead();
//		sizelink2 = G::SizelistToFile2.GetHead();
//		
//		Sleep(20);
//		//左产品总长、外径；右产品总长，外径
//		range.AttachDispatch(usedrange.get_Item(_variant_t((long)beginrow),
//			_variant_t((long)colno)).pdispVal);
//		//data = sizelink1.TotalLength[0].D();
//		str.Format(L"%.3f", sizelink1.TotalLength);
//		range.put_Value2(_variant_t(str));
//		colno++;                                     //下一列，左产品外径
//		range.AttachDispatch(usedrange.get_Item(_variant_t((long)beginrow),
//			_variant_t((long)colno)).pdispVal);
//		str.Format(L"%.3f", sizelink1.Diameter);
//		range.put_Value2(_variant_t(str));
//		colno++;                                  //下一列，右产品总长
//		Sleep(20);
//
//		range.AttachDispatch(usedrange.get_Item(_variant_t((long)beginrow),
//			_variant_t((long)colno)).pdispVal);
//		//sizelink2.TotalLength = 44;
//		str.Format(L"%.3f", sizelink2.TotalLength);
//		range.put_Value2(_variant_t(str));
//		colno++;                               //下一列，右产品外径
//		range.AttachDispatch(usedrange.get_Item(_variant_t((long)beginrow),
//			_variant_t((long)colno)).pdispVal);
//		str.Format(L"%.3f", sizelink2.Diameter);
//		range.put_Value2(_variant_t(str));
//		//删除头节点
//		G::SizelistToFile1.RemoveHead();
//		G::SizelistToFile2.RemoveHead();
//		
//		Sleep(20);
//		/////////////////////////////////////////屏蔽修改文件时弹出的
//		//在覆盖时是否提示。设置为FALSE
//		//app.put_AlertBeforeOverwriting(FALSE);
//		//显示警告。设置为FALSE
//		//app.put_DisplayAlerts(FALSE);
//		//第七个参数为访问模式，1（默认值，不更改访问模式）,2（共享列表）,3（独占模式）
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


// 打开Excel程序并进行初始化
int CFileRW::OpenExcel(CString filepath)
{
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	//创建Excel 服务器(启动Excel)
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("启动Excel服务器失败!"));
		return -1;
	}

	////在覆盖时是否提示。设置为FALSE
	app.put_AlertBeforeOverwriting(FALSE);
	//显示警告。设置为FALSE
	app.put_DisplayAlerts(FALSE);

	books.AttachDispatch(app.get_Workbooks());
	/*打开一个工作簿，如不存在，则新增一个工作簿*/
	try
	{
		/*打开一个工作簿*/
		lpdisp = books.Open(filepath,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);
		book.AttachDispatch(lpdisp);
	}
	catch (...)
	{
		/*增加一个新的工作簿*/
		lpdisp = books.Add(covOptional);
		book.AttachDispatch(lpdisp);
	}
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());

	return 0;
}
