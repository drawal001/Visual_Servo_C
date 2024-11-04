// GxImageProcessDlg.h : header file
//

#if !defined(AFX_GXIMAGEPROCESSDLG_H__FA4E5FCA_79E2_40DF_9164_D9FDEE73A8DB__INCLUDED_)
#define AFX_GXIMAGEPROCESSDLG_H__FA4E5FCA_79E2_40DF_9164_D9FDEE73A8DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGxImageProcessDlg dialog
#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include "StdAfx.h"
#include <vector>
#include <stdio.h>

using namespace std;

#define   CALCCCPARAM_LENGTH               (sizeof(VxInt16) * 9)      ///< 色彩调节数组长度  
#define   DEFAULT_LIGHTNESS                0                          ///< 默认亮度值
#define   DEFAULT_SHARPEN                  0.1f                       ///< 默认锐化值
#define   DEFAULT_SATURATION               64                         ///< 默认饱和度值
#define   DEFAULT_CONTRAST                 0                          ///< 默认对比度值
#define   DEFAULT_GAMMA                    1                          ///< 默认Gamma值
#define   DEFAULT_COLOR_CORRECTION         0                          ///< 默认颜色校正值
#define   WHITE_BALANCE_RNTRY              3                          ///< 自动白平衡枚举数


///< 判断返回值宏定义
#define VERIFY_STATUS_BREAK(emStatus) \
    if(emStatus != GX_STATUS_SUCCESS) \
    {\
        break;\
    }

class CGxImageProcessDlg : public CDialog
{
// Construction
public:
	explicit CGxImageProcessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGxImageProcessDlg)
	enum { IDD = IDD_GXIMAGEPROCESS_DIALOG };
	CSliderCtrl	m_sliderGamma;
	CSliderCtrl	m_sliderContrast;
	CSliderCtrl	m_sliderLightness;
	CSliderCtrl	m_sliderSaturation;
	CSliderCtrl	m_sliderSharpen;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxImageProcessDlg)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL



	// Generated message map functions
	//{{AFX_MSG(CGxImageProcessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenDevice();
	afx_msg void OnStartSnap();
	afx_msg void OnStopSnap();
	afx_msg void OnCloseDevice();
	afx_msg void OnSelchangeComboAwb();
	afx_msg void OnSelchangeComboAwbLight();
	afx_msg void OnSelchangeComboPixelFormat();
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnOpenBadPixelCorrect();
	afx_msg void OnOpenDenoise();
	afx_msg void OnOpenAccelerate();
	afx_msg void OnOpenSharpen();
	afx_msg void OnOpenColorCorrect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	/// 初始化参数
	GX_STATUS InitParam();

	/// 错误信息提示
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// 相机回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

	/// 初始化枚举类型UI界面
	void InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplement);

	/// 图像相关参数
	GX_STATUS GetImageParam();

	/// 初始化图像处理功能结构体
	bool InitImageProcessStruct();

	/// 释放图像处理功能申请的资源
	void UnInitImageProcessStruct();

	/// 获取图像质量提升参数
	void GetImageImproveParam();
	/// 释放图像质量提升参数占用的内存
	void ReleaseImageImproveParam();
	
	/// 初始化UI界面
	void InitUI();

	/// 更新UI界面
	void UpdateUI();


private:
    
    /// 设备是否开启颜色校正
    GX_STATUS __IsEnableDeviceColorCorrect();

    /// 设备是否开启Gamma使能
    GX_STATUS __IsEnableDeviceGamma();

    /// 设备是否开启锐化
    GX_STATUS __IsEnableDeviceSharpness();

	/// 初始下拉列表控件
	void __InitCombo();

	/// 初始化彩色相机部分参数
	GX_STATUS __InitColorParam();

	/// 初始化彩色图像处理结构体
	bool __InitColorStruct();

	/// 初始化黑白图像处理结构体
	bool __InitMonoStruct();

	/// 回调函数中黑白图像处理算法
	void OnMonoImageProcess();

	/// 回调函数中彩色图像处理算法
	void OnColorImageProcess();


	GX_DEV_HANDLE       m_hDevice;              ///< 设备句柄
	GX_DS_HANDLE	    m_hStream;			    ///< 设备流句柄
	void                *m_pBufferRaw;          ///< 原始图像数据
	int64_t             m_nPayLoadSize;         ///< 图像大小
	int64_t             m_nImageHeight;         ///< 原始图像高
    int64_t             m_nImageWidth;          ///< 原始图像宽
	int64_t             m_nPixelColorFilter;    ///< Bayer格式
	int64_t             m_i64PixelFormat;       ///< 当前像素格式
	bool                m_bColorFilter;         ///< 标识是否是彩色像素格式
	bool                m_bAwbLampHouse;        ///< 标示是否支持光源选择
	bool                m_bColorCorrection;     ///< 标示是否支持颜色校正
	bool                m_bIsOpenCC;            ///< 标识颜色校正复选框是否被选中
	bool                m_bIsOpen;              ///< 相机打开标志 
	bool                m_bIsSnap;              ///< 相机采集标志
	int64_t             m_nGetColorCorrection;  ///< 获取的颜色校正值
	int64_t             m_nColorCorrection;     ///< 颜色校正值
	int64_t             m_nContrast;            ///< 对比度值
	int64_t             m_nAutoWhiteBalance;    ///< 自动白平衡的值
	double              m_dGamma;               ///< The value of Gamma 
	int                 m_nLightness;           ///< 亮度值
	int                 m_nSaturation;          ///< 饱和度值
	int                 m_nPreSaturation;       ///< 饱和度更新前的值
	int64_t             m_nPreContrast;         ///< 对比度更新前的值
	int64_t             m_nPreColorCorrection;  ///< 颜色更新前的值
	int                 m_nPreLightness;        ///< 亮度更新前的值
	double              m_dPreGamma;            ///< Gammma更新前的值	
	float               m_fSharpen;             ///< 锐化值
	COLOR_IMG_PROCESS   m_objImageProcess;      ///< 彩色图像处理功能设置结构体
	bool                m_bUpData;              ///< 更新CPU当前显示
	HWND                m_hWndHandle;           ///< 显示图像的窗口句柄
    bool                m_bEnableGamma;         ///< 标识设备是否开启Gamma
    bool                m_bEnableSharpness;     ///< 标识设备是否开启锐化
	bool                m_bEnableOpen;          ///< 打开设备按键使能标志位
	MONO_IMG_PROCESS    m_objMonoImgProc;       ///< 黑白图像处理功能设置结构体
	bool                m_bSharpness;           ///< 标识锐化复选框是否被选中
	bool                m_bAccelerate;          ///< 标识算法加速复选框是否被选中
	bool                m_bDenoise;				///< 标识算法加速复选框是否被选中
	bool                m_bDefectivePixelCorrect; ///< 标识坏点校正复选框是否被选中
	bool                m_bImageProcess;        ///< 标识程序是否正在进行图像处理
	vector<GX_DEVICE_BASE_INFO>   m_vecDevInfo; ///< 设备基础信息结构体数组
	GX_DEVICE_BASE_INFO           *m_pBaseinfo; ///< 设备基础信息结构体指针

	CGXBitmap          *m_pBitmap;            ///< 显示及画图对象
	BYTE                *m_pGammaLut;           ///< Gamma查找表
	BYTE                *m_pContrastLut;        ///< 对比度查找表
	int                 m_nLutLength;           ///< 查找表长度
	// Implementation
	HICON m_hIcon;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_comboChooseDevice;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXIMAGEPROCESSDLG_H__FA4E5FCA_79E2_40DF_9164_D9FDEE73A8DB__INCLUDED_)
