
// ImageDefinition.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CImageDefinitionApp:
// �йش����ʵ�֣������ ImageDefinition.cpp
//

class CImageDefinitionApp : public CWinApp
{
public:
	CImageDefinitionApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CImageDefinitionApp theApp;