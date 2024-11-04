// GxViewerView.cpp : implementation of the CGxViewerView class
//

#include "stdafx.h"
#include "GxViewer.h"

#include "GxViewerDoc.h"
#include "GxViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGxViewerView

IMPLEMENT_DYNCREATE(CGxViewerView, CView)

BEGIN_MESSAGE_MAP(CGxViewerView, CView)
	//{{AFX_MSG_MAP(CGxViewerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxViewerView construction/destruction

CGxViewerView::CGxViewerView()
{
	// TODO: add construction code here

}

CGxViewerView::~CGxViewerView()
{
}

BOOL CGxViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGxViewerView drawing

void CGxViewerView::OnDraw(CDC* pDC)
{
	CGxViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGxViewerView diagnostics

#ifdef _DEBUG
void CGxViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CGxViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGxViewerDoc* CGxViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGxViewerDoc)));
	return (CGxViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGxViewerView message handlers
