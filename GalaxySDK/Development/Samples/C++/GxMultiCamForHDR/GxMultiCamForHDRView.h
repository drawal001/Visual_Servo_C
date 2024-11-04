// GxMulitCamForHDRView.h : CGxMulitCamForHDRView 类的接口
//


#pragma once


class CGxMulitCamForHDRView : public CView
{
protected: // 仅从序列化创建
	CGxMulitCamForHDRView();
	DECLARE_DYNCREATE(CGxMulitCamForHDRView)

// 属性
public:
	CGxMulitCamForHDRDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CGxMulitCamForHDRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // GxMulitCamForHDRView.cpp 中的调试版本
inline CGxMulitCamForHDRDoc* CGxMulitCamForHDRView::GetDocument() const
   { return reinterpret_cast<CGxMulitCamForHDRDoc*>(m_pDocument); }
#endif

