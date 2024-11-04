// GxMuitCamDoc.cpp : CGxMuitCamDoc 类的实现
//

#include "stdafx.h"
#include "GxMultiCam.h"

#include "GxMultiCamDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGxMuitCamDoc

IMPLEMENT_DYNCREATE(CGxMuitCamDoc, CDocument)

BEGIN_MESSAGE_MAP(CGxMuitCamDoc, CDocument)
END_MESSAGE_MAP()


// CGxMuitCamDoc 构造/析构

CGxMuitCamDoc::CGxMuitCamDoc()
{
	// TODO: 在此添加一次性构造代码

}

CGxMuitCamDoc::~CGxMuitCamDoc()
{
}

BOOL CGxMuitCamDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CGxMuitCamDoc 序列化

void CGxMuitCamDoc::Serialize(CArchive& ar)
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


// CGxMuitCamDoc 诊断

#ifdef _DEBUG
void CGxMuitCamDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGxMuitCamDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGxMuitCamDoc 命令
