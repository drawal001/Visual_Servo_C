
// SaveImage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "CameraApi.h"
#include "CameraGrabber.h"
#include "CameraImage.h"

#ifdef _WIN64
#pragma comment(lib, "MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "MVCAMSDK.lib")
#endif



// CSaveImageApp:
// �йش����ʵ�֣������ SaveImage.cpp
//

class CSaveImageApp : public CWinApp
{
public:
	CSaveImageApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSaveImageApp theApp;