
// StockStrategyApp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStockStrategyApp:
// �йش����ʵ�֣������ StockStrategyApp.cpp
//

class CStockStrategyApp : public CWinApp
{
public:
	CStockStrategyApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStockStrategyApp theApp;