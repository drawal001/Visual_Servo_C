// GxViewerDoc.h : interface of the CGxViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GXVIEWERDOC_H__54726830_1C0A_4797_BAEE_B0CC15397B26__INCLUDED_)
#define AFX_GXVIEWERDOC_H__54726830_1C0A_4797_BAEE_B0CC15397B26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGxViewerDoc : public CDocument
{
protected: // create from serialization only
	CGxViewerDoc();
	DECLARE_DYNCREATE(CGxViewerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGxViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGxViewerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXVIEWERDOC_H__54726830_1C0A_4797_BAEE_B0CC15397B26__INCLUDED_)
