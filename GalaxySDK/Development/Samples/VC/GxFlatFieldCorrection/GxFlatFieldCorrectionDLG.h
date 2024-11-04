// GxFlatFieldCorrectionDlg.h : header file
//

#if !defined(AFX_GxFlatFieldCorrectionDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_)
#define AFX_GxFlatFieldCorrectionDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  WHITE_BALANCE_RNTRY               3      ///< 自动白平衡枚举数

#define  GRAY_MIN                          0      ///< 灰度最小值
#define  GRAY_MAX                          255    ///< 灰度最大值
#define  GRAY_INVALID                      256    ///< 灰度无效值
#define  GRAY_INIT                         127    ///< 灰度初始化值
#define  BRIGHT_IMG_GRAY_MIN               20     ///< 亮场图像灰度最小值
#define  BRIGHT_IMG_GRAY_MAX               250    ///< 亮场图像灰度最大值

#define  CORRECTION_UI_FRAME_COUNT_MIN     0      ///< UI界面采集帧数最小值
#define  CORRECTION_UI_FRAME_COUNT_1       1      ///< UI界面采集帧数1
#define  CORRECTION_UI_FRAME_COUNT_2       2      ///< UI界面采集帧数2
#define  CORRECTION_UI_FRAME_COUNT_3       3      ///< UI界面采集帧数3
#define  CORRECTION_UI_FRAME_COUNT_MAX     4      ///< UI界面采集帧数最大值

#define  CORRECTION_ACTUAL_FRAME_COUNT_1   1      ///< 平场校正实际采集帧数1
#define  CORRECTION_ACTUAL_FRAME_COUNT_2   2      ///< 平场校正实际采集帧数2
#define  CORRECTION_ACTUAL_FRAME_COUNT_4   4      ///< 平场校正实际采集帧数4
#define  CORRECTION_ACTUAL_FRAME_COUNT_8   8      ///< 平场校正实际采集帧数8
#define  CORRECTION_ACTUAL_FRAME_COUNT_16  16     ///< 平场校正实际采集帧数16

#define  IMAGE_DATA_NUM_MAX                16     ///< 像素位宽最大值
#define  IMAGE_DATA_NUM_MIN                8      ///< 像素位宽最小值

/// 文件状态码
typedef enum  FFC_STATUS
{
	FFC_STATUS_SUCCESS          =  0,   ///< 文件操作成功
	FFC_STATUS_FILE_OPEN_ERROR  = -1,   ///< 文件打开失败
	FFC_STATUS_FILE_WRITE_ERROR = -2,   ///< 文件写入失败
	FFC_STATUS_FILE_READ_ERROR  = -3,   ///< 文件读取失败
	FFC_STATUS_FILE_PATH_ERROR  = -4,   ///< 文件路径非法
	FFC_STATUS_FILE_DATA_ERROR  = -5    ///< 文件数据异常
} FFC_STATUS;

/////////////////////////////////////////////////////////////////////////////
// CGxFlatFieldCorrectionDlg dialog

#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include <string>
#include <iostream>
#include <new>

class CGxFlatFieldCorrectionDlg : public CDialog
{
// Construction
public:
	CGxFlatFieldCorrectionDlg(CWnd* pParent = NULL);           ///< standard constructor
	
private:
	GX_DEV_HANDLE       m_hDevice;                             ///< 设备句柄
	GX_DS_HANDLE	    m_hStream;				               ///< 设备流句柄

	int64_t             m_nAutoWhiteBalance;                   ///< 自动白平衡当前选项
	int64_t             m_nRatioSelector;                      ///< 记录当前的白平衡通道
	int64_t             m_i64ImageHeight;                      ///< 原始图像高
	int64_t             m_i64ImageWidth;                       ///< 原始图像宽

	GX_FLOAT_VALUE      m_stShutterFloatRange;                 ///< 保存曝光时间的范围
	GX_FLOAT_VALUE      m_stGainFloatRange;                    ///< 保存增益值的范围
	GX_FLOAT_VALUE      m_stFloatRange;                        ///< 保存白平衡系数的范围  

	bool                m_bDevOpened;                          ///< 标识是否已打开设备
	bool                m_bIsSnap;                             ///< 标识是否开始采集
 
	bool                m_bColorFilter;                        ///< 标识当前是否为彩色
	bool                m_bBalanceRatioSelector;               ///< 标识是否支持白平衡通道

	CWnd                *m_pWnd;                               ///< 显示图像窗口(控件)指针

	BYTE                *m_pBrightBuf;                         ///< 亮场图像buffer
	BYTE                *m_pDarkBuf;                           ///< 暗场图像buffer
	bool                m_bDarkImgAcquired;                    ///< 标识是否采集暗场图像
	bool                m_bDarkImgAcquiredOnce;                ///< 标记是否已采集暗场图像
	bool                m_bBrightImgAcquired;                  ///< 标识是否采集亮场图像
	bool                m_bBrightImgAcquiredOnce;              ///< 标记是否已采集亮场图像      
	DX_ACTUAL_BITS      m_nActualBits;                         ///< 像素实际位宽
	bool                m_bGetFFCCoefficients;                 ///< 标识是否获得平场校正系数

