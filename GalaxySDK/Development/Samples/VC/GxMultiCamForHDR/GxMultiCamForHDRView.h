// GxMultiCamForHDRView.h : interface of the CGxMultiCamForHDRView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GxMultiCamForHDRVIEW_H__1B3F5550_00C1_477F_B9BC_3540891B52AF__INCLUDED_)
#define AFX_GxMultiCamForHDRVIEW_H__1B3F5550_00C1_477F_B9BC_3540891B52AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGxMultiCamForHDRView : public CView
{
protected: // create from serialization only
	CGxMultiCamForHDRView();
	DECLARE_DYNCREATE(CGxMultiCamForHDRView)

// Attributes
public:
	CGxMultiCamForHDRDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxMultiCamForHDRView)
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
	virtual ~CGxMultiCamForHDRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGxMultiCamForHDRView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GxMultiCamForHDRView.cpp
inline CGxMultiCamForHDRDoc* CGxMultiCamForHDRView::GetDocument()
   { return (CGxMultiCamForHDRDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxMultiCamForHDRVIEW_H__1B3F5550_00C1_477F_B9BC_3540891B52AF__INCLUDED_)
