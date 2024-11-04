// GxAutoFuncCtrl.h : main header file for the GXAUTOFUNCCTRL application
//

#if !defined(AFX_GXAUTOFUNCCTRL_H__88D67D7B_9C11_4AD8_BFCD_1F5540B8E559__INCLUDED_)
#define AFX_GXAUTOFUNCCTRL_H__88D67D7B_9C11_4AD8_BFCD_1F5540B8E559__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGxAutoFuncCtrlApp:
// See GxAutoFuncCtrl.cpp for the implementation of this class
//

class CGxAutoFuncCtrlApp : public CWinApp
{
public:
	CGxAutoFuncCtrlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxAutoFuncCtrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGxAutoFuncCtrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXAUTOFUNCCTRL_H__88D67D7B_9C11_4AD8_BFCD_1F5540B8E559__INCLUDED_)
