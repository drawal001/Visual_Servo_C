// GxMulitCamForHDRView.cpp : CGxMulitCamForHDRView ���ʵ��
//

#include "stdafx.h"
#include "GxMultiCamForHDR.h"

#include "GxMultiCamForHDRDoc.h"
#include "GxMultiCamForHDRView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGxMulitCamForHDRView

IMPLEMENT_DYNCREATE(CGxMulitCamForHDRView, CView)

BEGIN_MESSAGE_MAP(CGxMulitCamForHDRView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGxMulitCamForHDRView ����/����

CGxMulitCamForHDRView::CGxMulitCamForHDRView()
{
	// TODO: �ڴ˴���ӹ������

}

CGxMulitCamForHDRView::~CGxMulitCamForHDRView()
{
}

BOOL CGxMulitCamForHDRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CGxMulitCamForHDRView ����

void CGxMulitCamForHDRView::OnDraw(CDC* /*pDC*/)
{
	CGxMulitCamForHDRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CGxMulitCamForHDRView ��ӡ

BOOL CGxMulitCamForHDRView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CGxMulitCamForHDRView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CGxMulitCamForHDRView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CGxMulitCamForHDRView ���

#ifdef _DEBUG
void CGxMulitCamForHDRView::AssertValid() const
{
	CView::AssertValid();
}

void CGxMulitCamForHDRView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGxMulitCamForHDRDoc* CGxMulitCamForHDRView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGxMulitCamForHDRDoc)));
	return (CGxMulitCamForHDRDoc*)m_pDocument;
}
#endif //_DEBUG


// CGxMulitCamForHDRView ��Ϣ�������