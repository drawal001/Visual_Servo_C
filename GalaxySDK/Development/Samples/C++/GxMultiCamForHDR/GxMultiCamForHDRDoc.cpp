// GxMulitCamForHDRDoc.cpp : CGxMulitCamForHDRDoc ���ʵ��
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


// CGxMulitCamForHDRDoc ����/����

CGxMulitCamForHDRDoc::CGxMulitCamForHDRDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CGxMulitCamForHDRDoc::~CGxMulitCamForHDRDoc()
{
}

BOOL CGxMulitCamForHDRDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CGxMulitCamForHDRDoc ���л�

void CGxMulitCamForHDRDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CGxMulitCamForHDRDoc ���

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


// CGxMulitCamForHDRDoc ����
