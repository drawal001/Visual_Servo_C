// GxMultiCamForHDRDoc.cpp : implementation of the CGxMultiCamForHDRDoc class
//

#include "stdafx.h"
#include "GxMultiCamForHDR.h"

#include "GxMultiCamForHDRDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamForHDRDoc

IMPLEMENT_DYNCREATE(CGxMultiCamForHDRDoc, CDocument)

BEGIN_MESSAGE_MAP(CGxMultiCamForHDRDoc, CDocument)
	//{{AFX_MSG_MAP(CGxMultiCamForHDRDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamForHDRDoc construction/destruction

CGxMultiCamForHDRDoc::CGxMultiCamForHDRDoc()
{
	// TODO: add one-time construction code here

}

CGxMultiCamForHDRDoc::~CGxMultiCamForHDRDoc()
{
}

BOOL CGxMultiCamForHDRDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamForHDRDoc serialization

void CGxMultiCamForHDRDoc::Serialize(CArchive& ar)
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
// CGxMultiCamForHDRDoc diagnostics

#ifdef _DEBUG
void CGxMultiCamForHDRDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGxMultiCamForHDRDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamForHDRDoc commands
