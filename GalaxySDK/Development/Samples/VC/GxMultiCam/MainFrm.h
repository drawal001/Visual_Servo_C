// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F7267783_0D88_4AE4_AF53_C8B1FBC2BBF4__INCLUDED_)
#define AFX_MAINFRM_H__F7267783_0D88_4AE4_AF53_C8B1FBC2BBF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include "GxEx.h"
#include "Fps.h"

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Operations
public:

	///�����ע����Ӧ�Ļص���Ŀǰ���֧��4���
	void RegisterCallback(int nCamID);

	///������Ϣ��ʾ
	void ShowErrorString(GX_STATUS emErrorStatus);

	///0������ص�
	static void __stdcall OnFrameCallbackFun1(GX_FRAME_CALLBACK_PARAM* pFrame);

	///1������ص�
	static void __stdcall OnFrameCallbackFun2(GX_FRAME_CALLBACK_PARAM* pFrame);

	///2������ص�
	static void __stdcall OnFrameCallbackFun3(GX_FRAME_CALLBACK_PARAM* pFrame);

	///3������ص�
	static void __stdcall OnFrameCallbackFun4(GX_FRAME_CALLBACK_PARAM* pFrame);

// Attributes
public:
	CMenu               m_objMenu;          ///< �����˵�����
	GX_DEV_HANDLE*	    m_hDevices;			///< �豸���ָ��
	GX_DS_HANDLE*	    m_hStream;		    ///< �豸�����
	CAMER_INFO*	  	    m_pstCam;			///< ������ݽṹ��		
	int				    m_nOperateID;		///< �����豸ID
	uint32_t		    m_nCamNum;			///< �����Ŀ
	BOOL			    m_bViewID;			///< �Ƿ���ʾ������
	GX_DEVICE_BASE_INFO *m_pBaseinfo; 	    ///< �豸��Ϣ�ṹ��
	CSplitterWnd        m_objSplitter;      ///< �����з���
	CFps*               m_pCamsFps;         ///< �豸Fps��������

	// ͼ����ʾ��Դ����
	CWnd                *m_pCWnds[MAX_DEVICE_MUN];       ///< �зִ���ָ������

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMenuSltcam();
	afx_msg void OnMenuOpen();
	afx_msg void OnMenuClose();
	afx_msg void OnMenuStart();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMenuStop();
	afx_msg void OnUpdateMenuOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuClose(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuStop(CCmdUI* pCmdUI);
	afx_msg void OnMenuCamid();
	afx_msg void OnUpdateMenuCamid(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnMenuCammerParam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F7267783_0D88_4AE4_AF53_C8B1FBC2BBF4__INCLUDED_)
