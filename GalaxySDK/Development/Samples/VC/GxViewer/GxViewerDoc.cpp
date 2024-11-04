// GxViewerDoc.cpp : implementation of the CGxViewerDoc class
//

#include "stdafx.h"
#include "GxViewer.h"

#include "GxViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGxViewerDoc

IMPLEMENT_DYNCREATE(CGxViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CGxViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CGxViewerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxViewerDoc construction/destruction

CGxViewerDoc::CGxViewerDoc()
{
	// TODO: add one-time construction code here

}

CGxViewerDoc::~CGxViewerDoc()
{
}

BOOL CGxViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGxViewerDoc serialization

void CGxViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGxViewerDoc diagnostics

#ifdef _DEBUG
void CGxViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGxViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGxViewerDoc commands
