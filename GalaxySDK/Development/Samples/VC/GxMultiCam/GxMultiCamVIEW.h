// GxMultiCamView.h : interface of the CGxMultiCamView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GxMultiCamVIEW_H__1B3F5550_00C1_477F_B9BC_3540891B52AF__INCLUDED_)
#define AFX_GxMultiCamVIEW_H__1B3F5550_00C1_477F_B9BC_3540891B52AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGxMultiCamView : public CView
{
protected: // create from serialization only
	CGxMultiCamView();
	DECLARE_DYNCREATE(CGxMultiCamView)

// Attributes
public:
	CGxMultiCamDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxMultiCamView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGxMultiCamView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGxMultiCamView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GxMultiCamView.cpp
inline CGxMultiCamDoc* CGxMultiCamView::GetDocument()
   { return (CGxMultiCamDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxMultiCamVIEW_H__1B3F5550_00C1_477F_B9BC_3540891B52AF__INCLUDED_)
