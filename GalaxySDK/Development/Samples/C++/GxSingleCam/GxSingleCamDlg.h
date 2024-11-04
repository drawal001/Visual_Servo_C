// GxSingleCamDlg.h : 头文件
//
#pragma once

#include "GalaxyIncludes.h"
#include "GXBitmap.h"


// CGxSingleCamDlg 对话框
class CGxSingleCamDlg : public CDialog
{
	//---------------------------------------------------------------------------------
	/**
	\brief   用户继承采集事件处理类
	*/
	//----------------------------------------------------------------------------------
	class CSampleCaptureEventHandler :public ICaptureEventHandler
	{
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
				CGxSingleCamDlg* pSingleCamDlg = (CGxSingleCamDlg*)pUserParam;

				//显示图像
				pSingleCamDlg->m_pBitmap->Show(objImageDataPointer);

				//判断是否需要保存图像
				if (pSingleCamDlg->m_bCheckSaveBmp == TRUE)
				{
					pSingleCamDlg->SavePicture(objImageDataPointer);
				}
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
// 构造
public:
	CGxSingleCamDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GXSINGLECAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnCbnSelchangeComboTriggerMode();
	afx_msg void OnCbnSelchangeComboTriggerSource();
	afx_msg void OnCbnSelchangeComboTriggerActive();
	afx_msg void OnBnClickedCheckSaveBmp();

public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboBalanceWhiteAuto();
	afx_msg void OnCbnSelchangeComboBalanceWhiteSelect();
	afx_msg void OnBnClickedBtnSofttrigger();
	afx_msg void OnEnKillfocusEditShutter();
	afx_msg void OnEnKillfocusEditGain();
	afx_msg void OnEnKillfocusEditBalanceWhiteSerise();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	double m_dEditShutterValue;
	double m_dEditGainValue;
	double m_dEditBalanceRatioValue;
	BOOL   m_bCheckSaveBmp;

public:

	/// 保存图像为BMP格式
	void SavePicture(CImageDataPointer& objImageDataPointer);

	CGXBitmap*  m_pBitmap;                  ///< 保存图像指针

private:

	/// 初始化设备参数
	void __InitParam();

	/// 初始化组合框
	int __InitComBox(gxstring strName, CComboBox* pComboBox, bool bIsImplemented);

	/// 初始化曝光时间控件
	void __InitShutterTime();

	/// 初始化增益控件
	void __InitGain();

	/// 初始化自动白平衡系数控件
	void __InitBalanceWhiteRatio();

	/// 初始化界面
	void __InitUI();

	/// 更新界面
	void __UpdateUI();	

	/// ComboBox设置功能到设备
	void __SetEnumToDevice(gxstring strEnumName,CComboBox* pCombo);

	CGXDevicePointer                  m_objDevicePtr;             ///< 设备句柄
	CGXStreamPointer                  m_objStreamPtr;             ///< 设备流
	CGXFeatureControlPointer          m_objFeatureControlPtr;     ///< 属性控制器
	CGXFeatureControlPointer          m_objStreamFeatureControlPtr; ///< 流层控制器对象
	CWnd*                             m_pWnd;                     ///< 窗口指针
	CSampleCaptureEventHandler*       m_pSampleCaptureEventHandle;///< 回调函数指针

	bool                              m_bIsOpen;                  ///< 设备打开标识
	bool                              m_bIsSnap;                  ///< 设备采集标识
	bool                              m_bColorFilter;             ///< 是否支持彩色相机
	bool                              m_bTriggerMode;             ///< 是否支持触发模式
	bool                              m_bTriggerSource;           ///< 是否支持触发源
	bool                              m_bTriggerActive;           ///< 是否支持触发极性
	bool                              m_bBalanceWhiteAuto;        ///< 是否支持自动白平衡
	bool                              m_bBalanceWhiteRatioSelect; ///< 是否支持白平衡通道选择
	double                            m_dShutterValueMax;         ///< 曝光时间最大值      
	double                            m_dShutterValueMin;         ///< 曝光时间最小值     
	double                            m_dGainValueMax;            ///< 增益最大值
	double                            m_dGainValueMin;            ///< 增益最小值
	double                            m_dBalanceWhiteRatioMax;    ///< 自动白平衡系数最大值
	double                            m_dBalanceWhiteRatioMin;    ///< 自动白平衡系数最小值
	int                               m_nTriggerModeOld;          ///< 记录触发模式
	int                               m_nTriggerSourceOld;        ///< 记录触发源
	int                               m_nTriggerActiveOld;        ///< 记录触发极性
	int                               m_nBalanceWhiteAutoOld;     ///< 记录自动白平衡
	int                               m_nBanlanceWhiteRatioOld;   ///< 记录自动白平衡系数
	gxstring                          m_strBalanceWhiteAutoMode;  ///< 记录自动白平衡方式
	std::string                       m_strSavePath;              ///< 图像保存路径
};
