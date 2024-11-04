                                                                                                                                                                                                                                                                                                                                                      // GxStoreAVIDlg.h : header file
//

#if !defined(AFX_GXSTOREAVIDLG_H__A8E25217_6814_45C7_AF97_010FAB1249DC__INCLUDED_)
#define AFX_GXSTOREAVIDLG_H__A8E25217_6814_45C7_AF97_010FAB1249DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include <string>
#include <iostream>

using namespace std;

#define _WIN32_DCOM
#include <ObjBase.h>
#pragma comment(lib, "ole32.lib")

/////////////////////////////////////////////////////////////////////////////
// CGxStoreAVIDlg dialog
class CGxStoreAVIDlg : public CDialog
{
// Construction
public:
	CGxStoreAVIDlg(CWnd* pParent = NULL);	// standard constructor

	/// 回调函数
	static void __stdcall         OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 刷新UI界面
	void                          UpDateUI();

	/// 错误信息提示
	void                          ShowErrorString(GX_STATUS emErrorStatus);

	/// 设备初始化
	GX_STATUS                     InitDevice();

public:
	bool                          m_bEnableSaveAVI;     ///< 是否存储AVI视频
	BOOL                          m_bDevOpened;         ///< 标识设备是否已被打开

	GX_DEV_HANDLE                 m_hDevice;            ///< 设备句柄
	GX_DS_HANDLE                  m_hStream;			///< 设备流句柄

	CWnd                         *m_pWnd;               ///< 显示图像窗口(控件)指针
	CGXBitmap                    *m_pBitmap;            ///< 显示及画图对象

// Dialog Data
	//{{AFX_DATA(CGxStoreAVIDlg)
	enum { IDD = IDD_GXSTOREAVI_DIALOG };
	CString	                      m_strAVIFolder;
	BOOL	                      m_bCompressedAVI;     ///< 是否压缩存储
	BOOL	                      m_bSelectCompressed;  ///< 是否已选择编码器
	BOOL	                      m_bShowImage;
	DX_IMAGE_FORMAT_CONVERT_HANDLE m_hConvert;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxStoreAVIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxStoreAVIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnBnClickedBtnSaveAs();
	afx_msg void OnBnClickedBtnAviStart();
	afx_msg void OnBnClickedBtnStopAvi();
	afx_msg void OnBnClickedCheckCompressedAvi();
	afx_msg void OnBnClickedCheckShowImage();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnSelectCompressor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	DWORD m_wPlayFPS;
	afx_msg void OnEnKillfocusEditFps();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXSTOREAVIDLG_H__A8E25217_6814_45C7_AF97_010FAB1249DC__INCLUDED_)
