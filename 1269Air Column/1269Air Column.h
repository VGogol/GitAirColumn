
// 1269Air Column.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy1269AirColumnApp: 
// �йش����ʵ�֣������ 1269Air Column.cpp
//

class CMy1269AirColumnApp : public CWinApp
{
public:
	CMy1269AirColumnApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy1269AirColumnApp theApp;