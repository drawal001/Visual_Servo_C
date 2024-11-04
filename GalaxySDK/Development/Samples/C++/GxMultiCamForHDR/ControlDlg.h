#pragma once
#include <iostream>
#include "CamerParamDlg.h"
#include "DeviceProcess.h"
#include "MainFrm.h"
#include "afxwin.h"
#include "map"


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



public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnDropdownComboDeviceList();
	afx_msg void OnBnClickedCheckShowDeviceFps();
	BOOL      m_bCheckShowDevice;
	CComboBox m_comboxDevice;

private:
	
	/// 初始化界面UI
	void __InitUI(CGXFeatureControlPointer objFeatureControlPtr);

	/// 更新界面
	void __UpdateUI(CDeviceProcess* pDeviceProcess);

	GxIAPICPP::gxdeviceinfo_vector m_vectorDeviceInfo;          ///< 枚举到的设备信息
	CDeviceProcess* m_pDeviceProcess[DEVICE_CONTS];             ///< 设备处理类对象
	CDeviceProcess* m_pDeviceProcessCurrent;                    ///< 当前设备处理类对象
	std::map<int, CDeviceProcess*> m_mapDeviceInformation;      ///< 用于存储设备处理类

	int      m_nDeviceListCurrent;                              ///< 当前设备列表序号

public:
	afx_msg void OnDestroy();
    afx_msg void OnBnClickedButtonCameraParam();
};


