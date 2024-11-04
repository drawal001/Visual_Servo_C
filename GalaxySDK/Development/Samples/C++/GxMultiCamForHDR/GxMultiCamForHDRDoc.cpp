// GxMulitCamForHDRDoc.cpp : CGxMulitCamForHDRDoc 类的实现
//

#include "stdafx.h"
#include "GxMultiCamForHDR.h"

#include "GxMultiCamForHDRDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGxMulitCamForHDRDoc

IMPLEMENT_DYNCREATE(CGxMulitCamForHDRDoc, CDocument)

BEGIN_MESSAGE_MAP(CGxMulitCamForHDRDoc, CDocument)
END_MESSAGE_MAP()


// CGxMulitCamForHDRDoc 构造/析构

CGxMulitCamForHDRDoc::CGxMulitCamForHDRDoc()
{
	// TODO: 在此添加一次性构造代码

}

CGxMulitCamForHDRDoc::~CGxMulitCamForHDRDoc()
{
}

BOOL CGxMulitCamForHDRDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CGxMulitCamForHDRDoc 序列化

void CGxMulitCamForHDRDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CGxMulitCamForHDRDoc 诊断

#ifdef _DEBUG
void CGxMulitCamForHDRDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGxMulitCamForHDRDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGxMulitCamForHDRDoc 命令
