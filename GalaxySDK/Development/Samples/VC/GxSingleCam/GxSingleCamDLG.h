// GxSingleCamColorDlg.h : header file
//

#if !defined(AFX_GxSingleCamColorDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_)
#define AFX_GxSingleCamColorDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  WHITE_BALANCE_RNTRY        3         ///< 自动白平衡枚举数
#define  DEFAULT_CONTRAST           0         ///< 默认对比度值
#define  DEFAULT_GAMMA              1         ///< 默认Gamma值
#define  DEFAULT_COLOR_CORRECTION   0         ///< 默认颜色校正值


/////////////////////////////////////////////////////////////////////////////
// CGxSingleCamDlg dialog

#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"

class CGxSingleCamDlg : public CDialog
{
// Construction
public:
	CGxSingleCamDlg(CWnd* pParent = NULL);	// standard constructor
	
public:
	GX_DEV_HANDLE       m_hDevice;              ///< 设备句柄
	GX_DS_HANDLE	    m_hStream;				///< 设备流句柄

	int64_t             m_nTriggerMode;         ///< 记录当前的触发模式
	int64_t             m_nAutoWhiteBalance;    ///< 自动白平衡当前选项
	int64_t             m_nRatioSelector;       ///< 记录当前的白平衡通

	GX_FLOAT_VALUE      m_stShutterFloatRange;  ///< 保存曝光时间的范围
	GX_FLOAT_VALUE      m_stGainFloatRange;     ///< 保存增益值的范围
	GX_FLOAT_VALUE      m_stFloatRange;         ///< 保存白平衡系数的范围  

	BYTE                *m_pGammaLut;           ///< Gamma查找表
	BYTE                *m_pContrastLut;        ///< 对比度查找表
	int                 m_nLutLength;           ///< 查找表长度
	int64_t             m_nContrast;            ///< 对比度值
	int64_t             m_nColorCorrection;     ///< 颜色校正值
	double              m_dGamma;               ///< The value of Gamma

    CString             m_strFilePath;          ///< BMP图像默认保存路径
	BOOL                m_bIsSaveImg;           ///< 标识是否保存图像
	BOOL                m_bDevOpened;           ///< 标识是否已打开设备
	BOOL                m_bIsSnap;              ///< 标识是否开始采集
	BOOL                m_bIsImproveImg;        ///< 标识是否进行图像质量提升

	bool                m_bTriggerMode;         ///< 是否支持触发模式
	bool                m_bTriggerActive;       ///< 是否支持触发极性
	bool                m_bTriggerSource;       ///< 是否支持触发源 
	bool                m_bColorFilter;         ///< 标识是否支持彩色
	bool                m_bBalanceRatioSelector;///< 标识是否支持白平衡通道

	CWnd                *m_pWnd;                ///< 显示图像窗口(控件)指针
	CGXBitmap           *m_pBitmap;                ///< 显示及画图对象
public:

	/// 获取设备参数,初始化界面控件
	void InitUI();

    /// 刷新UI界面
	void UpDateUI();

	/// 回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

	/// 获取图像质量提升参数
	void GetImageImproveParam();

	/// 保存图像
	void SaveImage();

	/// 错误信息提示
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// 初始化枚举类型UI界面
	void InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplement);

	/// 初始化白平衡系数相关控件
	void InitWhiteRatioUI();

	/// 刷新显示当前白平衡系数
	void RefreshWhiteRatio();

	/// 初始化增益等相关控件
	void InitGainUI();

	/// 初始化曝光相关控件
	void InitShutterUI();

	/// 初始化触发模式相关控件
	void InitTriggerModeUI();

	/// 初始化触发源相关控件
	void InitTriggerSourceUI();

	/// 初始化触发极性相关控件
	void InitTriggerActivationUI();

	/// 初始化插值算法相关控件
	void InitInterpolationAlgorithmUI(CComboBox *pComboBox, bool bIsColorCamera);

	/// 初始化相机:设置默认参数
	GX_STATUS InitDevice();

	/// 获取设备的宽、高等属性信息
	GX_STATUS GetDeviceParam();

// Dialog Data
	//{{AFX_DATA(CGxSingleCamDlg)
	enum { IDD = IDD_GXSINGLECAMCOLOR_DIALOG };
	double      m_dBalanceRatio;
	double	m_dGainValue;
	double	m_dShutterValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxSingleCamDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxSingleCamDlg)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnSofttrigger();
	afx_msg void OnCbnSelchangeComboTriggerMode();
	afx_msg void OnCbnSelchangeComboWriteBlance();
	afx_msg void OnCbnSelchangeComboTriggerSource();
	afx_msg void OnCbnSelchangeComboTriggerActive();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnBnClickedCheckSaveBmp();
	afx_msg void OnBnClickedCheckImageImprove();
	afx_msg void OnCbnSelchangeComboRatioSelector();
	afx_msg void OnEnKillfocusEditBalanceRatio();
	afx_msg void OnKillfocusEditShutter();
	afx_msg void OnKillfocusEditGain();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboInterpolationAlgorithm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxSingleCamColorDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_)
