// GxMuitCamView.cpp : CGxMuitCamView 类的实现
//

#include "stdafx.h"
#include "GxMultiCam.h"

#include "GxMultiCamDoc.h"
#include "GxMultiCamView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGxMuitCamView

IMPLEMENT_DYNCREATE(CGxMuitCamView, CView)

BEGIN_MESSAGE_MAP(CGxMuitCamView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGxMuitCamView 构造/析构

CGxMuitCamView::CGxMuitCamView()
{
	// TODO: 在此处添加构造代码

}

CGxMuitCamView::~CGxMuitCamView()
{
}

BOOL CGxMuitCamView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGxMuitCamView 绘制

void CGxMuitCamView::OnDraw(CDC* /*pDC*/)
{
	CGxMuitCamDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CGxMuitCamView 打印

BOOL CGxMuitCamView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGxMuitCamView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGxMuitCamView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CGxMuitCamView 诊断

#ifdef _DEBUG
void CGxMuitCamView::AssertValid() const
{
	CView::AssertValid();
}

void CGxMuitCamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGxMuitCamDoc* CGxMuitCamView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGxMuitCamDoc)));
	return (CGxMuitCamDoc*)m_pDocument;
}
#endif //_DEBUG


// CGxMuitCamView 消息处理程序