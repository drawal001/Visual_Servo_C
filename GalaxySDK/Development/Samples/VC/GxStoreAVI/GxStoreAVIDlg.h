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

	/// �ص�����
	static void __stdcall         OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// ˢ��UI����
	void                          UpDateUI();

	/// ������Ϣ��ʾ
	void                          ShowErrorString(GX_STATUS emErrorStatus);

	/// �豸��ʼ��
	GX_STATUS                     InitDevice();

public:
	bool                          m_bEnableSaveAVI;     ///< �Ƿ�洢AVI��Ƶ
	BOOL                          m_bDevOpened;         ///< ��ʶ�豸�Ƿ��ѱ���

	GX_DEV_HANDLE                 m_hDevice;            ///< �豸���
	GX_DS_HANDLE                  m_hStream;			///< �豸�����

	CWnd                         *m_pWnd;               ///< ��ʾͼ�񴰿�(�ؼ�)ָ��
	CGXBitmap                    *m_pBitmap;            ///< ��ʾ����ͼ����

// Dialog Data
	//{{AFX_DATA(CGxStoreAVIDlg)
	enum { IDD = IDD_GXSTOREAVI_DIALOG };
	CString	                      m_strAVIFolder;
	BOOL	                      m_bCompressedAVI;     ///< �Ƿ�ѹ���洢
	BOOL	                      m_bSelectCompressed;  ///< �Ƿ���ѡ�������
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
