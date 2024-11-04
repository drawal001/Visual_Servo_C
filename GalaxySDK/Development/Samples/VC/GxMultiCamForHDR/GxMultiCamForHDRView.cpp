// GxMultiCamForHDRView.cpp : implementation of the CGxMultiCamForHDRView class
//

#include "stdafx.h"
#include "GxMultiCamForHDR.h"

#include "GxMultiCamForHDRDoc.h"
#include "GxMultiCamForHDRView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamForHDRView

IMPLEMENT_DYNCREATE(CGxMultiCamForHDRView, CView)

BEGIN_MESSAGE_MAP(CGxMultiCamForHDRView, CView)
	//{{AFX_MSG_MAP(CGxMultiCamForHDRView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamForHDRView construction/destruction

CGxMultiCamForHDRView::CGxMultiCamForHDRView()
{
	// TODO: add construction code here

}

CGxMultiCamForHDRView::~CGxMultiCamForHDRView()
{
}

BOOL CGxMultiCamForHDRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamForHDRView drawing

void CGxMultiCamForHDRView::OnDraw(CDC* pDC)
{
	CGxMultiCamForHDRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamForHDRView printing

BOOL CGxMultiCamForHDRView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGxMultiCamForHDRView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGxMultiCamForHDRView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamForHDRView diagnostics

#ifdef _DEBUG
void CGxMultiCamForHDRView::AssertValid() const
{
	CView::AssertValid();
}

void CGxMultiCamForHDRView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGxMultiCamForHDRDoc* CGxMultiCamForHDRView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGxMultiCamForHDRDoc)));
	return (CGxMultiCamForHDRDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGxMultiCamForHDRView message handlers
