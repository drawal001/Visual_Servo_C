
// FirstStepMulti.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFirstStepMultiApp:
// �йش����ʵ�֣������ FirstStepMulti.cpp
//

class CFirstStepMultiApp : public CWinApp
{
public:
	CFirstStepMultiApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFirstStepMultiApp theApp;