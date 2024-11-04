// GxImageProcess.h : main header file for the GXIMAGEPROCESS application
//

#if !defined(AFX_GXIMAGEPROCESS_H__2B09CF48_BA24_4560_9E2D_D391016A3559__INCLUDED_)
#define AFX_GXIMAGEPROCESS_H__2B09CF48_BA24_4560_9E2D_D391016A3559__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGxImageProcessApp:
// See GxImageProcess.cpp for the implementation of this class
//

class CGxImageProcessApp : public CWinApp
{
public:
	CGxImageProcessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxImageProcessApp)

	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGxImageProcessApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXIMAGEPROCESS_H__2B09CF48_BA24_4560_9E2D_D391016A3559__INCLUDED_)
