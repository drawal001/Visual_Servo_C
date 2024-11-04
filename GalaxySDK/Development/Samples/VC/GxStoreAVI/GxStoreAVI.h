// GxStoreAVI.h : main header file for the GXSTOREAVI application
//

#if !defined(AFX_GXSTOREAVI_H__EB826F87_7BF4_4543_9C27_1305B34427A1__INCLUDED_)
#define AFX_GXSTOREAVI_H__EB826F87_7BF4_4543_9C27_1305B34427A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGxStoreAVIApp:
// See GxStoreAVI.cpp for the implementation of this class
//

class CGxStoreAVIApp : public CWinApp
{
public:
	CGxStoreAVIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxStoreAVIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGxStoreAVIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class GxStoreAVI
{
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXSTOREAVI_H__EB826F87_7BF4_4543_9C27_1305B34427A1__INCLUDED_)
