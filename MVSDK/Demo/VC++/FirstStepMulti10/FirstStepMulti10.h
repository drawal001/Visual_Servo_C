
// FirstStepMulti10.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFirstStepMulti10App:
// �йش����ʵ�֣������ FirstStepMulti10.cpp
//

class CFirstStepMulti10App : public CWinApp
{
public:
	CFirstStepMulti10App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFirstStepMulti10App theApp;