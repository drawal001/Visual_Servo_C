// GxViewerView.h : interface of the CGxViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GXVIEWERVIEW_H__0A34C021_A997_4FFD_AD41_8475C657AE3D__INCLUDED_)
#define AFX_GXVIEWERVIEW_H__0A34C021_A997_4FFD_AD41_8475C657AE3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGxViewerView : public CView
{
protected: // create from serialization only
	CGxViewerView();
	DECLARE_DYNCREATE(CGxViewerView)

// Attributes
public:
	CGxViewerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGxViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGxViewerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GxViewerView.cpp
inline CGxViewerDoc* CGxViewerView::GetDocument()
   { return (CGxViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXVIEWERVIEW_H__0A34C021_A997_4FFD_AD41_8475C657AE3D__INCLUDED_)