	bool                m_bExecuteFFC;                         ///< 标识是否执行平场校正
	bool                m_bIsFFC;                              ///< 标识对采集的图像是否进行平场校正（FFC）
	bool                m_bShowFFCOK;                          ///< 标识是否显示平场校正（FFC）执行成功

	BYTE                *m_pFFCCoefficients;                   ///< 平场校正参数
	int                 m_nFFClength;                          ///< 平场校正参数长度（字节数）
	int                 m_nFFCGray;                            ///< 进行平场校正采用的期望灰度值
	int                 m_nNewGray;                            ///< 界面设置的期望灰度值
	bool                m_bFFCGray;                            ///< 标识是否采用用户设置的期望灰度值
	int                 m_nFrameCount;                         ///< 采集帧数
	int                 m_nBrightImgNum;                       ///< 采集亮场图像数
	int                 m_nAverageGrayDarkImg;                 ///< 暗场图像平均灰度值
	int                 m_nAverageGrayBrightImg;               ///< 亮场图像平均灰度值
	VxUint32            *m_pAccumulateImgBuf;                  ///< 亮场累加图像buffer
	BYTE                *m_pAverageImgBuf;                     ///< 亮场平均图像buffer

	FLAT_FIELD_CORRECTION_PROCESS    m_stFlatFieldCorrection;  ///< 平场校正结构体
	CGXBitmap           *m_pBitmap;                            ///< 显示及画图对象
public:
	///  为图像显示准备资源
	bool PrepareForShowImg();

	/// 获取设备参数,初始化界面控件
	void InitUI();

    /// 刷新UI界面
	void UpDateUI();

	/// 回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

	///  释放为图像显示准备资源
	void UnPrepareForShowImg();

	/// 错误信息提示
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// 初始化枚举类型UI界面
	void InitEnumUI(char* chFeatire, CComboBox *pComboBox, bool bIsImplement);

	/// 初始化白平衡系数相关控件
	void InitWhiteRatioUI();

	/// 刷新显示当前白平衡系数
	void RefreshWhiteRatio();

	/// 初始化增益等相关控件
	void InitGainUI();

	/// 初始化曝光相关控件
	void InitShutterUI();

	/// 初始化灰度值等相关控件
	void InitGrayUI();

	/// 初始化采集帧数等相关控件
	void InitFrameCountUI();

	/// 初始化相机:设置默认参数
	GX_STATUS InitDevice();

	/// 获取设备的属性信息
	GX_STATUS GetDeviceParam();

	/// 图像累加函数
	DX_STATUS AccumulateImage(void *pImgBuf, int nDataWidth, int64_t nlength, VxUint32 *pAccumulateImgBuf);

	/// 平均图像计算函数
	DX_STATUS GetAverageImage(VxUint32 *pImgBuf, int64_t nlength, int nAccumulateNum, int nDataWidth, void *pAverageImgBuf);

	/// 求图像平均灰度值
	int64_t GetAverageGray8bit(BYTE *pImgBuf, int64_t nImgSize);

	/// 采集暗场图像
	void AcquireDarkFieldImg(void);
		
	/// 采集亮场图像
	void AcquireBrightFieldImg(void);

	/// 图像采集帧数
	void GetFrameCount(int &nFrameCount);

	/// 从文件中读取FFC参数
	FFC_STATUS ReadFFCCoefficientsFile(const std::string &strFilePath);

	/// 将FFC参数保存到文件中
	FFC_STATUS WriteFFCCoefficientsFile(const std::string &strFilePath);

// Dialog Data
	//{{AFX_DATA(CGxFlatFieldCorrectionDlg)
	enum { IDD = IDD_GxFlatFieldCorrection_DIALOG };
	double      m_dBalanceRatio;
	double	    m_dGainValue;
	double	    m_dShutterValue;
	CSliderCtrl m_sliderFrameCount;
	CSliderCtrl m_sliderGrayValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxFlatFieldCorrectionDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxFlatFieldCorrectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboWriteBlance();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnCbnSelchangeComboRatioSelector();
	afx_msg void OnEnKillfocusEditBalanceRatio();
	afx_msg void OnKillfocusEditShutter();
	afx_msg void OnKillfocusEditGain();
	afx_msg void OnKillfocusEditGray();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDeltaposSpinExpectGrayValue(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButtonAcquireDarkImg();
	afx_msg void OnBnClickedButtonAcquireBrightImg();
	afx_msg void OnBnClickedButtonExecuteFFC();         
	afx_msg void OnBnClickedCheckPreview();             
	afx_msg void OnBnClickedCheckGray();               
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxFlatFieldCorrectionDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_)
