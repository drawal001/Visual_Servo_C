
// Denoise3D.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDenoise3DApp:
// �йش����ʵ�֣������ Denoise3D.cpp
//

class CDenoise3DApp : public CWinApp
{
public:
	CDenoise3DApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDenoise3DApp theApp;