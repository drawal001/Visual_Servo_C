// GxMuitCamView.h : CGxMuitCamView 类的接口
//


#pragma once


class CGxMuitCamView : public CView
{
protected: // 仅从序列化创建
	CGxMuitCamView();
	DECLARE_DYNCREATE(CGxMuitCamView)

// 属性
public:
	CGxMuitCamDoc* GetDocument() const;

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
	virtual ~CGxMuitCamView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // GxMuitCamView.cpp 中的调试版本
inline CGxMuitCamDoc* CGxMuitCamView::GetDocument() const
   { return reinterpret_cast<CGxMuitCamDoc*>(m_pDocument); }
#endif

