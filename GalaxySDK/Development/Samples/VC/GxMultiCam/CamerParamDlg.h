#if !defined(AFX_CAMERPARAMDLG_H__875A1E64_0402_42AB_A20C_F4E36375A831__INCLUDED_)
#define AFX_CAMERPARAMDLG_H__875A1E64_0402_42AB_A20C_F4E36375A831__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamerParamDlg.h : header file
//
#include "GxIAPI.h"
#include "MainFrm.h"

#define  WHITE_BALANCE_RNTRY  3         ///< 自动白平衡枚举数

/////////////////////////////////////////////////////////////////////////////
// CCamerParamDlg dialog

class CCamerParamDlg : public CDialog
{
// Construction
public:
	CCamerParamDlg(CWnd* pParent = NULL);   // standard constructor

	bool                m_bAutoWhiteBalance;       ///< 是否支持白平衡功能
	bool                m_bSpeedLevel;             ///< 是否支持采集速度级别

	int64_t             m_nAutoWhiteBalance;       ///< 自动白平衡当前选项

	GX_FLOAT_VALUE      m_stShutterFloatRange;     ///< 保存曝光时间的范围
	GX_FLOAT_VALUE      m_stGainFloatRange;        ///< 保存增益值的范围

	CMainFrame         *m_pMainFrame;              ///< 主窗口框架指针
// Dialog Data
	//{{AFX_DATA(CCamerParamDlg)
	enum { IDD = IDD_DLG_CAMERA_PARAM };
	int		m_nSpeedLevel;
	double	m_dShutterValue;
	double	m_dGainValue;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamerParamDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


public:
	/// 错误信息提示
	void ShowErrorString(GX_STATUS emErrorStatus);
	
    /// 刷新UI界面
	void UpDataUI();

	/// 初始化UI界面
	void InitUI();

	/// 初始化枚举类型UI界面
	void InitAutoWhiteBalanceUI(GX_DEV_HANDLE hDevice);
		
	/// 初始化曝光控制相关控件
	void InitShutterUI(GX_DEV_HANDLE hDevice);

	/// 初始化增益控制相关控件
	void InitGainUI(GX_DEV_HANDLE hDevice);

	/// 初始化采集速度级别相关控件
	void InitSpeedLevel(GX_DEV_HANDLE hDevice);

protected:
	// Generated message map functions
	//{{AFX_MSG(CCamerParamDlg)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnClose();
	afx_msg void OnSelchangeComboWriteBlance();
	afx_msg void OnKillfocusEditShutter();
	afx_msg void OnKillfocusEditGain();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERPARAMDLG_H__875A1E64_0402_42AB_A20C_F4E36375A831__INCLUDED_)
