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
	//列表控件写入Excel，将原内容清除
	//参数：控件变量，存储路径，表头数组，控件行数，控件列数
	void ListToExcel(CListCtrl *datalist, CString filepath, CString str[], int rowcount, int colcount);
	
	//Excel读取到list控件
	//参数：控件变量
	void ExcelToList(CListCtrl *datalist);

	//list控件内容插入Excel，原文件必须存在
	//参数：控件变量，插入文件路径，控件行数，控件列数
	void ListInsertExcel(CListCtrl *datalist, CString filepath, int rowcount, int colcount);

	//字符串写入text
	//参数：文件路径，写入字符串
	void WriteToTxt(CString filepath,CString insertstr);

	//改变Excel字体，颜色，字体大小
	//文件路径，起始行，起始列，结束行，结束列,字体，字体颜色索引，字体大小
	void ExcelChangeFont(CString filepath, long startrowfont, long startcolfont,
		long endrowfont,long endcolfont,CString fonts,long colorindex,long fontsize);

	//设置Excel单元格样式
	//文件路径，起始行，起始列，结束行，结束列，边框线样式，边框线宽度，边框线颜色，单元格列宽自动适应，单元格内容自动对齐
	void ExcelBoarderSet(CString filepath,long startrowboard, long startcolboard, 
		long endrowboard,long endcolboard,long linestyle,long linewidth,long color);

	//释放资源
	void release();

	//不可用
	// 将链表内容的N组数据插入Excel（插Excel中的单列，链表中的数据属性无法得知）
	//插入组数，链表，插入数据，插入文件路径，插入行号，插入列号（当前文档已使用最后一行的下一行为第一行）
	int InsertBatchData(int counts, CPtrList *list, VARIANT data, CString filepath, int rowno, int colno);
	
	//仅1269空气柱检测可用  2018.12.4
	// 将检测得到的尺寸数据链表内容写入Excel
	//插入行数，文件路径，起始行，起始列
	int ResultLinkedToExcel(int counts, CString filepath, int rowno, int colno);

	////仅1269空气柱检测可用  2018.12.7
	//向Excel文件中插入一行数据
	int  InsertOneItem1269(CString filepath);

	// 打开Excel程序并进行初始化,同时打开或新建一个文件
	int OpenExcel(CString filepath);
};

