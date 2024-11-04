// GxMulitCamForHDRView.cpp : CGxMulitCamForHDRView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGxMulitCamForHDRView 构造/析构

CGxMulitCamForHDRView::CGxMulitCamForHDRView()
{
	// TODO: 在此处添加构造代码

}

CGxMulitCamForHDRView::~CGxMulitCamForHDRView()
{
}

BOOL CGxMulitCamForHDRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGxMulitCamForHDRView 绘制

void CGxMulitCamForHDRView::OnDraw(CDC* /*pDC*/)
{
	CGxMulitCamForHDRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CGxMulitCamForHDRView 打印

BOOL CGxMulitCamForHDRView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGxMulitCamForHDRView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGxMulitCamForHDRView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CGxMulitCamForHDRView 诊断

#ifdef _DEBUG
void CGxMulitCamForHDRView::AssertValid() const
{
	CView::AssertValid();
}

void CGxMulitCamForHDRView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGxMulitCamForHDRDoc* CGxMulitCamForHDRView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGxMulitCamForHDRDoc)));
	return (CGxMulitCamForHDRDoc*)m_pDocument;
}
#endif //_DEBUG


// CGxMulitCamForHDRView 消息处理程序