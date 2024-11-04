// GxTransferDelay.h : main header file for the GxTransferDelay application
//

#if !defined(AFX_GxTransferDelay_H__1420ED1B_0137_4A92_ADD0_82A1E413221F__INCLUDED_)
#define AFX_GxTransferDelay_H__1420ED1B_0137_4A92_ADD0_82A1E413221F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGxTransferDelayApp:
// See GxTransferDelay.cpp for the implementation of this class
//

class CGxTransferDelayApp : public CWinApp
{
public:
	CGxTransferDelayApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxTransferDelayApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGxTransferDelayApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxTransferDelay_H__1420ED1B_0137_4A92_ADD0_82A1E413221F__INCLUDED_)
