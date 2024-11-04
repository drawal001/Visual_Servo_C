// GxViewer.h : main header file for the GXVIEWER application
//

#if !defined(AFX_GXVIEWER_H__2373B846_1B03_4874_821A_20A240ACD935__INCLUDED_)
#define AFX_GXVIEWER_H__2373B846_1B03_4874_821A_20A240ACD935__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGxViewerApp:
// See GxViewer.cpp for the implementation of this class
//

class CGxViewerApp : public CWinApp
{
public:
	CGxViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGxViewerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXVIEWER_H__2373B846_1B03_4874_821A_20A240ACD935__INCLUDED_)
