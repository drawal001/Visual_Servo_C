#if !defined(AFX_SLTCAMDLG_H__7597506D_21C3_45A7_9714_246EEA8CE0F1__INCLUDED_)
#define AFX_SLTCAMDLG_H__7597506D_21C3_45A7_9714_246EEA8CE0F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SltCamDlg.h : header file
//
#include "GxIAPI.h"
#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CSltCamDlg dialog

class CSltCamDlg : public CDialog
{
// Construction
public:
	CSltCamDlg(CWnd* pParent = NULL);   // standard constructor
	CMainFrame         *m_pMainFrame;   ///< 主窗口框架指针
	uint32_t            m_nDeviceNum;   ///< 记录设备个数

// Dialog Data
	//{{AFX_DATA(CSltCamDlg)
	enum { IDD = IDD_DLG_CAMSLT };
	CComboBox	m_cmbcam;
	//}}AFX_DATA

public:

	///为主窗口分配资源信息
	bool AllocBufferForMainFrame();

	///关闭相机
	GX_STATUS CloseCamer();

	///清空Buffer
	void ClearBuffer();

	/// 枚举设备信息
	void UpdateDeviceList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSltCamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSltCamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeCmbCam();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnEnum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLTCAMDLG_H__7597506D_21C3_45A7_9714_246EEA8CE0F1__INCLUDED_)
