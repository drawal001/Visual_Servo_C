#if !defined(AFX_CAMERPARAMDLG_H__875A1E64_0402_42AB_A20C_F4E36375A831__INCLUDED_)
#define AFX_CAMERPARAMDLG_H__875A1E64_0402_42AB_A20C_F4E36375A831__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamerParamDlg.h : header file
//
#include "GxIAPI.h"
#include "MainFrm.h"
#include "afxcmn.h"
#include <string>
#include <vector>
#include <sstream>
#include "afxwin.h"

typedef float float_t;
typedef double double_t;

#define TIMER_INTERVAL      (1000)      ///< 定时器间隔
#define IMG_FORMAT_SIZE     (64)        ///< 视频采集模式的显示格式大小

#define IMG_SIZE_OFF        ("Off")     ///< 显示格式：默认
#define IMG_SIZE_1080P      ("1080P")   ///< 显示格式：1080P
#define IMG_SIZE_720P       ("720P")    ///< 显示格式：720P
#define IMG_SIZE_480P       ("480P")    ///< 显示格式：480P

#define IMG_WIDTH_1920      (1920)      ///< 1080P图像宽
#define IMG_WIDTH_1280      (1280)      ///< 720P图像宽
#define IMG_WIDTH_640       (640)       ///< 480P图像宽

#define IMG_HEIGHT_1080     (1080)      ///< 1080P图像高
#define IMG_HEIGHT_1024     (1024)      ///< 720P图像高
#define IMG_HEIGHT_480      (480)       ///< 480P图像高

#define IMG_OFFSET_X_0      (0)         ///< 1080P水平偏移
#define IMG_OFFSET_X_320    (320)       ///< 720P水平偏移
#define IMG_OFFSET_X_640    (640)       ///< 480P水平偏移

#define IMG_OFFSET_Y_0      (0)         ///< 1080P垂直偏移
#define IMG_OFFSET_Y_28     (28)        ///< 720P垂直偏移
#define IMG_OFFSET_Y_300    (300)       ///< 480P垂直偏移

#define TIMER_AWB           (0)         ///< 自动白平衡Timer
#define TIMER_EXPOSURE_AUTO (1)         ///< 自动曝光Timer
#define TIMER_GAIN_AUTO     (2)         ///< 自动增益Timer
#define FLOAT_PRECISION     (100)       ///< 浮点精度，该值不能超过10^4，否则可能会因超过
                                        ///< MFC滑动条范围类型而设置范围失败

/// 视频采集模式
typedef struct GRAB_MODE
{
    char        strImgSize[IMG_FORMAT_SIZE];    ///< 显示格式：1080P/720P/480P
    int64_t     i64ImgWidth;                    ///< 图像宽 
    int64_t     i64ImgHeight;                   ///< 图像高
    int64_t     i64ImgOffsetX;                  ///< X偏移
    int64_t     i64ImgOffsetY;                  ///< Y偏移
}GRAB_MODE;

/////////////////////////////////////////////////////////////////////////////
// CCamerParamDlg dialog

class CCamerParamDlg : public CDialog
{
// Construction
public:
	CCamerParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCamerParamDlg)
	enum { IDD = IDD_DLG_CAMERA_PARAM };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamerParamDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CCamerParamDlg)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnClose();
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	

