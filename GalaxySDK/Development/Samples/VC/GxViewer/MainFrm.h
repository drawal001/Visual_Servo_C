// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__561D6510_C385_4CB3_A6EA_80DA13D81C5E__INCLUDED_)
#define AFX_MAINFRM_H__561D6510_C385_4CB3_A6EA_80DA13D81C5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnOpenDevice();
	afx_msg void OnStartSnap();
	afx_msg void OnStopSnap();
	afx_msg void OnCloseDevice();
	afx_msg void OnClose();
	afx_msg void OnUpdateOpenDevice(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStartSnap(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStopSnap(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCloseDevice(CCmdUI* pCmdUI);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	/// ��ʼ������
	GX_STATUS InitParam();

	/// �ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// ��ȡ������Ϣ����
	void ShowErrorString(GX_STATUS emErrorStatus);

private:
	GX_DEV_HANDLE           m_hDevice;                ///< �豸���
	GX_DS_HANDLE			m_hStream;			      ///< �豸�����
	bool                    m_bIsOpen;                ///< ����򿪱�־ 
	bool                    m_bIsSnap;                ///< �����ʼ�ɼ���־

	CGXBitmap              *m_pBitmap;                ///< ��ʾ����ͼ����
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__561D6510_C385_4CB3_A6EA_80DA13D81C5E__INCLUDED_)
