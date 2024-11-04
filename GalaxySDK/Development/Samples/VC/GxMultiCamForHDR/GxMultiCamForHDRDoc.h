// GxMultiCamForHDRDoc.h : interface of the CGxMultiCamForHDRDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GxMultiCamForHDRDOC_H__BE3133BB_BB53_4601_A95F_2F64EC29E586__INCLUDED_)
#define AFX_GxMultiCamForHDRDOC_H__BE3133BB_BB53_4601_A95F_2F64EC29E586__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGxMultiCamForHDRDoc : public CDocument
{
protected: // create from serialization only
	CGxMultiCamForHDRDoc();
	DECLARE_DYNCREATE(CGxMultiCamForHDRDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxMultiCamForHDRDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGxMultiCamForHDRDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGxMultiCamForHDRDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxMultiCamForHDRDOC_H__BE3133BB_BB53_4601_A95F_2F64EC29E586__INCLUDED_)
