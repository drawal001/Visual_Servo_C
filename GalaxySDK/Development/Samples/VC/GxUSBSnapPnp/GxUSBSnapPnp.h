// GxUSBSnapPnp.h : main header file for the GxUSBSnapPnp application
//

#if !defined(AFX_GxUSBSnapPnp_H__C976A1C0_0BCF_4E6E_B122_A3A90DF1AB92__INCLUDED_)
#define AFX_GxUSBSnapPnp_H__C976A1C0_0BCF_4E6E_B122_A3A90DF1AB92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGxUSBSnapPnpApp:
// See GxUSBSnapPnp.cpp for the implementation of this class
//

class CGxUSBSnapPnpApp : public CWinApp
{
public:
	CGxUSBSnapPnpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxUSBSnapPnpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGxUSBSnapPnpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxUSBSnapPnp_H__C976A1C0_0BCF_4E6E_B122_A3A90DF1AB92__INCLUDED_)
