
// CameraDisplay.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCameraDisplayApp:
// �йش����ʵ�֣������ CameraDisplay.cpp
//

class CCameraDisplayApp : public CWinApp
{
public:
	CCameraDisplayApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCameraDisplayApp theApp;