public:
    afx_msg void OnCbnSelchangeComboGrabMode();
    afx_msg void OnCbnSelchangeComboPixelFormat();
    afx_msg void OnCbnSelchangeComboReverseX();
    afx_msg void OnCbnSelchangeComboReverseY();
    afx_msg void OnCbnSelchangeComboGainAuto();
    afx_msg void OnCbnSelchangeComboGammaEnable();
    afx_msg void OnCbnSelchangeComboGammaMode();
    afx_msg void OnCbnSelchangeComboAwbMode();
    afx_msg void OnCbnSelchangeComboWbSelector();
    afx_msg void OnCbnSelchangeComboSaturationMode();
    afx_msg void OnCbnSelchangeComboExposureAuto();
    afx_msg void OnCbnSelchangeComboSharpnessMode();
    afx_msg void OnCbnSelchangeCombo2dNoiseReductionMode();
    afx_msg void OnCbnSelchangeCombo3dNoiseReductionMode();
    afx_msg void OnCbnSelchangeComboHdrMode();
    afx_msg void OnEnKillfocusEditGamma();
    afx_msg void OnEnKillfocusEditBalanceRatio();
    afx_msg void OnEnKillfocusEditSaturation();
    afx_msg void OnEnKillfocusEditExposureTime();
    afx_msg void OnEnKillfocusEditExpectedGrayValue();
    afx_msg void OnEnKillfocusEditSharpness();
    afx_msg void OnEnKillfocusEditHdrLongValue();
    afx_msg void OnEnKillfocusEditHdrShortValue();
    afx_msg void OnEnKillfocusEditHdrMainValue();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
    /// 初始化UI界面
    void __InitUI();

    /// 初始化图像格式控制相关控件
    void __InitImageFormatUI(void);

    /// 初始化模拟控制相关控件
    void __InitAnalogUI(void);

    /// 初始化颜色转换控制相关控件
    void __InitColorTransfUI(void);

    /// 初始化采集控制相关控件
    void __InitAcqUI(void);

    /// 初始化图像质量控制相关控件
    void __InitImgQualityUI(void);

    /// 初始化HDR控制相关控件
    void __InitHDRUI(void);

    /// 初始化采集视频模式
    void __InitComboGrabMode(CComboBox *pComboBox, bool &bIsEnable);

    /// 更新枚举下拉框UI
    void __UpDateEnumComboUI(char* chFeature, CComboBox *pComboBox, bool &bIsEnable);

    /// 更新Bool下拉框UI
    void __UpDateBoolComboUI(char* chFeature, CComboBox *pComboBox, bool &bIsEnable);

    /// 更新Float编辑框UI
    void __UpDateFloatEditUI(char* chFeature, CStatic *pStatic, CEdit *pEdit, CSliderCtrl *pSlider, bool &bIsReadable, bool &bIsEnable);

    /// 更新Int编辑框UI
    void __UpDateIntEditUI(char* chFeature, CStatic *pStatic, CEdit *pEdit, CSliderCtrl *pSlider, bool &bIsEnable);

    /// 更新UI值
    void __UpDateUI(const CComboBox *pComboBox);

    /// 更新UI使能状态
    void __UpDateUIEnable(void) const;

    /// 设置枚举项到设备中
    void __SetEnum(char* chFeature, const CComboBox *pComboBox);

    /// 设置Bool到设备中
    void __SetBool(char* chFeature, const CComboBox *pComboBox);

    /// 设置图像采集模式
    void __SetGrabMode(void);

    /// 获取节点是否可读，是否使能
    GX_STATUS __GetControlStatus(char* chFeature, bool &bIsReadable, bool &bIsEnable);

    /// 判断设备支持该分辨率
    void __IsSupportImgSize(bool &bIsEnable);

    /// 初始化图像采集模式
    void __InitGrabMode(void);

    /// 刷新自动白平衡
    void __RefreshAWB(void);

    /// 刷新白平衡系数
    void __RefreshWhiteRatio(void);

    /// 刷新自动曝光
    void __RefreshExposureAuto(void);

    /// 刷新曝光时间
    void __RefreshExposureTime(void);

    /// 刷新自动增益
    void __RefreshGainAuto(void);

    /// 设置FloatEdit
    void __SetFloatEdit(char* chFeature, const CEdit *pEdit, CSliderCtrl *pSlider);
    
    /// 设置IntEdit
    void __SetIntEdit(char* chFeature, const CEdit *pEdit, CSliderCtrl *pSlider);

    /// 设置Float滑动条
    void __SetFloatScroll(char* chFeature, int64_t i64CurValue, CEdit *pEdit);

    /// 设置Int滑动条
    void __SetIntScroll(char* chFeature, int64_t i64CurValue, CEdit *pEdit);

    /// 获取当前采集模式
    void __GetGrabModeValue(char* szCurValue);

private:
    CMainFrame                  *m_pMainFrame;                      ///< 主窗口框架指针
    GX_DEV_HANDLE               m_hDevice;                          ///< 设备句柄
    std::vector<GRAB_MODE>      m_vecGrabMode;                      ///< 视频采集模式
    GX_BALANCE_WHITE_AUTO_ENTRY m_emAWB;                            ///< 自动白平衡当前值
    GX_EXPOSURE_AUTO_ENTRY      m_emExposureAuto;                   ///< 自动曝光当前值
    bool                        m_bIsGrabModeEnable;                ///< 采集模式使能状态
    bool                        m_bIsPixelFormatEnable;             ///< 像素格式使能状态
    bool                        m_bIsReverseXEnable;                ///< 水平翻转使能状态
    bool                        m_bIsReverseYEnable;                ///< 垂直翻转使能状态
    bool                        m_bIsExposureTimeEnable;            ///< 曝光时间使能状态
    bool                        m_bIsExposureAutoEnable;            ///< 自动曝光使能状态
    bool                        m_bIsExpectedGrayValueEnable;       ///< 期望灰度值使能状态
    bool                        m_bIsGainAutoEnable;                ///< 自动增益使能状态
    bool                        m_bIsGammaEnableEnable;             ///< Gamma使能使能状态
    bool                        m_bIsGammaModeEnable;               ///< Gamma模式使能状态
    bool                        m_bIsGammaEnable;                   ///< Gamma值使能状态
    bool                        m_bIsAWBModeEnable;                 ///< 自动白平衡模式使能状态
    bool                        m_bIsAWBSelectorEnable;             ///< 白平衡通道选择使能状态
    bool                        m_bIsBalanceRatioEnable;            ///< 白平衡系数使能状态
    bool                        m_bIsSaturationModeEnable;          ///< 饱和度模式使能状态
    bool                        m_bIsSaturationEnable;              ///< 饱和度值使能状态
    bool                        m_bIsSharpnessModeEnable;           ///< 锐化模式使能状态
    bool                        m_bIsSharpnessEnable;               ///< 锐化值使能状态
    bool                        m_bIs2DNoiseReductionModeEnable;    ///< 2D降噪模式使能状态
    bool                        m_bIs3DNoiseReductionModeEnable;    ///< 3D降噪模式使能状态
    bool                        m_bIsHDRModeEnable;                 ///< HDR模式使能状态
    bool                        m_bIsHDRLongEnable;                 ///< HDR亮场期望值使能状态
    bool                        m_bIsHDRShortEnable;                ///< HDR暗场期望值使能状态
    bool                        m_bIsHDRMainEnable;                 ///< HDR融合期望值使能状态
    bool                        m_bIsBalanceRatioReadable;          ///< 白平衡系数只读状态
    bool                        m_bIsExposureTimeReadable;          ///< 曝光时间只读状态

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERPARAMDLG_H__875A1E64_0402_42AB_A20C_F4E36375A831__INCLUDED_)
