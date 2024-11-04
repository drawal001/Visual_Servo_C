// GxMultiCamDoc.cpp : implementation of the CGxMultiCamDoc class
//

#include "stdafx.h"
#include "GxMultiCam.h"

#include "GxMultiCamDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamDoc

IMPLEMENT_DYNCREATE(CGxMultiCamDoc, CDocument)

BEGIN_MESSAGE_MAP(CGxMultiCamDoc, CDocument)
	//{{AFX_MSG_MAP(CGxMultiCamDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamDoc construction/destruction

CGxMultiCamDoc::CGxMultiCamDoc()
{
	// TODO: add one-time construction code here

}

CGxMultiCamDoc::~CGxMultiCamDoc()
{
}

BOOL CGxMultiCamDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamDoc serialization

void CGxMultiCamDoc::Serialize(CArchive& ar)
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
// CGxMultiCamDoc diagnostics

#ifdef _DEBUG
void CGxMultiCamDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGxMultiCamDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamDoc commands
