// GxMulitCamForHDRView.h : CGxMulitCamForHDRView ��Ľӿ�
//


#pragma once


class CGxMulitCamForHDRView : public CView
{
protected: // �������л�����
	CGxMulitCamForHDRView();
	DECLARE_DYNCREATE(CGxMulitCamForHDRView)

// ����
public:
	CGxMulitCamForHDRDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CGxMulitCamForHDRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // GxMulitCamForHDRView.cpp �еĵ��԰汾
inline CGxMulitCamForHDRDoc* CGxMulitCamForHDRView::GetDocument() const
   { return reinterpret_cast<CGxMulitCamForHDRDoc*>(m_pDocument); }
#endif

