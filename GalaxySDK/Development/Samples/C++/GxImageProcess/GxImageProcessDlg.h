// GxImageProcessDlg.h : header file

#pragma once
#include "afxcmn.h"
#include "GalaxyIncludes.h"
#include "GXBitmap.h"

#define PARAM_TRANSFORMATION_VALUE  10.0 ///< 由于UI中滑动条控件只接受整型值，Gamma值和锐化值是小数，
                                         ///< 在这里我们需要在获取设备Gamma和锐化值后将其转变为整数值

// CGxImageProcessDlg dialog
class CGxImageProcessDlg : public CDialog
{
public:
	//---------------------------------------------------------------------------------
	/**
	\brief   用户继承采集事件处理类
	*/
	//----------------------------------------------------------------------------------
	class CSampleCaptureEventHandler : public ICaptureEventHandler
	{
	public:
		//---------------------------------------------------------------------------------
		/**
		\brief   采集回调函数
		\param   objImageDataPointer      图像处理参数
		\param   pFrame                   用户参数

		\return  无
		*/
		//----------------------------------------------------------------------------------
		void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
		{
			try
			{
				CGxImageProcessDlg*  pImgProDlg = (CGxImageProcessDlg*)pUserParam;
				pImgProDlg->ShowImageProcess(objImageDataPointer);
			}
			catch (CGalaxyException)
			{
				//do nothing
			}
			catch (std::exception)
			{
				//do nothing
			}
			
		}

	};

// Construction
	explicit CGxImageProcessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GXIMAGEPROCESS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
	

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOpenDevice();
	afx_msg void OnBnClickedCloseDevice();
	afx_msg void OnBnClickedStartSnap();
	afx_msg void OnBnClickedStopSnap();
	afx_msg void OnCbnSelchangeComboAwb();
	afx_msg void OnCbnSelchangeComboAwbLight();
	afx_msg void OnBnClickedAidcOpenAccelerateccelerateCheck();
	afx_msg void OnBnClickedOpenBadPixelCorrect();
	afx_msg void OnBnClickedOpenDenoise();
	afx_msg void OnBnClickedOpenColorCorrect();
	afx_msg void OnBnClickedOpenSharpen();
	afx_msg void OnCbnDropdownComboAwb();
	afx_msg void OnCbnDropdownComboAwbLight();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	/// 图像处理函数
	void ShowImageProcess(CImageDataPointer& objImageDataPointer);

private:

	/// 初始化界面
	void __InitUI(); 

	/// 初始化枚举类型UI界面
	int __InitEnumUI(const gxstring& strFeaturName, CComboBox *pComboBox, bool bIsImplemented);

	/// 初始化设备参数
	void __InitParam(); 

	/// 更新界面
	void __UpdateUI();

	///对RGB和BGR的UI界面做特殊处理
	void __UpdateRGBUI();

	/// 改变Combox下拉框的宽度
	void __ChangeComBoxWidth(CComboBox* pComBox);

    /// 判断相机是否开启颜色校正
    void __IsEnableDeviceColorCorrection();

    /// 判断相机是否开启Gamma使能
    void __IsEnableDeviceGamma();

    /// 判断相机是否开启锐化
    void __IsEnableDeviceSharpen();

	/// 初始化下拉框
	void __InitCombo();

	HICON                              m_hIcon;
	CGXDevicePointer                   m_objDevicePtr;                     ///< 设备句柄
	CGXStreamPointer                   m_objStreamPtr;                     ///< 流对象
	CGXFeatureControlPointer           m_objFeatureControlPtr;             ///< 属性控制器对象
	CGXFeatureControlPointer           m_objStreamFeatureControlPtr;       ///< 流层控制器对象
	CImageProcessConfigPointer         m_objImageProcessPtr;               ///< 图像处理对象
	CGXBitmap*                         m_pBitmap;                          ///< 显示及画图对象
	CWnd*                              m_pWnd;                             ///< 窗口句柄
	CSampleCaptureEventHandler*        m_pSampleCaptureEventHandler;       ///< 回调函数指针
	bool                               m_bIsOpen;                          ///< 相机打开标志
	bool                               m_bIsSnap;                          ///< 相机采集标志
	bool                               m_bIsWhiteBalance;                  ///< 是否支持白平衡
	bool                               m_bIsWhiteBalanceLight;             ///< 是否支持白平衡光源
	int                                m_nComBoAWBOldValue;                ///< 自动白平衡值
	int                                m_nComBoAWBLightOldValue;           ///< 自动白平衡光源值
	int                                m_nOldSliderSharpenValue;           ///< 记录锐化值
	int                                m_nOldSliderGammaValue;             ///< 记录Gamma值
	int                                m_nOldSliderContrastValue;          ///< 记录对比度的值
	int                                m_nOldSliderLightnessValue;         ///< 记录亮度值
	int                                m_nOldSliderSaturationValue;        ///< 记录饱和度值
	std::string                        m_strBalanceWhiteAutoValue;         ///< 记录自动白平衡模式
    bool                               m_bEnableColorCorrect;              ///< 颜色校正使能开关
    bool                               m_bEnableGamma;                     ///< Gamma使能开关
    bool                               m_bEnableSharpness;                 ///< 锐化使能开关
	bool                               m_bEnableStaturation;               ///< 饱和度使能开关
	bool                               m_bEnableCheckDenoise;              ///< 降噪使能开关
	bool                               m_bIsColorFilter;                   ///< 打开的相机是否为彩色相机标志
	bool                               m_bEnumDevices;                     ///< 设备是否枚举到设备标志
	CSliderCtrl                        m_sliderSharpen;
	CSliderCtrl                        m_sliderGamma;
	CSliderCtrl                        m_sliderContrast;
	CSliderCtrl                        m_sliderLightness;
	CSliderCtrl                        m_sliderSaturation;
	BOOL                               m_bCheckAcceleract;
	BOOL                               m_bCheckPixelCorrect;
	BOOL                               m_bCheckDenoise;
	BOOL                               m_bCheckColorCorrect;
	BOOL                               m_bCheckSharpen;
	CComboBox                          m_comboChooseDevice;
	GxIAPICPP::gxdeviceinfo_vector     m_vecDeviceInfo;
};
