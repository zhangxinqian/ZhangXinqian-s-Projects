
// CryptographyAlgorithm.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCryptographyAlgorithmApp:
// �йش����ʵ�֣������ CryptographyAlgorithm.cpp
//

class CCryptographyAlgorithmApp : public CWinApp
{
public:
	CCryptographyAlgorithmApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCryptographyAlgorithmApp theApp;