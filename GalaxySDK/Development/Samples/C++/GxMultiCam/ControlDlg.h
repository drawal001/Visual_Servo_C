#pragma once
#include <iostream>
#include "DeviceProcess.h"
#include "MainFrm.h"
#include "afxwin.h"
#include "map"

#define  DEVICE_CONTS 4               ///< 最多允许同时操作4台设备

// CControlDlg form view

class CControlDlg : public CFormView
{
	DECLARE_DYNCREATE(CControlDlg)

public:
	CControlDlg();           // protected constructor used by dynamic creation
	virtual ~CControlDlg();

public:
	enum { IDD = IDD_CONTROL_DIALOG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnCbnSelchangeComboDeviceList();
	afx_msg void OnCbnSelchangeComboBalanceWhiteAuto();
	afx_msg void OnEnKillfocusEditShutter();
	afx_msg void OnEnKillfocusEditGain();
	afx_msg void OnEnKillfocusEditSnapSpeed();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnDropdownComboDeviceList();
	afx_msg void OnBnClickedCheckShowDeviceFps();
	BOOL      m_bCheckShowDevice;
	CComboBox m_comboxDevice;
	CComboBox m_comboxBalanceWhiteAuto;
	CStatic   m_staticShutterTime;
	CStatic   m_staticGain;
	CStatic   m_staticSnapSpeed;
	CEdit     m_editShutter;
	CEdit     m_editGain;
	CEdit     m_editSnapSpeed;
	double    m_dEditShutter;
	double    m_dEditGain;
	int64_t   m_nEditSnapSpeed;

private:
	
	/// 初始化界面UI
	void __InitUI(CGXFeatureControlPointer objFeatureControlPtr);

	/// 更新界面
	void __UpdateUI(CDeviceProcess* pDeviceProcess);

	GxIAPICPP::gxdeviceinfo_vector m_vectorDeviceInfo;          ///< 枚举到的设备信息
	CDeviceProcess* m_pDeviceProcess[DEVICE_CONTS];             ///< 设备处理类对象
	CDeviceProcess* m_pDeviceProcessCurrent;                    ///< 当前设备处理类对象
	std::map<int, CDeviceProcess*> m_mapDeviceInformation;      ///< 用于存储设备处理类
	bool     m_bIsSnapSpeed;                                    ///< 是否支持采集速度级别
	bool     m_bIsColorFilter;                                  ///< 是否支持Bayer格式
	double   m_dShutterMax;                                     ///< 曝光时间最大值
	double   m_dShutterMin;                                     ///< 曝光时间最小值
	double   m_dGainMax;                                        ///< 增益最大值
	double   m_dGainMin;                                        ///< 增益最小值
	int64_t  m_nSnapSpeedMax;                                   ///< 采集速度级别最大值
	int64_t  m_nSnapSpeedMin;                                   ///< 采集速度级别最小值
	int      m_nDeviceListCurrent;                              ///< 当前设备列表序号
	int      m_nCurrentBalanceAutoWhiteSel;                     ///< 记录白平衡的值
	gxstring m_strBalanceWhiteAuto;                             ///< 当前自动白平衡

public:
	afx_msg void OnDestroy();
};


