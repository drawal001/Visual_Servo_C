// GxMulitCamForHDRDoc.h : CGxMulitCamForHDRDoc ��Ľӿ�
//


#pragma once
#include "GalaxyIncludes.h"

class CGxMulitCamForHDRDoc : public CDocument
{
protected: // �������л�����
	CGxMulitCamForHDRDoc();
	DECLARE_DYNCREATE(CGxMulitCamForHDRDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CGxMulitCamForHDRDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


