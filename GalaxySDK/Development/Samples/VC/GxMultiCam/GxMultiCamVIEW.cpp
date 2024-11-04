// GxMultiCamView.cpp : implementation of the CGxMultiCamView class
//

#include "stdafx.h"
#include "GxMultiCam.h"

#include "GxMultiCamDoc.h"
#include "GxMultiCamView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamView

IMPLEMENT_DYNCREATE(CGxMultiCamView, CView)

BEGIN_MESSAGE_MAP(CGxMultiCamView, CView)
	//{{AFX_MSG_MAP(CGxMultiCamView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamView construction/destruction

CGxMultiCamView::CGxMultiCamView()
{
	// TODO: add construction code here

}

CGxMultiCamView::~CGxMultiCamView()
{
}

BOOL CGxMultiCamView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamView drawing

void CGxMultiCamView::OnDraw(CDC* pDC)
{
	CGxMultiCamDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamView printing

BOOL CGxMultiCamView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGxMultiCamView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGxMultiCamView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamView diagnostics

#ifdef _DEBUG
void CGxMultiCamView::AssertValid() const
{
	CView::AssertValid();
}

void CGxMultiCamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGxMultiCamDoc* CGxMultiCamView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGxMultiCamDoc)));
	return (CGxMultiCamDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamView message handlers
