// GxSnapRaw16.h : main header file for the GXSNAPRAW16 application
//

#if !defined(AFX_GXSNAPRAW16_H__6B45A42B_C3EA_4B96_9182_8F1EBF9B462A__INCLUDED_)
#define AFX_GXSNAPRAW16_H__6B45A42B_C3EA_4B96_9182_8F1EBF9B462A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGxSnapRaw16App:
// See GxSnapRaw16.cpp for the implementation of this class
//

class CGxSnapRaw16App : public CWinApp
{
public:
	CGxSnapRaw16App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxSnapRaw16App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGxSnapRaw16App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXSNAPRAW16_H__6B45A42B_C3EA_4B96_9182_8F1EBF9B462A__INCLUDED_)
