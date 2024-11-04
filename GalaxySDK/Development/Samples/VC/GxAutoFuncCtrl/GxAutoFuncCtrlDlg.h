// GxAutoFuncCtrlDlg.h : header file
//

#if !defined(AFX_GXAUTOFUNCCTRLDLG_H__48794C28_712D_4E1F_97EA_58B5A231704F__INCLUDED_)
#define AFX_GXAUTOFUNCCTRLDLG_H__48794C28_712D_4E1F_97EA_58B5A231704F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GxIAPI.h"
#include "DxImageProc.h"
#include "GxBitmap.h"
#include <string>
#include <iostream>

using namespace std;


/////////////////////////////////////////////////////////////////////////////
// CGxAutoFuncCtrlDlg dialog

class CGxAutoFuncCtrlDlg : public CDialog
{
// Construction
public:
	CGxAutoFuncCtrlDlg(CWnd* pParent = NULL);	// standard constructor
public:
    /// 回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 设备初始化
	GX_STATUS InitDevice();

	/// 更新感兴趣区域的取值范围
	void UpdateROIRange();

	/// 获取枚举型信息并初始化其对应的Combox框
	void InitEnumUI(char* strFeatureName, CComboBox *pComboBox, bool bIsImplemented);

	/// 初始化UI界面
	void InitUI();

	/// 初始化曝光相关的控件
	void InitShutterUI();

	/// 初始化增益相关的控件
	void InitGainUI();

	/// 初始化期望灰度值相关的控件
	void InitGrayUI();

	/// 初始化自动增益Combox控件
	void InitAutoGainUI();

	/// 初始化感兴趣区域的宽、高、X坐标和Y坐标的值
	void InitROIUI();

	/// 初始化自动曝光Combox控件
	void InitAutoShutterUI();

	/// 初始化2A光照环境Combox控件
	void InitLightEnvironmentUI();
	
	/// 刷新界面
	void UpDateUI();

	/// 刷新显示曝光和增益当前值
	void RefreshCurValue();

	///获取错误信息
	void ShowErrorString(GX_STATUS emErrorStatus);

// Dialog Data
	//{{AFX_DATA(CGxAutoFuncCtrlDlg)
	enum { IDD = IDD_GXAUTOFUNCCTRL_DIALOG };
	int		m_nGray;               ///< 期望灰度值
	int		m_nRoiX;               ///< 2A感兴趣区域X坐标
	int		m_nRoiY;               ///< 2A感兴趣区域Y坐标
	int		m_nRoiH;               ///< 2A感兴趣区域高
	int		m_nRoiW;               ///< 2A感兴趣区域宽
	double	m_dAutoGainMin;        ///< 自动增益最小值
	double  m_dAutoGainMax;        ///< 自动增益最大值
	double	m_dAutoShutterMin;     ///< 自动曝光最小值
	double	m_dAutoShutterMax;     ///< 自动曝光最大值
	double	m_dShutterValue;       ///< 曝光值
	double	m_dGainValue;          ///< 增益值       
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxAutoFuncCtrlDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxAutoFuncCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnCbnSelchangeComboLight();
	afx_msg void OnCbnSelchangeComboAutoGain();
	afx_msg void OnCbnSelchangeComboAutoShutter();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusEditRoiX();
	afx_msg void OnKillfocusEditRioY();
	afx_msg void OnKillfocusEditRoiW();
	afx_msg void OnKillfocusEditRoiH();
	afx_msg void OnKillfocusEditAutoGainMax();
	afx_msg void OnKillfocusEditAutoGainMin();
	afx_msg void OnKillfocusEditAutoShutterMax();
	afx_msg void OnKillfocusEditAutoShutterMin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnKillfocusEditCurShutter();
	afx_msg void OnKillfocusEditCurGain();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()    

public:
	GX_DEV_HANDLE                  m_hDevice;                ///< 设备句柄
	GX_DS_HANDLE			       m_hStream;				 ///< 设备流句柄

	GX_FLOAT_VALUE			       m_stShutterFloatRange;	 ///< 保存曝光时间范围
	GX_FLOAT_VALUE                 m_stGainFloatRange;       ///< 保存增益值范围           

	BOOL                           m_bDevOpened;             ///< 标识设备是否已打开
	BOOL                           m_bIsSnap;                ///< 标识设备是否已开采
	bool                           m_bImplementAutoGain;     ///< 是否支持自动增益
	bool                           m_bImplementAutoShutter;  ///< 是否支持自动曝光
	bool                           m_bImplementLight;        ///< 是否支持2A光照环境功能

	GX_EXPOSURE_AUTO_ENTRY         m_emAutoShutterMode;      ///< 自动曝光模式
	GX_GAIN_AUTO_ENTRY             m_emAutoGainMode;         ///< 自动增益模式

	CWnd                          *m_pWnd;                   ///< 显示图像窗口(控件)指针

	CGXBitmap                     *m_pBitmap;                ///< 显示及画图对象
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXAUTOFUNCCTRLDLG_H__48794C28_712D_4E1F_97EA_58B5A231704F__INCLUDED_)
