// GxFlatFieldCorrection.h : main header file for the GxFlatFieldCorrection application
//

#if !defined(AFX_GxFlatFieldCorrection_H__42E4C164_A272_45E1_945D_981DE8DEE59C__INCLUDED_)
#define AFX_GxFlatFieldCorrection_H__42E4C164_A272_45E1_945D_981DE8DEE59C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGxFlatFieldCorrectionApp:
// See GxFlatFieldCorrection.cpp for the implementation of this class
//

class CGxFlatFieldCorrectionApp : public CWinApp
{
public:
	CGxFlatFieldCorrectionApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxFlatFieldCorrectionApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGxFlatFieldCorrectionApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxFlatFieldCorrection_H__42E4C164_A272_45E1_945D_981DE8DEE59C__INCLUDED_)
