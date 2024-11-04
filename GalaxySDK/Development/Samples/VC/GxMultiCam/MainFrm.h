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

	///给相机注册相应的回调，目前最多支持4相机
	void RegisterCallback(int nCamID);

	///错误信息提示
	void ShowErrorString(GX_STATUS emErrorStatus);

	///0号相机回调
	static void __stdcall OnFrameCallbackFun1(GX_FRAME_CALLBACK_PARAM* pFrame);

	///1号相机回调
	static void __stdcall OnFrameCallbackFun2(GX_FRAME_CALLBACK_PARAM* pFrame);

	///2号相机回调
	static void __stdcall OnFrameCallbackFun3(GX_FRAME_CALLBACK_PARAM* pFrame);

	///3号相机回调
	static void __stdcall OnFrameCallbackFun4(GX_FRAME_CALLBACK_PARAM* pFrame);

// Attributes
public:
	CMenu               m_objMenu;          ///< 声明菜单对象
	GX_DEV_HANDLE*	    m_hDevices;			///< 设备句柄指针
	GX_DS_HANDLE*	    m_hStream;		    ///< 设备流句柄
	CAMER_INFO*	  	    m_pstCam;			///< 相机数据结构体		
	int				    m_nOperateID;		///< 操作设备ID
	uint32_t		    m_nCamNum;			///< 相机数目
	BOOL			    m_bViewID;			///< 是否显示相机编号
	GX_DEVICE_BASE_INFO *m_pBaseinfo; 	    ///< 设备信息结构体
	CSplitterWnd        m_objSplitter;      ///< 窗口切分类
	CFps*               m_pCamsFps;         ///< 设备Fps计数对象

	// 图像显示资源定义
	CWnd                *m_pCWnds[MAX_DEVICE_MUN];       ///< 切分窗口指针数组

	

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
