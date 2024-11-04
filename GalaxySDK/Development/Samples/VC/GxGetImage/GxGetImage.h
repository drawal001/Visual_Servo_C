// GxGetImage.h : main header file for the GXGETIMAGE application
//

#if !defined(AFX_GXGETIMAGE_H__98B3018F_E408_4AB8_9E2F_AFB444FDC42F__INCLUDED_)
#define AFX_GXGETIMAGE_H__98B3018F_E408_4AB8_9E2F_AFB444FDC42F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGxGetImageApp:
// See GxGetImage.cpp for the implementation of this class
//

class CGxGetImageApp : public CWinApp
{
public:
	CGxGetImageApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxGetImageApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGxGetImageApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXGETIMAGE_H__98B3018F_E408_4AB8_9E2F_AFB444FDC42F__INCLUDED_)
