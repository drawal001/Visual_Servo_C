
// DrawText.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDrawTextApp:
// �йش����ʵ�֣������ DrawText.cpp
//

class CDrawTextApp : public CWinApp
{
public:
	CDrawTextApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDrawTextApp theApp;