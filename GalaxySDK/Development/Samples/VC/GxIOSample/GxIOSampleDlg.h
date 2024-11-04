// GxIOSampleDlg.h : header file
//

#if !defined(AFX_GXIOSAMPLEDLG_H__26540673_FD9B_4799_889E_E05C3076016C__INCLUDED_)
#define AFX_GXIOSAMPLEDLG_H__26540673_FD9B_4799_889E_E05C3076016C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGxIOSampleDlg dialog
#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include <string>
#include <iostream>

using namespace std;

///< 宏定义用户IO输出值 0:false 1:true
#define OUTPUT_ACTIVATION_FALSE  0
#define OUTPUT_ACTIVATION_TRUE   1

///< 宏定义引脚电平反转值 1:true 0:false
#define LINE_INVERTER_FALSE 0
#define LINE_INVERTER_TRUE  1

class CGxIOSampleDlg : public CDialog
{
// Construction
public:
	CGxIOSampleDlg(CWnd* pParent = NULL);	// standard constructor

public:
	/// 回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

	/// 初始化相机:设置默认参数
	GX_STATUS InitDevice();

	/// 初始化UI界面
	void InitUI();

	/// 初始化组合框UI控件
	void InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplemented);

	/// 初始化增益等相关控件
	void InitGainUI();

	/// 初始化曝光相关控件
	void InitShutterUI();

	/// 初始化用户自定义输出值Combox控件
    void InitOutputValue();

	/// 刷新UI
	void UpDateUI();

	/// 错误信息提示
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// 初始化触发模式Combox控件
	void InitTriggerModeUI();

	/// 初始化触发源Combox控件
	void InitTriggerSourceUI();

    /// 初始化触发极性Combox控件
	void InitTriggerActivationUI();

	/// 初始化闪光灯开关Combox控件
	void InitStrobeSwitchUI();

	/// 初始化用户IO输出模式Combox控件
	void InitOutputModeUI();

	/// 初始化用户自定义输出选择Combox控件
	void InitOutputSelectorUI();

	/// 初始化引脚选择ComBox控件
	void InitLineSelector();

	/// 初始化引脚方向ComBox控件
	void InitLineMode();

	/// 初始化引脚电平反转ComBox控件
	void InitLineInverter();

	/// 初始化引脚输出源ComBox控件
	void InitLineSource();

	/// 切换引脚选择时，更新其关联的引脚方向，电平反转，输出源相对应的Combo框
	void UpdateLineRelativeInfo();

	/// 切换用户自定义输出选择时，更新其关联的用户自定义输出值相对应的Combo框
	void UpdateUserOutputRelativeInfo();

public:
	GX_DEV_HANDLE           m_hDevice;          ///< 设备句柄
	GX_DS_HANDLE			m_hStream;		    ///< 设备流句柄

	GX_FLOAT_VALUE          m_stShutterFloatRange;///< 保存曝光时间的范围
	GX_FLOAT_VALUE          m_stGainFloatRange;   ///< 保存增益值的范围

	int64_t                 m_nTriggerMode;       ///< 记录当前的触发模式

    BOOL                    m_bDevOpened;         ///< 标识设备是否已打开
    BOOL                    m_bIsSnap;            ///< 标识设备是否已开采

	bool                    m_bTriggerMode;       ///< 是否支持触发模式
	bool                    m_bTriggerActive;     ///< 是否支持触发极性
	bool                    m_bTriggerSource;     ///< 是否支持触发源
	bool                    m_bOutputSelector;    ///< 是否支持输出源
	bool                    m_bOutputMode;        ///< 是否支持输出模式
	bool                    m_bOutputValue;       ///< 是否支持输出值
	bool                    m_bStrobeSwitch;      ///< 是否支持闪光灯开关
	
	bool                    m_bLineSelector;      ///< 是否支持引脚选择
	bool                    m_bLineMode;          ///< 是否支持引脚方向
	bool                    m_bLineInverter;      ///< 是否支持引脚电平反转
	bool                    m_bLineSource;        ///< 是否支持引脚输出源
	bool                    m_bStrobeSwitchIsUse; ///< 闪光灯开关是否可用

	// 图像显示资源定义
	CWnd                   *m_pWnd;             ///< 图像显示控件窗口指针
	CGXBitmap              *m_pBitmap;            ///< 显示及画图对象

// Dialog Data
	//{{AFX_DATA(CGxIOSampleDlg)
	enum { IDD = IDD_GXIOSAMPLE_DIALOG };
	double	m_dGainValue;
	double	m_dShutterValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxIOSampleDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxIOSampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnCbnSelchangeComboOutputSource();
	afx_msg void OnCbnSelchangeComboOutputMode();
	afx_msg void OnCbnSelchangeComboOutputValue();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboTriggerSource();
	afx_msg void OnCbnSelchangeComboTriggerMode();
	afx_msg void OnBnClickedBtnSendSoftTrigger();
	afx_msg void OnCbnSelchangeComboTriggerActivation();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnCbnSelchangeComboStrobeSwitch();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnKillfocusEditShutterValue();
	afx_msg void OnKillfocusEditGainValue();
	afx_msg void OnCbnSelchangeComboLineSelector();
	afx_msg void OnCbnSelchangeComboLineMode();
	afx_msg void OnCbnSelchangeComboLineInverter();
	afx_msg void OnCbnSelchangeComboLineSource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXIOSAMPLEDLG_H__26540673_FD9B_4799_889E_E05C3076016C__INCLUDED_)
